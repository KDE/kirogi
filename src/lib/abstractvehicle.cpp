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

#include <memory>

#include "abstractvehicle.h"
#include "debug.h"
#include "parametermodel.h"

#include <QElapsedTimer>
#include <QTimer>

namespace Kirogi
{
class Q_DECL_HIDDEN AbstractVehicle::Private
{
public:
    explicit Private(AbstractVehicle *q);
    ~Private();

    static int s_availableId;
    int id;
    AbstractVehicle::ConnectionState connectionState = AbstractVehicle::Disconnected;
    AbstractVehicle::FlyingState flyingState = AbstractVehicle::Unknown;

    QElapsedTimer *flightTime = nullptr;
    std::unique_ptr<QTimer> flightTimeTimer;

    ParameterModel parameters;

private:
    AbstractVehicle *m_q;
};

int AbstractVehicle::Private::s_availableId = 0;

AbstractVehicle::Private::Private(AbstractVehicle *q)
    : m_q(q)
{
    id = s_availableId;
    ++s_availableId;
}

AbstractVehicle::Private::~Private()
{
    delete flightTime;
}

AbstractVehicle::AbstractVehicle(QObject *parent)
    : QObject(parent)
    , d(new Private(this))
{
}

AbstractVehicle::~AbstractVehicle()
{
}

int AbstractVehicle::id() const
{
    return d->id;
}

QString AbstractVehicle::iconName() const
{
    return QStringLiteral("uav");
}

Kirogi::AbstractVehicle::VehicleType AbstractVehicle::vehicleType() const
{
    return UnknownVehicleType;
}

AbstractVehicle::ConnectionState AbstractVehicle::connectionState() const
{
    return d->connectionState;
}

void Kirogi::AbstractVehicle::setConnectionState(Kirogi::AbstractVehicle::ConnectionState state)
{
    if (connectionState() != state) {
        qCDebug(KIROGI_CORE) << name() << "changed connection state to:" << state;
        d->connectionState = state;
        emit connectionStateChanged();
    }
}

Kirogi::AbstractVehicle::FlyingState AbstractVehicle::flyingState() const
{
    return d->flyingState;
}

void Kirogi::AbstractVehicle::setFlyingState(Kirogi::AbstractVehicle::FlyingState state)
{
    if (flyingState() != state) {
        qCDebug(KIROGI_CORE) << name() << "changed flying state to:" << state;
        d->flyingState = state;
        emit flyingStateChanged();

        if (state > Landed) {
            if (!d->flightTime) {
                d->flightTime = new QElapsedTimer();
            }

            if (!d->flightTimeTimer) {
                d->flightTimeTimer = std::make_unique<QTimer>(this);
                QObject::connect(d->flightTimeTimer.get(), &QTimer::timeout, this, &AbstractVehicle::flightTimeChanged);
                d->flightTimeTimer->setTimerType(Qt::PreciseTimer);
                d->flightTimeTimer->setInterval(1000);
            }

            d->flightTime->start();
            d->flightTimeTimer->start();
        } else {
            if (d->flightTime) {
                d->flightTime->invalidate();
            }

            if (d->flightTimeTimer) {
                d->flightTimeTimer->stop();
            }
        }
    }
}

bool AbstractVehicle::connected() const
{
    return connectionState() >= Connected;
}

bool AbstractVehicle::ready() const
{
    return connectionState() >= Ready;
}

bool AbstractVehicle::flying() const
{
    return flyingState() >= TakingOff;
}

QList<Kirogi::AbstractVehicle::VehicleAction> AbstractVehicle::supportedActions() const
{
    return QList<VehicleAction>();
}

bool AbstractVehicle::isActionSupported(Kirogi::AbstractVehicle::VehicleAction action) const
{
    return supportedActions().contains(action);
}

bool AbstractVehicle::piloting() const
{
    return false;
}

void AbstractVehicle::setPiloting(bool piloting)
{
    Q_UNUSED(piloting)
}

int AbstractVehicle::flightTime() const
{
    if (d->flightTime && d->flightTime->isValid()) {
        return d->flightTime->elapsed() / 1000;
    }

    return 0;
}

Kirogi::AbstractVehicle::PerformanceMode AbstractVehicle::performanceMode() const
{
    return CustomPerformance;
}

void AbstractVehicle::requestPerformanceMode(Kirogi::AbstractVehicle::PerformanceMode mode)
{
    Q_UNUSED(mode)
}

float AbstractVehicle::maxRollSpeed() const
{
    return 0.0;
}

void AbstractVehicle::requestMaxRollSpeed(float speed)
{
    Q_UNUSED(speed)
}

float AbstractVehicle::maxRollSpeedMin() const
{
    return 0.0;
}

float AbstractVehicle::maxRollSpeedMax() const
{
    return 0.0;
}

float AbstractVehicle::maxPitchSpeed() const
{
    return 0.0;
}

void AbstractVehicle::requestMaxPitchSpeed(float speed)
{
    Q_UNUSED(speed)
}

float AbstractVehicle::maxPitchSpeedMin() const
{
    return 0.0;
}

float AbstractVehicle::maxPitchSpeedMax() const
{
    return 0.0;
}

float AbstractVehicle::maxYawSpeed() const
{
    return 0.0;
}

void AbstractVehicle::requestMaxYawSpeed(float speed)
{
    Q_UNUSED(speed)
}

float AbstractVehicle::maxYawSpeedMin() const
{
    return 0.0;
}

float AbstractVehicle::maxYawSpeedMax() const
{
    return 0.0;
}

float AbstractVehicle::maxGazSpeed() const
{
    return 0.0;
}

void AbstractVehicle::requestMaxGazSpeed(float speed)
{
    Q_UNUSED(speed)
}

float AbstractVehicle::maxGazSpeedMin() const
{
    return 0.0;
}

float AbstractVehicle::maxGazSpeedMax() const
{
    return 0.0;
}

float AbstractVehicle::maxTilt() const
{
    return 0.0;
}

void AbstractVehicle::requestMaxTilt(float tilt)
{
    Q_UNUSED(tilt)
}

float AbstractVehicle::maxTiltMin() const
{
    return 0.0;
}

float AbstractVehicle::maxTiltMax() const
{
    return 0.0;
}

bool AbstractVehicle::bankedTurns() const
{
    return false;
}

void AbstractVehicle::requestEnableBankedTurns(bool enable)
{
    Q_UNUSED(enable)
}

bool AbstractVehicle::geofence() const
{
    return false;
}

void AbstractVehicle::requestEnableGeofence(bool enable)
{
    Q_UNUSED(enable)
}

float AbstractVehicle::maxAltitude() const
{
    return 0.0;
}

void AbstractVehicle::requestMaxAltitude(float altitude)
{
    Q_UNUSED(altitude)
}

float AbstractVehicle::maxAltitudeMin() const
{
    return 0.0;
}

float AbstractVehicle::maxAltitudeMax() const
{
    return 0.0;
}

float AbstractVehicle::maxDistance() const
{
    return 0.0;
}

void AbstractVehicle::requestMaxDistance(float distance)
{
    Q_UNUSED(distance)
}

float AbstractVehicle::maxDistanceMin() const
{
    return 0.0;
}

float AbstractVehicle::maxDistanceMax() const
{
    return 0.0;
}

void AbstractVehicle::requestAction(Kirogi::AbstractVehicle::VehicleAction action)
{
    if (!ready()) {
        qCWarning(KIROGI_CORE) << name() << "Requested an action while not ready, aborting. Current connection state:" << connectionState();
        return;
    }

    qCDebug(KIROGI_CORE) << name() << "No implementation for action:" << action;
}

void AbstractVehicle::requestTakeOff()
{
    if (flying()) {
        qCWarning(KIROGI_CORE) << name() << "Requested to take off while flying, aborting. Current flying state:" << flyingState();
        return;
    }

    requestAction(TakeOff);
}

void AbstractVehicle::requestLand()
{
    if (!flying()) {
        qCWarning(KIROGI_CORE) << name() << "Requested to land while not flying, aborting. Current flying state:" << flyingState();
        return;
    }

    requestAction(Land);
}

void AbstractVehicle::pilot(qint8 roll, qint8 pitch, qint8 yaw, qint8 gaz)
{
    Q_UNUSED(roll)
    Q_UNUSED(pitch)
    Q_UNUSED(yaw)
    Q_UNUSED(gaz)
}

void AbstractVehicle::requestMoveTo(QGeoCoordinate destination)
{
    Q_UNUSED(destination)
}

float AbstractVehicle::roll() const
{
    return 0.0;
}

float AbstractVehicle::pitch() const
{
    return 0.0;
}

float AbstractVehicle::yaw() const
{
    return 0.0;
}

int AbstractVehicle::signalStrength() const
{
    return -1;
}

int AbstractVehicle::batteryLevel() const
{
    return -1;
}

bool AbstractVehicle::gpsSupported() const
{
    return false;
}

bool AbstractVehicle::gpsFix() const
{
    return false;
}

QGeoCoordinate AbstractVehicle::gpsPosition() const
{
    return QGeoCoordinate();
}

float AbstractVehicle::altitude() const
{
    return 0.0;
}

float AbstractVehicle::distance() const
{
    return -1.0;
}

void AbstractVehicle::setControllerGpsPosition(const QGeoCoordinate &position)
{
    Q_UNUSED(position)
}

void AbstractVehicle::requestReturnHome()
{
}

float AbstractVehicle::speed() const
{
    return 0.0;
}

QString AbstractVehicle::videoSource() const
{
    return QString();
}

bool AbstractVehicle::videoStreamEnabled() const
{
    return false;
}

void AbstractVehicle::requestEnableVideoStream(bool enable)
{
    Q_UNUSED(enable)

    qCWarning(KIROGI_CORE) << name() << "Enabling video streaming is not supported.";
}

bool AbstractVehicle::videoStabilization() const
{
    return false;
}

void AbstractVehicle::requestEnableVideoStabilization(bool enable)
{
    Q_UNUSED(enable)
}

bool AbstractVehicle::canTakePicture() const
{
    return false;
}

bool AbstractVehicle::isRecordingVideo() const
{
    return false;
}

quint16 AbstractVehicle::numberOfFlights() const
{
    return 0;
}

quint16 AbstractVehicle::lastFlightDuration() const
{
    return 0;
}

ParameterModel *AbstractVehicle::parameters()
{
    return &d->parameters;
}

}
