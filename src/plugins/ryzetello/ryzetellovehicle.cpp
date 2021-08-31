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

#include "ryzetellovehicle.h"
#include "debug.h"
#include "ryzetelloconnection.h"

#include <QDataStream>
#include <QDateTime>
#include <QStandardPaths>

RyzeTelloVehicle::RyzeTelloVehicle(QObject *parent)
    : Kirogi::AbstractVehicle(parent)
    , m_stateSeq(0)
    , m_motorOnTime(-1)
    , m_oldMotorOnTime(-1)
    , m_roll(0.0)
    , m_pitch(0.0)
    , m_yaw(0.0)
    , m_signalStrength(100)
    , m_batteryLevel(100)
    , m_distance(0.0)
    , m_altitude(0.0)
    , m_speed(0.0)
    , m_signalStrengthTimer(std::make_unique<QTimer>(this))
    , m_disconnectTimer(std::make_unique<QTimer>(this))
    , m_connection(nullptr)
{
    m_connection = new RyzeTelloConnection(name());

    // Queued connections across thread boundaries.
    QObject::connect(m_connection, &RyzeTelloConnection::stateChanged, this, &RyzeTelloVehicle::setConnectionState, Qt::QueuedConnection);

    QObject::connect(
        m_connection,
        &RyzeTelloConnection::stateChanged,
        this,
        [this](AbstractVehicle::ConnectionState state) {
            if (state == Connecting) {
                initVehicle();
            }

            if (state == Disconnected) {
                m_stateSeq = 0;
                m_motorOnTime = 0;
                m_oldMotorOnTime = 0;

                m_signalStrengthTimer->stop();

                setFlyingState(Unknown);
            }
        },
        Qt::QueuedConnection);

    QObject::connect(m_connection, &RyzeTelloConnection::responseReceived, this, &RyzeTelloVehicle::processIncomingResponse, Qt::QueuedConnection);

    QObject::connect(m_connection, &RyzeTelloConnection::stateReceived, this, &RyzeTelloVehicle::processIncomingState, Qt::QueuedConnection);

    // Do the networking on a separate thread, so our fixed-tick work never gets
    // blocked by activity on the main thread.
    m_connection->moveToThread(&m_connectionThread);
    QObject::connect(&m_connectionThread, &QThread::finished, m_connection, &QObject::deleteLater);

    m_connectionThread.setObjectName(QStringLiteral("RyzeTelloConnectionThread"));
    m_connectionThread.start();
}

RyzeTelloVehicle::~RyzeTelloVehicle()
{
    m_connectionThread.quit();
    m_connectionThread.wait();
}

QString RyzeTelloVehicle::name() const
{
    return QStringLiteral("Tello");
}

QString RyzeTelloVehicle::iconName() const
{
    return QStringLiteral("uav-quadcopter");
}

Kirogi::AbstractVehicle::VehicleType RyzeTelloVehicle::vehicleType() const
{
    return Kirogi::AbstractVehicle::QuadCopter;
}

QList<Kirogi::AbstractVehicle::VehicleAction> RyzeTelloVehicle::supportedActions() const
{
    return {Kirogi::AbstractVehicle::TakeOff,
            Kirogi::AbstractVehicle::Land,
            Kirogi::AbstractVehicle::FlipForward,
            Kirogi::AbstractVehicle::FlipBackward,
            Kirogi::AbstractVehicle::FlipLeft,
            Kirogi::AbstractVehicle::FlipRight,
            Kirogi::AbstractVehicle::SwitchPerformanceMode};
}

void RyzeTelloVehicle::requestAction(Kirogi::AbstractVehicle::VehicleAction action)
{
    if (!ready()) {
        qCWarning(KIROGI_VEHICLESUPPORT_RYZETELLO) << name() << "Requested an action while not ready, aborting. Current connection state:" << connectionState();

        return;
    }

    switch (action) {
    case TakeOff: {
        if (flying()) {
            return;
        }

        sendCommand(QStringLiteral("takeoff"));
        setFlyingState(TakingOff); // FIXME: We don't /really/ know that without looking at the response.
        break;
    }
    case Land: {
        if (!flying()) {
            return;
        }

        sendCommand(QStringLiteral("land"));
        setFlyingState(Landing); // FIXME: We don't /really/ know that without looking at the response.
        break;
    }
    case FlipForward: {
        if (!flying()) {
            return;
        }

        sendCommand(QStringLiteral("flip f"));
        break;
    }
    case FlipBackward: {
        if (!flying()) {
            return;
        }

        sendCommand(QStringLiteral("flip b"));
        break;
    }
    case FlipLeft: {
        if (!flying()) {
            return;
        }

        sendCommand(QStringLiteral("flip l"));
        break;
    }
    case FlipRight: {
        if (!flying()) {
            return;
        }

        sendCommand(QStringLiteral("flip r"));
        break;
    }
    default: {
    }
    }
}

void RyzeTelloVehicle::pilot(qint8 roll, qint8 pitch, qint8 yaw, qint8 gaz)
{
    if (!flying()) {
        qCWarning(KIROGI_VEHICLESUPPORT_RYZETELLO) << name() << "Piloting request while not flying, aborting. Current flying state:" << flyingState();

        return;
    }

    QMetaObject::invokeMethod(m_connection, "pilot", Qt::QueuedConnection, Q_ARG(qint8, roll), Q_ARG(qint8, pitch), Q_ARG(qint8, yaw), Q_ARG(qint8, gaz));
}

Kirogi::AbstractVehicle::PerformanceMode RyzeTelloVehicle::performanceMode() const
{
    return CustomPerformance; // FIXME
}

void RyzeTelloVehicle::requestPerformanceMode(Kirogi::AbstractVehicle::PerformanceMode mode)
{
    if (performanceMode() != mode) {
        switch (mode) {
        case FilmPerformance: {
            sendCommand(QStringLiteral("speed 10"));

            break;
        }
        case SportPerformance: {
            sendCommand(QStringLiteral("speed 100"));

            break;
        }
        default: {
        }
        }
    }
}

float RyzeTelloVehicle::roll() const
{
    return m_roll;
}

float RyzeTelloVehicle::pitch() const
{
    return m_pitch;
}

float RyzeTelloVehicle::yaw() const
{
    return m_yaw;
}

int RyzeTelloVehicle::signalStrength() const
{
    return m_signalStrength;
}

int RyzeTelloVehicle::batteryLevel() const
{
    return m_batteryLevel;
}

bool RyzeTelloVehicle::gpsSupported() const
{
    return false;
}

float RyzeTelloVehicle::distance() const
{
    return m_distance;
}

float RyzeTelloVehicle::altitude() const
{
    return m_altitude;
}

float RyzeTelloVehicle::speed() const
{
    return m_speed;
}

void RyzeTelloVehicle::connectToVehicle()
{
    if (connectionState() > Disconnected) {
        if (connectionState() > Connecting) {
            qCDebug(KIROGI_VEHICLESUPPORT_RYZETELLO) << name() << "Asked to connect when not disconnected.";
        }

        QMetaObject::invokeMethod(m_connection, "reset", Qt::BlockingQueuedConnection);
    }

    QMetaObject::invokeMethod(m_connection, "handshake", Qt::QueuedConnection);

    // Keep re-trying every 3 seconds until we're successfully connected.
    QTimer::singleShot(3000, this, [this]() {
        if (connectionState() != Ready) {
            qCWarning(KIROGI_VEHICLESUPPORT_RYZETELLO) << name() << "Unable to establish connection within 3 seconds. Starting over.";

            connectToVehicle();
        }
    });
}

void RyzeTelloVehicle::processIncomingResponse(const QString &response)
{
    if (connectionState() < Connected) {
        setConnectionState(Connected);
    }

    if (connectionState() != Ready && response == QStringLiteral("ok")) {
        setConnectionState(Ready);

        return;
    }

    bool ok = false;
    const int signalStrength = response.trimmed().toInt(&ok);

    if (ok) {
        m_disconnectTimer->stop();

        m_signalStrength = signalStrength;
        emit signalStrengthChanged();

        m_signalStrengthTimer->singleShot(2000, this, &RyzeTelloVehicle::pollSignalStrength);
    }
}

void RyzeTelloVehicle::processIncomingState(const QByteArray &state)
{
    // From the SDK doc:
    // "pitch:%d;roll:%d;yaw:%d;vgx:%d;vgy%d;vgz:%d;templ:%d;temph:%d;tof:%d;h:%d;bat:%d;baro:%.2f;time:%d;agx:%.2f;agy:%.2f;agz:%.2f;\r\n"

    ++m_stateSeq;
    m_stateSeq %= 20;

    const QString &encoded = QString::fromUtf8(state);
    const QStringList &segments = encoded.split(QChar(';'));

    float speedX = 0.0;
    float speedY = 0.0;
    float speedZ = 0.0;

    for (const QString &segment : segments) {
        if (segment.startsWith(QStringLiteral("roll"))) {
            m_roll = segment.midRef(5).toFloat() / 100;
        } else if (segment.startsWith(QStringLiteral("pitch"))) {
            m_pitch = segment.midRef(6).toFloat() / 100;
        } else if (segment.startsWith(QStringLiteral("yaw"))) {
            m_yaw = segment.midRef(4).toFloat() / 100;
        } else if (segment.startsWith(QStringLiteral("bat"))) {
            m_batteryLevel = segment.midRef(4).toInt();
        } else if (segment.startsWith(QStringLiteral("tof"))) {
            m_distance = segment.midRef(2).toInt() / 100;
        } else if (segment.startsWith(QStringLiteral("h"))) {
            m_altitude = segment.midRef(2).toInt() / 100;
        } else if (segment.startsWith(QStringLiteral("vgx"))) {
            speedX = segment.midRef(4).toInt() / 100;
        } else if (segment.startsWith(QStringLiteral("vgy"))) {
            speedY = segment.midRef(4).toInt() / 100;
        } else if (segment.startsWith(QStringLiteral("vgz"))) {
            speedZ = segment.midRef(4).toInt() / 100;
        } else if (segment.startsWith(QStringLiteral("time"))) {
            m_motorOnTime = segment.midRef(5).toInt();

            if (m_oldMotorOnTime == -1) {
                m_oldMotorOnTime = m_motorOnTime;
            }
        }
    }

    m_speed = std::max({speedX, speedY, speedZ});

    if (m_stateSeq == 19) {
        if (m_motorOnTime != m_oldMotorOnTime) {
            setFlyingState(Flying);
        } else {
            setFlyingState(Landed);
        }

        m_oldMotorOnTime = m_motorOnTime;
    }

    emit attitudeChanged();
    emit rollChanged();
    emit pitchChanged();
    emit yawChanged();

    emit batteryLevelChanged();

    emit distanceChanged();
    emit altitudeChanged();

    emit speedChanged();
}

void RyzeTelloVehicle::pollSignalStrength()
{
    sendCommand(QStringLiteral("wifi?"));

    if (connectionState() == Ready) {
        m_disconnectTimer->singleShot(500, this, &RyzeTelloVehicle::connectToVehicle);
    }
}

void RyzeTelloVehicle::initVehicle()
{
    sendCommand(QStringLiteral("command"), /* retryForever */ true);

    requestPerformanceMode(FilmPerformance);

    pollSignalStrength();
}

void RyzeTelloVehicle::sendCommand(const QString &command, bool retryForever)
{
    if (connectionState() < Connecting) {
        qCWarning(KIROGI_VEHICLESUPPORT_RYZETELLO) << name() << "Request to send command" << command
                                                   << "rejected. Connection not ready. Current connection state:" << connectionState();
        return;
    }

    QMetaObject::invokeMethod(m_connection, "sendCommand", Qt::QueuedConnection, Q_ARG(QString, command), Q_ARG(bool, retryForever));
}
