/*
 * Copyright 2019 Eike Hein <hein@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "parrotvehicle.h"
#include "debug.h"
#include "parrotconnection.h"
#include "parrotprotocol.h"

#include <KLocalizedString>

#include <QDataStream>
#include <QDateTime>
#include <QStandardPaths>

ParrotVehicle::ParrotVehicle(Type type, const QString &hostName, int port, const QString &productSerial, QObject *parent)
    : Kirogi::AbstractVehicle(parent)
    , m_type(type)
    , m_hostName(hostName)
    , m_productSerial(productSerial)
    , m_initialized(0)
    , m_piloting(false)
    , m_maxRollPitchSpeed(0.0)
    , m_maxRollPitchSpeedMin(0.0)
    , m_maxRollPitchSpeedMax(0.0)
    , m_maxYawSpeed(0.0)
    , m_maxYawSpeedMin(0.0)
    , m_maxYawSpeedMax(0.0)
    , m_maxGazSpeed(0.0)
    , m_maxGazSpeedMin(0.0)
    , m_maxGazSpeedMax(0.0)
    , m_maxTilt(0.0)
    , m_maxTiltMin(0.0)
    , m_maxTiltMax(0.0)
    , m_bankedTurns(false)
    , m_geofence(false)
    , m_maxAltitude(0.0)
    , m_maxAltitudeMin(0.0)
    , m_maxAltitudeMax(0.0)
    , m_maxDistance(0.0)
    , m_maxDistanceMin(0.0)
    , m_maxDistanceMax(0.0)
    , m_roll(0.0)
    , m_pitch(0.0)
    , m_yaw(0.0)
    , m_defaultCameraOrientationTilt(0)
    , m_defaultCameraOrientationPan(0)
    , m_signalStrength(100)
    , m_batteryLevel(100)
    , m_gpsFix(false)
    , m_altitude(0.0)
    , m_speed(0.0)
    , m_videoStreamEnabled(false)
    , m_videoStreamMode(0)
    , m_videoStabilizationMode(0)
    , m_canTakePicture(false)
    , m_isRecordingVideo(false)
    , m_numberOfFlights(0)
    , m_lastFlightDuration(0)
    , m_connection(nullptr)
{
    m_connection = new ParrotConnection(type, name(), hostName, port);

    // Queued connections across thread boundaries.
    QObject::connect(m_connection, &ParrotConnection::stateChanged, this, &ParrotVehicle::setConnectionState, Qt::QueuedConnection);

    QObject::connect(
        m_connection,
        &ParrotConnection::stateChanged,
        this,
        [this](AbstractVehicle::ConnectionState state) {
            if (state == Connected) {
                initVehicle();
            }

            if (state == Disconnected) { // FIXME Reset more stuff.
                m_initialized = 0;
                setFlyingState(Unknown);

                m_videoStreamEnabled = false;
                emit videoStreamEnabledChanged();
                emit videoSourceChanged();
                m_canTakePicture = false;
                emit canTakePictureChanged();
                m_isRecordingVideo = false;
                emit isRecordingVideoChanged();

                m_gpsFix = false;
                emit gpsFixChanged();
            }
        },
        Qt::QueuedConnection);

    QObject::connect(m_connection, &ParrotConnection::commandReceived, this, &ParrotVehicle::processIncomingCommand, Qt::QueuedConnection);

    // Do the networking on a seperate thread, so our fixed-tick work never gets
    // blocked by activity on the main thread.
    m_connection->moveToThread(&m_connectionThread);
    QObject::connect(&m_connectionThread, &QThread::finished, m_connection, &QObject::deleteLater);

    m_connectionThread.setObjectName(QStringLiteral("ParrotConnectionThread"));
    m_connectionThread.start();
}

ParrotVehicle::~ParrotVehicle()
{
    m_connectionThread.quit();
    m_connectionThread.wait();
}

QString ParrotVehicle::name() const
{
    QString prettyName;

    if (m_type == Bebop2) {
        prettyName = i18n("Bebop 2");
    } else if (m_type == Anafi) {
        prettyName = i18n("Anafi");
    } else {
        prettyName = i18n("Unknown Parrot Model");
    }

    return i18nc("%1 = Model name, %2 = Hostname", "%1 (%2)", prettyName, (m_productSerial.isEmpty() ? m_hostName : m_productSerial));
}

QString ParrotVehicle::iconName() const
{
    if (m_type == Bebop2) {
        return QStringLiteral("uav-quadcopter");
        ;
    }

    return Kirogi::AbstractVehicle::iconName();
}

Kirogi::AbstractVehicle::VehicleType ParrotVehicle::vehicleType() const
{
    if (m_type == Bebop2) {
        return Kirogi::AbstractVehicle::QuadCopter;
    }

    return Kirogi::AbstractVehicle::UnknownVehicleType;
}

QList<Kirogi::AbstractVehicle::VehicleAction> ParrotVehicle::supportedActions() const
{
    auto actions = QList<Kirogi::AbstractVehicle::VehicleAction>({Kirogi::AbstractVehicle::TakeOff,
                                                                  Kirogi::AbstractVehicle::Land,
                                                                  Kirogi::AbstractVehicle::FlatTrim,
                                                                  Kirogi::AbstractVehicle::SwitchPerformanceMode,
                                                                  Kirogi::AbstractVehicle::SetMaxRollSpeed,
                                                                  Kirogi::AbstractVehicle::SetMaxPitchSpeed,
                                                                  Kirogi::AbstractVehicle::SetMaxYawSpeed,
                                                                  Kirogi::AbstractVehicle::SetMaxTilt,
                                                                  Kirogi::AbstractVehicle::SetMaxGazSpeed,
                                                                  Kirogi::AbstractVehicle::ToggleBankedTurns,
                                                                  Kirogi::AbstractVehicle::ToggleGeofence,
                                                                  Kirogi::AbstractVehicle::SetMaxAltitude,
                                                                  Kirogi::AbstractVehicle::SetMaxDistance,
                                                                  Kirogi::AbstractVehicle::ToggleVideoStream,
                                                                  Kirogi::AbstractVehicle::RecordVideo});

    // FIXME TODO: Implement camera mode changing.
    // actions << Kirogi::AbstractVehicle::TakePicture;

    // FIXME TODO: Look for alternatives for the Anafi.
    if (m_type == Bebop2) {
        actions += QList<Kirogi::AbstractVehicle::VehicleAction>(
            {Kirogi::AbstractVehicle::FlipForward, Kirogi::AbstractVehicle::FlipBackward, Kirogi::AbstractVehicle::FlipLeft, Kirogi::AbstractVehicle::FlipRight, Kirogi::AbstractVehicle::ToggleVideoStabilization});
    }

    return actions;
}

bool ParrotVehicle::piloting() const
{
    return m_piloting;
}

void ParrotVehicle::requestMoveTo(QGeoCoordinate destination)
{
    if (!flying()) {
        qCWarning(KIROGI_VEHICLESUPPORT_PARROT) << name() << "Move-to request while not flying, aborting. Current flying state:" << flyingState();

        return;
    }

    if (!gpsFix()) {
        qCWarning(KIROGI_VEHICLESUPPORT_PARROT) << name() << "Move-to request without a GPS fix, aborting.";

        return;
    }

    sendCommand(Parrot::Ardrone3PilotingmoveTo, {destination.latitude(), destination.longitude(), destination.altitude(), 1, 0.0});
}

void ParrotVehicle::setPiloting(bool piloting)
{
    if (m_piloting != piloting) {
        if (piloting) {
            sendCommand(Parrot::CommonControllerisPiloting, {1});
        } else {
            sendCommand(Parrot::CommonControllerisPiloting, {0});
        }
    }
}

void ParrotVehicle::requestAction(Kirogi::AbstractVehicle::VehicleAction action)
{
    if (!ready()) {
        qCWarning(KIROGI_VEHICLESUPPORT_PARROT) << name() << "Requested an action while not ready, aborting. Current connection state:" << connectionState();

        return;
    }

    switch (action) {
    case TakeOff: {
        if (flying()) {
            return;
        }

        sendCommand(Parrot::Ardrone3PilotingTakeOff);
        break;
    }
    case Land: {
        if (!flying()) {
            return;
        }

        sendCommand(Parrot::Ardrone3PilotingLanding);
        break;
    }
    case FlatTrim: {
        sendCommand(Parrot::Ardrone3PilotingFlatTrim);
        break;
    }
    case FlipForward: {
        if (!flying()) {
            return;
        }

        sendCommand(Parrot::Ardrone3AnimationsFlip, {0});
        break;
    }
    case FlipBackward: {
        if (!flying()) {
            return;
        }

        sendCommand(Parrot::Ardrone3AnimationsFlip, {1});
        break;
    }
    case FlipLeft: {
        if (!flying()) {
            return;
        }

        sendCommand(Parrot::Ardrone3AnimationsFlip, {3});
        break;
    }
    case FlipRight: {
        if (!flying()) {
            return;
        }

        sendCommand(Parrot::Ardrone3AnimationsFlip, {2});
        break;
    }
    case ToggleBankedTurns: {
        requestEnableBankedTurns(!bankedTurns());
        break;
    }
    case ToggleGeofence: {
        requestEnableGeofence(!geofence());
        break;
    }
    case ToggleVideoStabilization: {
        requestEnableVideoStabilization(!videoStabilization());
        break;
    }
    case TakePicture: {
        sendCommand(Parrot::Ardrone3MediaRecordPictureV2);
        break;
    }
    case RecordVideo: {
        if (m_isRecordingVideo) {
            sendCommand(Parrot::Ardrone3MediaRecordVideoV2, {0});
        } else {
            sendCommand(Parrot::Ardrone3MediaRecordVideoV2, {1});
        }
        break;
    }
    default: {
        AbstractVehicle::requestAction(action);
    }
    }
}

void ParrotVehicle::pilot(qint8 roll, qint8 pitch, qint8 yaw, qint8 gaz)
{
    if (!flying()) {
        qCWarning(KIROGI_VEHICLESUPPORT_PARROT) << name() << "Piloting request while not flying, aborting. Current flying state:" << flyingState();

        return;
    }

    QMetaObject::invokeMethod(m_connection, "pilot", Qt::QueuedConnection, Q_ARG(qint8, roll), Q_ARG(qint8, pitch), Q_ARG(qint8, yaw), Q_ARG(qint8, gaz));
}

Kirogi::AbstractVehicle::PerformanceMode ParrotVehicle::performanceMode() const
{
    // FIXME TODO: These values are coming from FreeFlight, but particularly
    // the SportPerformance values don't feel satisfactory to me.

    if (ready()) {
        if (m_type == Bebop2) {
            if (m_maxRollPitchSpeed == 80 && m_maxYawSpeed == 13 && m_maxGazSpeed == 1 && m_maxTilt == 8 && m_bankedTurns == true && m_videoStreamMode == 1 && videoStabilization()) {
                return FilmPerformance;
            } else if (m_maxRollPitchSpeed == 200 && m_maxYawSpeed == 150 && m_maxGazSpeed == 5 && m_maxTilt == 35 && m_bankedTurns == false && m_videoStreamMode == 0 && !videoStabilization()) {
                return SportPerformance;
            }
        } else if (m_type == Anafi) {
            // FIXME TODO: Look for an equivalent to `m_videoStreamMode` for the Anafi.
            // FIXME TODO: Look for an equivalent to `videoStabilization()` for the
            // Anafi.
            // FIXME TODO: FreeFlight 6 also has a 'Max camera tilt speed' setting we
            // may need before long.
            if (m_maxRollPitchSpeed == 80 && m_maxYawSpeed == 10 && m_maxGazSpeed == 1 && m_maxTilt == 10 && m_bankedTurns == true) {
                return FilmPerformance;
            } else if (m_maxRollPitchSpeed == 200 && m_maxYawSpeed == 30 && m_maxGazSpeed == 2 // FreeFlight 6 uses 1.9, but there's some weird precision mismatch issue.
                       && m_maxTilt == 25 && m_bankedTurns == false) {
                return SportPerformance;
            }
        }
    }

    return CustomPerformance;
}

void ParrotVehicle::requestPerformanceMode(Kirogi::AbstractVehicle::PerformanceMode mode)
{
    if (performanceMode() != mode) {
        if (m_type == Bebop2) {
            switch (mode) {
            case FilmPerformance: {
                sendCommand(Parrot::Ardrone3SpeedSettingsMaxPitchRollRotationSpeed, {80});
                sendCommand(Parrot::Ardrone3SpeedSettingsMaxRotationSpeed, {13});
                sendCommand(Parrot::Ardrone3SpeedSettingsMaxVerticalSpeed, {1});
                sendCommand(Parrot::Ardrone3PilotingSettingsMaxTilt, {8});
                sendCommand(Parrot::Ardrone3PilotingSettingsBankedTurn, {1});
                sendCommand(Parrot::Ardrone3MediaStreamingVideoStreamMode, {1});
                requestEnableVideoStabilization(true);

                break;
            }
            case SportPerformance: {
                sendCommand(Parrot::Ardrone3SpeedSettingsMaxPitchRollRotationSpeed, {200});
                sendCommand(Parrot::Ardrone3SpeedSettingsMaxRotationSpeed, {150});
                sendCommand(Parrot::Ardrone3SpeedSettingsMaxVerticalSpeed, {5});
                sendCommand(Parrot::Ardrone3PilotingSettingsMaxTilt, {35});
                sendCommand(Parrot::Ardrone3PilotingSettingsBankedTurn, {0});
                sendCommand(Parrot::Ardrone3MediaStreamingVideoStreamMode, {0});
                requestEnableVideoStabilization(false);

                break;
            }
            default: {
            }
            }
        } else if (m_type == Anafi) {
            switch (mode) {
            case FilmPerformance: {
                sendCommand(Parrot::Ardrone3SpeedSettingsMaxPitchRollRotationSpeed, {80});
                sendCommand(Parrot::Ardrone3SpeedSettingsMaxRotationSpeed, {10});
                sendCommand(Parrot::Ardrone3SpeedSettingsMaxVerticalSpeed, {1});
                sendCommand(Parrot::Ardrone3PilotingSettingsMaxTilt, {10});
                sendCommand(Parrot::Ardrone3PilotingSettingsBankedTurn, {1});

                break;
            }
            case SportPerformance: {
                sendCommand(Parrot::Ardrone3SpeedSettingsMaxPitchRollRotationSpeed, {200});
                sendCommand(Parrot::Ardrone3SpeedSettingsMaxRotationSpeed, {30});
                sendCommand(Parrot::Ardrone3SpeedSettingsMaxVerticalSpeed, {2});
                sendCommand(Parrot::Ardrone3PilotingSettingsMaxTilt, {25});
                sendCommand(Parrot::Ardrone3PilotingSettingsBankedTurn, {0});

                break;
            }
            default: {
            }
            }
        }
    }
}

float ParrotVehicle::maxRollSpeed() const
{
    return m_maxRollPitchSpeed;
}

void ParrotVehicle::requestMaxRollSpeed(float speed)
{
    if (m_maxRollPitchSpeed != speed) {
        sendCommand(Parrot::Ardrone3SpeedSettingsMaxPitchRollRotationSpeed, {speed});
    }
}

float ParrotVehicle::maxRollSpeedMin() const
{
    return m_maxRollPitchSpeedMin;
}

float ParrotVehicle::maxRollSpeedMax() const
{
    return m_maxRollPitchSpeedMax;
}

float ParrotVehicle::maxPitchSpeed() const
{
    return m_maxRollPitchSpeed;
}

void ParrotVehicle::requestMaxPitchSpeed(float speed)
{
    if (m_maxRollPitchSpeed != speed) {
        sendCommand(Parrot::Ardrone3SpeedSettingsMaxPitchRollRotationSpeed, {speed});
    }
}

float ParrotVehicle::maxPitchSpeedMin() const
{
    return m_maxRollPitchSpeedMin;
}

float ParrotVehicle::maxPitchSpeedMax() const
{
    return m_maxRollPitchSpeedMax;
}

float ParrotVehicle::maxYawSpeed() const
{
    return m_maxYawSpeed;
}

void ParrotVehicle::requestMaxYawSpeed(float speed)
{
    if (m_maxYawSpeed != speed) {
        sendCommand(Parrot::Ardrone3SpeedSettingsMaxRotationSpeed, {speed});
    }
}

float ParrotVehicle::maxYawSpeedMin() const
{
    return m_maxYawSpeedMin;
}

float ParrotVehicle::maxYawSpeedMax() const
{
    return m_maxYawSpeedMax;
}

float ParrotVehicle::maxGazSpeed() const
{
    return m_maxGazSpeed;
}

void ParrotVehicle::requestMaxGazSpeed(float speed)
{
    if (m_maxGazSpeed != speed) {
        sendCommand(Parrot::Ardrone3SpeedSettingsMaxVerticalSpeed, {speed});
    }
}

float ParrotVehicle::maxGazSpeedMin() const
{
    return m_maxGazSpeedMin;
}

float ParrotVehicle::maxGazSpeedMax() const
{
    return m_maxGazSpeedMax;
}

float ParrotVehicle::maxTilt() const
{
    return m_maxTilt;
}

void ParrotVehicle::requestMaxTilt(float tilt)
{
    if (m_maxTilt != tilt) {
        sendCommand(Parrot::Ardrone3PilotingSettingsMaxTilt, {tilt});
    }
}

float ParrotVehicle::maxTiltMin() const
{
    return m_maxTiltMin;
}

float ParrotVehicle::maxTiltMax() const
{
    return m_maxTiltMax;
}

bool ParrotVehicle::bankedTurns() const
{
    return m_bankedTurns;
}

void ParrotVehicle::requestEnableBankedTurns(bool enable)
{
    if (m_bankedTurns != enable) {
        if (enable) {
            sendCommand(Parrot::Ardrone3PilotingSettingsBankedTurn, {1});
        } else {
            sendCommand(Parrot::Ardrone3PilotingSettingsBankedTurn, {0});
        }
    }
}

void ParrotVehicle::requestEnableGeofence(bool enable)
{
    if (m_geofence != enable) {
        if (enable) {
            sendCommand(Parrot::Ardrone3PilotingSettingsNoFlyOverMaxDistance, {1});
        } else {
            sendCommand(Parrot::Ardrone3PilotingSettingsNoFlyOverMaxDistance, {0});
        }
    }
}

float ParrotVehicle::maxAltitude() const
{
    return m_maxAltitude;
}

void ParrotVehicle::requestMaxAltitude(float altitude)
{
    if (m_maxAltitude != altitude) {
        sendCommand(Parrot::Ardrone3PilotingSettingsMaxAltitude, {altitude});
    }
}

float ParrotVehicle::maxAltitudeMin() const
{
    return m_maxAltitudeMin;
}

float ParrotVehicle::maxAltitudeMax() const
{
    return m_maxAltitudeMax;
}

bool ParrotVehicle::geofence() const
{
    return m_geofence;
}

float ParrotVehicle::maxDistance() const
{
    return m_maxDistance;
}

void ParrotVehicle::requestMaxDistance(float distance)
{
    if (m_maxDistance != distance) {
        sendCommand(Parrot::Ardrone3PilotingSettingsMaxDistance, {distance});
    }
}

float ParrotVehicle::maxDistanceMin() const
{
    return m_maxDistanceMin;
}

float ParrotVehicle::maxDistanceMax() const
{
    return m_maxDistanceMax;
}

float ParrotVehicle::roll() const
{
    return m_roll;
}

float ParrotVehicle::pitch() const
{
    return m_pitch;
}

float ParrotVehicle::yaw() const
{
    return m_yaw;
}

int ParrotVehicle::signalStrength() const
{
    if (m_signalStrength <= -100) {
        return 0;
    } else if (m_signalStrength >= -50) {
        return 100;
    }

    return 2 * (m_signalStrength + 100);
}

int ParrotVehicle::batteryLevel() const
{
    return m_batteryLevel;
}

bool ParrotVehicle::gpsSupported() const
{
    return true;
}

bool ParrotVehicle::gpsFix() const
{
    return m_gpsFix;
}

QGeoCoordinate ParrotVehicle::gpsPosition() const
{
    return m_gpsPosition;
}

float ParrotVehicle::altitude() const
{
    return m_altitude;
}

void ParrotVehicle::setControllerGpsPosition(const QGeoCoordinate &position)
{
    sendCommand(Parrot::Ardrone3GPSSettingsSendControllerGPS, {position.latitude(), position.longitude(), position.altitude()});
}

void ParrotVehicle::requestReturnHome()
{
    sendCommand(Parrot::Ardrone3PilotingNavigateHome);
}

float ParrotVehicle::speed() const
{
    return m_speed;
}

QString ParrotVehicle::videoSource() const
{
    if (videoStreamEnabled()) {
        if (m_type == Bebop2) {
            return QStringLiteral("udpsrc port=55004 ! application/x-rtp, clock-rate=90000,payload=96 ! rtph264depay ! video/x-h264 ! queue ! h264parse ! decodebin !");
        } else if (m_type == Anafi) {
            return QStringLiteral("rtspsrc location=rtsp://192.168.42.1/live latency=5 ! rtph264depay ! video/x-h264 ! queue ! h264parse ! decodebin !");
        }
    }

    return {};
}

bool ParrotVehicle::videoStreamEnabled() const
{
    return m_videoStreamEnabled;
}

void ParrotVehicle::requestEnableVideoStream(bool enable)
{
    if (m_videoStreamEnabled != enable) {
        if (enable) {
            sendCommand(Parrot::Ardrone3MediaStreamingVideoEnable, {1});
            sendCommand(Parrot::Ardrone3PictureSettingsVideoAutorecordSelection, {0, 0});

            // The Anafi won't send us Ardrone3MediaStreamingStateVideoEnableChanged.
            if (m_type == Anafi) {
                m_videoStreamEnabled = true;
                emit videoStreamEnabledChanged();
                emit videoSourceChanged();
            }
        } else {
            // The Anafi won't send us Ardrone3MediaStreamingStateVideoEnableChanged.
            if (m_type == Anafi) {
                m_videoStreamEnabled = false;
                emit videoStreamEnabledChanged();
                emit videoSourceChanged();
            }

            sendCommand(Parrot::Ardrone3MediaStreamingVideoEnable, {0});
        }
    }
}

bool ParrotVehicle::videoStabilization() const
{
    return (m_videoStabilizationMode == 0);
}

void ParrotVehicle::requestEnableVideoStabilization(bool enable)
{
    const int requestedMode = enable ? 0 : 3;

    if (m_videoStabilizationMode != requestedMode) {
        if (enable) {
            sendCommand(Parrot::Ardrone3PictureSettingsVideoStabilizationMode, {0});
        } else {
            sendCommand(Parrot::Ardrone3PictureSettingsVideoStabilizationMode, {3});
        }
    }
}

bool ParrotVehicle::canTakePicture() const
{
    return m_canTakePicture;
}

bool ParrotVehicle::isRecordingVideo() const
{
    return m_isRecordingVideo;
}

quint16 ParrotVehicle::numberOfFlights() const
{
    return m_numberOfFlights;
}

quint16 ParrotVehicle::lastFlightDuration() const
{
    return m_lastFlightDuration;
}

void ParrotVehicle::connectToVehicle()
{
    if (connectionState() > Disconnected) {
        qCDebug(KIROGI_VEHICLESUPPORT_PARROT) << name() << "Asked to connect when not disconnected.";

        QMetaObject::invokeMethod(m_connection, "reset", Qt::BlockingQueuedConnection);
    }

    QMetaObject::invokeMethod(m_connection, "handshake", Qt::QueuedConnection, Q_ARG(QString, m_productSerial));

    // Keep re-trying every 3 seconds until we're successfully connected.
    QTimer::singleShot(3000, this, [this]() {
        if (connectionState() != Ready) {
            qCWarning(KIROGI_VEHICLESUPPORT_PARROT) << name() << "Unable to establish connection within 3 seconds. Starting over.";

            connectToVehicle();
        }
    });
}

void ParrotVehicle::processIncomingCommand(const ParrotCommand &command)
{
    const Parrot::Command id = command.id();

    switch (id) {
    case Parrot::Ardrone3PilotingStateAttitudeChanged: {
        QDataStream s(command.data);
        s.setByteOrder(QDataStream::LittleEndian);
        s.setFloatingPointPrecision(QDataStream::SinglePrecision);

        s >> m_roll >> m_pitch >> m_yaw;

        emit attitudeChanged();
        emit rollChanged();
        emit pitchChanged();
        emit yawChanged();

        break;
    }
    case Parrot::Ardrone3PilotingStateAltitudeChanged: {
        QDataStream s(command.data);
        s.setByteOrder(QDataStream::LittleEndian);
        s.setFloatingPointPrecision(QDataStream::DoublePrecision);

        s >> m_altitude;

        emit altitudeChanged();

        break;
    }
    case Parrot::Ardrone3PilotingStateSpeedChanged: {
        QDataStream s(command.data);
        s.setByteOrder(QDataStream::LittleEndian);
        s.setFloatingPointPrecision(QDataStream::SinglePrecision);

        float speedX;
        float speedY;
        float speedZ;

        s >> speedX >> speedY >> speedZ;

        m_speed = std::max({speedX, speedY, speedZ});

        emit speedChanged();

        break;
    }
    case Parrot::Ardrone3PilotingStatePositionChanged: {
        QDataStream s(command.data);
        s.setByteOrder(QDataStream::LittleEndian);
        s.setFloatingPointPrecision(QDataStream::DoublePrecision);

        qreal latitude;
        qreal longitude;
        qreal altitude;

        s >> latitude >> longitude >> altitude;

        QGeoCoordinate newPos(latitude, longitude, altitude);

        if (newPos.isValid()) {
            m_gpsPosition = newPos;
        }

        emit gpsPositionChanged();

        break;
    }
    case Parrot::CommonCommonStateWifiSignalChanged: {
        QDataStream s(command.data);
        s.setByteOrder(QDataStream::LittleEndian);

        s >> m_signalStrength;

        emit signalStrengthChanged();

        break;
    }
    case Parrot::Ardrone3GPSSettingsStateGPSFixStateChanged: {
        QDataStream s(command.data);
        s.setByteOrder(QDataStream::LittleEndian);

        quint8 gpsFix = 0;

        s >> gpsFix;

        m_gpsFix = (gpsFix == 1);

        emit gpsFixChanged();

        break;
    }
    case Parrot::CommonCommonStateBatteryStateChanged: {
        QDataStream s(command.data);
        s.setByteOrder(QDataStream::LittleEndian);

        qint8 battery;

        s >> battery;

        m_batteryLevel = battery;
        emit batteryLevelChanged();

        break;
    }
    case Parrot::Ardrone3PilotingStateFlyingStateChanged: {
        QDataStream s(command.data);
        s.setByteOrder(QDataStream::LittleEndian);

        qint32 flyingState;

        s >> flyingState;

        switch (flyingState) {
        case 0: {
            setFlyingState(Landed);
            break;
        }
        case 1: {
            setFlyingState(TakingOff);
            break;
        }
        case 2: {
            setFlyingState(Hovering);
            break;
        }
        case 3: {
            setFlyingState(Flying);
            break;
        }
        case 4: {
            setFlyingState(Landing);
            break;
        }
        default: {
            setFlyingState(Unknown);
        }
        }

        break;
    }
    case Parrot::Ardrone3SpeedSettingsStateMaxPitchRollRotationSpeedChanged: {
        QDataStream s(command.data);
        s.setByteOrder(QDataStream::LittleEndian);
        s.setFloatingPointPrecision(QDataStream::SinglePrecision);

        s >> m_maxRollPitchSpeed >> m_maxRollPitchSpeedMin >> m_maxRollPitchSpeedMax;

        emit maxRollSpeedChanged();
        emit maxRollSpeedMinChanged();
        emit maxRollSpeedMaxChanged();

        emit maxPitchSpeedChanged();
        emit maxPitchSpeedMinChanged();
        emit maxPitchSpeedMaxChanged();

        emit performanceModeChanged();

        break;
    }
    case Parrot::Ardrone3SpeedSettingsStateMaxRotationSpeedChanged: {
        QDataStream s(command.data);
        s.setByteOrder(QDataStream::LittleEndian);
        s.setFloatingPointPrecision(QDataStream::SinglePrecision);

        s >> m_maxYawSpeed >> m_maxYawSpeedMin >> m_maxYawSpeedMax;

        emit maxYawSpeedChanged();
        emit maxYawSpeedMinChanged();
        emit maxYawSpeedMaxChanged();

        emit performanceModeChanged();

        break;
    }
    case Parrot::Ardrone3SpeedSettingsStateMaxVerticalSpeedChanged: {
        QDataStream s(command.data);
        s.setByteOrder(QDataStream::LittleEndian);
        s.setFloatingPointPrecision(QDataStream::SinglePrecision);

        s >> m_maxGazSpeed >> m_maxGazSpeedMin >> m_maxGazSpeedMax;

        emit maxGazSpeedChanged();
        emit maxGazSpeedMinChanged();
        emit maxGazSpeedMaxChanged();

        emit performanceModeChanged();

        break;
    }
    case Parrot::Ardrone3PilotingSettingsStateMaxTiltChanged: {
        QDataStream s(command.data);
        s.setByteOrder(QDataStream::LittleEndian);
        s.setFloatingPointPrecision(QDataStream::SinglePrecision);

        s >> m_maxTilt >> m_maxTiltMin >> m_maxTiltMax;

        emit maxTiltChanged();
        emit maxTiltMinChanged();
        emit maxTiltMaxChanged();

        emit performanceModeChanged();

        break;
    }
    case Parrot::Ardrone3PilotingSettingsStateBankedTurnChanged: {
        QDataStream s(command.data);
        s.setByteOrder(QDataStream::LittleEndian);

        quint8 bankedTurns;

        s >> bankedTurns;

        m_bankedTurns = (bankedTurns == 1);

        emit bankedTurnsChanged();

        emit performanceModeChanged();

        break;
    }
    case Parrot::Ardrone3PilotingSettingsStateMaxAltitudeChanged: {
        QDataStream s(command.data);
        s.setByteOrder(QDataStream::LittleEndian);
        s.setFloatingPointPrecision(QDataStream::SinglePrecision);

        s >> m_maxAltitude >> m_maxAltitudeMin >> m_maxAltitudeMax;

        emit maxAltitudeChanged();
        emit maxAltitudeMinChanged();
        emit maxAltitudeMaxChanged();

        break;
    }
    case Parrot::Ardrone3PilotingSettingsStateNoFlyOverMaxDistanceChanged: {
        QDataStream s(command.data);
        s.setByteOrder(QDataStream::LittleEndian);

        quint8 geofence;

        s >> geofence;

        m_geofence = (geofence == 1);
        emit geofenceChanged();

        break;
    }
    case Parrot::Ardrone3PilotingSettingsStateMaxDistanceChanged: {
        QDataStream s(command.data);
        s.setByteOrder(QDataStream::LittleEndian);
        s.setFloatingPointPrecision(QDataStream::SinglePrecision);

        s >> m_maxDistance >> m_maxDistanceMin >> m_maxDistanceMax;

        emit maxDistanceChanged();
        emit maxDistanceMinChanged();
        emit maxDistanceMaxChanged();

        break;
    }
    case Parrot::Ardrone3SettingsStateMotorFlightsStatusChanged: {
        QDataStream s(command.data);
        s.setByteOrder(QDataStream::LittleEndian);

        s >> m_numberOfFlights >> m_lastFlightDuration;

        emit numberOfFlightsChanged();
        emit lastFlightDurationChanged();

        break;
    }
    case Parrot::Ardrone3MediaStreamingStateVideoEnableChanged: {
        QDataStream s(command.data);
        s.setByteOrder(QDataStream::LittleEndian);

        qint32 videoStreamEnabled;

        s >> videoStreamEnabled;

        m_videoStreamEnabled = (videoStreamEnabled == 0);
        emit videoStreamEnabledChanged();
        emit videoSourceChanged();

        break;
    }
    case Parrot::Ardrone3MediaStreamingStateVideoStreamModeChanged: {
        QDataStream s(command.data);
        s.setByteOrder(QDataStream::LittleEndian);

        s >> m_videoStreamMode;

        emit performanceModeChanged();

        break;
    }
    case Parrot::Ardrone3PictureSettingsStateVideoStabilizationModeChanged: {
        QDataStream s(command.data);
        s.setByteOrder(QDataStream::LittleEndian);

        s >> m_videoStabilizationMode;

        emit performanceModeChanged();

        break;
    }
    case Parrot::Ardrone3MediaRecordStatePictureStateChangedV2: {
        QDataStream s(command.data);
        s.setByteOrder(QDataStream::LittleEndian);

        qint32 canTakePictureState;

        s >> canTakePictureState;

        m_canTakePicture = (canTakePictureState == 0);

        emit canTakePictureChanged();

        break;
    }
    case Parrot::Ardrone3MediaRecordStateVideoStateChangedV2: {
        QDataStream s(command.data);
        s.setByteOrder(QDataStream::LittleEndian);

        qint32 videoRecordingState;

        s >> videoRecordingState;

        m_isRecordingVideo = (videoRecordingState == 1);

        emit isRecordingVideoChanged();

        break;
    }
    case Parrot::Ardrone3CameraStatedefaultCameraOrientationV2: {
        QDataStream s(command.data);
        s.setByteOrder(QDataStream::LittleEndian);

        s >> m_defaultCameraOrientationTilt >> m_defaultCameraOrientationPan;

        break;
    }
    case Parrot::CommonCommonStateAllStatesChanged:
    case Parrot::CommonSettingsStateAllSettingsChanged: {
        ++m_initialized;

        // If we got both of those for the first time within a session,
        // we're now Ready.
        if (m_initialized == 2) {
            qCDebug(KIROGI_VEHICLESUPPORT_PARROT) << name() << "All initial state received.";
            setConnectionState(Ready);
        }

        break;
    }
    case Parrot::UnknownCommand: {
        //             qCWarning(KIROGI_VEHICLESUPPORT_PARROT) << "Unknown command:"
        //                 << command.tuple.productId
        //                 << command.tuple.classId
        //                 << command.tuple.commandId;

        break;
    }
    default: {
        //             qCWarning(KIROGI_VEHICLESUPPORT_PARROT) << "No handler for command:" << id;
    }
    }
}

void ParrotVehicle::initVehicle()
{
    qCDebug(KIROGI_VEHICLESUPPORT_PARROT) << name() << "Sending initialization commands to vehicle" << name() << "...";

    const QDateTime &now = QDateTime::currentDateTime();

    sendCommand(Parrot::CommonCommonCurrentDate, {now.date().toString(Qt::ISODate)}, true);
    sendCommand(Parrot::CommonCommonCurrentTime, {now.time().toString(QStringLiteral("THHmmss+0000"))}, true);
    sendCommand(Parrot::CommonCommonAllStates, QVariantList(), true);
    sendCommand(Parrot::CommonSettingsAllSettings, QVariantList(), true);

    // FIXME TODO: Possibly delay connectionState=Ready until the all of following
    // are done, too.
    if (m_piloting) {
        sendCommand(Parrot::CommonControllerisPiloting, {1});
    } else {
        sendCommand(Parrot::CommonControllerisPiloting, {0});
    }

    requestPerformanceMode(FilmPerformance);
    requestMaxAltitude(30);
    requestMaxDistance(50);
    requestEnableGeofence(true);

    // FIXME TODO: Try to center camera upon successful connection.
    //     sendCommand(Parrot::Ardrone3CameraOrientationV2, {m_defaultCameraOrientationTilt,
    //         m_defaultCameraOrientationPan});

    // Set preferred home type to pilot.
    sendCommand(Parrot::Ardrone3GPSSettingsHomeType, {1});
}

void ParrotVehicle::sendCommand(Parrot::Command command, const QVariantList &arguments, bool retryForever)
{
    if (!connected()) {
        qCWarning(KIROGI_VEHICLESUPPORT_PARROT) << name() << "Request to send command" << command << "rejected. Connection not ready. Current connection state:" << connectionState();
        return;
    }

    QMetaObject::invokeMethod(m_connection, "sendCommand", Qt::QueuedConnection, Q_ARG(Parrot::Command, command), Q_ARG(QVariantList, arguments), Q_ARG(bool, retryForever));
}
