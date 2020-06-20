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

#pragma once

#include <QGeoCoordinate>
#include <QObject>
#include <QSharedPointer>

#include "kirogicore_export.h"

class ParameterModel;

namespace Kirogi
{
class KIROGI_EXPORT AbstractVehicle : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ id CONSTANT)

    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString iconName READ iconName NOTIFY iconNameChanged)

    Q_PROPERTY(VehicleType vehicleType READ vehicleType CONSTANT)

    Q_PROPERTY(ConnectionState connectionState READ connectionState NOTIFY connectionStateChanged)
    Q_PROPERTY(FlyingState flyingState READ flyingState NOTIFY flyingStateChanged)
    Q_PROPERTY(bool connected READ connected NOTIFY connectionStateChanged)
    Q_PROPERTY(bool ready READ ready NOTIFY connectionStateChanged)
    Q_PROPERTY(bool flying READ flying NOTIFY flyingStateChanged)

    Q_PROPERTY(QList<VehicleAction> supportedActions READ supportedActions CONSTANT)

    Q_PROPERTY(bool piloting READ piloting WRITE setPiloting NOTIFY pilotingChanged)

    Q_PROPERTY(int flightTime READ flightTime NOTIFY flightTimeChanged)

    Q_PROPERTY(PerformanceMode performanceMode READ performanceMode NOTIFY performanceModeChanged)

    Q_PROPERTY(float maxRollSpeed READ maxRollSpeed NOTIFY maxRollSpeedChanged)
    Q_PROPERTY(float maxRollSpeedMin READ maxRollSpeedMin NOTIFY maxRollSpeedMinChanged)
    Q_PROPERTY(float maxRollSpeedMax READ maxRollSpeedMax NOTIFY maxRollSpeedMaxChanged)

    Q_PROPERTY(float maxPitchSpeed READ maxPitchSpeed NOTIFY maxPitchSpeedChanged)
    Q_PROPERTY(float maxPitchSpeedMin READ maxPitchSpeedMin NOTIFY maxPitchSpeedMinChanged)
    Q_PROPERTY(float maxPitchSpeedMax READ maxPitchSpeedMax NOTIFY maxPitchSpeedMaxChanged)

    Q_PROPERTY(float maxYawSpeed READ maxYawSpeed NOTIFY maxYawSpeedChanged)
    Q_PROPERTY(float maxYawSpeedMin READ maxYawSpeedMin NOTIFY maxYawSpeedMinChanged)
    Q_PROPERTY(float maxYawSpeedMax READ maxYawSpeedMax NOTIFY maxYawSpeedMaxChanged)

    Q_PROPERTY(float maxGazSpeed READ maxGazSpeed NOTIFY maxGazSpeedChanged)
    Q_PROPERTY(float maxGazSpeedMin READ maxGazSpeedMin NOTIFY maxGazSpeedMinChanged)
    Q_PROPERTY(float maxGazSpeedMax READ maxGazSpeedMax NOTIFY maxGazSpeedMaxChanged)

    Q_PROPERTY(float maxTilt READ maxTilt NOTIFY maxTiltChanged)
    Q_PROPERTY(float maxTiltMin READ maxTiltMin NOTIFY maxTiltMinChanged)
    Q_PROPERTY(float maxTiltMax READ maxTiltMax NOTIFY maxTiltMaxChanged)

    Q_PROPERTY(bool bankedTurns READ bankedTurns NOTIFY bankedTurnsChanged)

    Q_PROPERTY(bool geofence READ geofence NOTIFY geofenceChanged)

    Q_PROPERTY(float maxAltitude READ maxAltitude NOTIFY maxAltitudeChanged)
    Q_PROPERTY(float maxAltitudeMin READ maxAltitudeMin NOTIFY maxAltitudeMinChanged)
    Q_PROPERTY(float maxAltitudeMax READ maxAltitudeMax NOTIFY maxAltitudeMaxChanged)

    Q_PROPERTY(float maxDistance READ maxDistance NOTIFY maxDistanceChanged)
    Q_PROPERTY(float maxDistanceMin READ maxDistanceMin NOTIFY maxDistanceMinChanged)
    Q_PROPERTY(float maxDistanceMax READ maxDistanceMax NOTIFY maxDistanceMaxChanged)

    Q_PROPERTY(QGeoCoordinate gpsPosition READ gpsPosition NOTIFY gpsPositionChanged)
    Q_PROPERTY(float distance READ distance NOTIFY distanceChanged)
    Q_PROPERTY(float altitude READ altitude NOTIFY altitudeChanged)

    Q_PROPERTY(int speed READ speed NOTIFY speedChanged)

    Q_PROPERTY(float roll READ roll NOTIFY attitudeChanged)
    Q_PROPERTY(float pitch READ pitch NOTIFY attitudeChanged)
    Q_PROPERTY(float yaw READ yaw NOTIFY attitudeChanged)

    Q_PROPERTY(int signalStrength READ signalStrength NOTIFY signalStrengthChanged)
    Q_PROPERTY(int batteryLevel READ batteryLevel NOTIFY batteryLevelChanged)
    Q_PROPERTY(bool gpsSupported READ gpsSupported NOTIFY gpsSupportedChanged)
    Q_PROPERTY(bool gpsFix READ gpsFix NOTIFY gpsFixChanged)

    Q_PROPERTY(QString videoSource READ videoSource NOTIFY videoSourceChanged)
    Q_PROPERTY(bool videoStreamEnabled READ videoStreamEnabled NOTIFY videoStreamEnabledChanged)
    Q_PROPERTY(bool videoStabilization READ videoStabilization NOTIFY videoStabilizationChanged)
    Q_PROPERTY(bool canTakePicture READ canTakePicture NOTIFY canTakePictureChanged)
    Q_PROPERTY(bool isRecordingVideo READ isRecordingVideo NOTIFY isRecordingVideoChanged)

    Q_PROPERTY(quint16 numberOfFlights READ numberOfFlights NOTIFY numberOfFlightsChanged)
    Q_PROPERTY(quint16 lastFlightDuration READ lastFlightDuration NOTIFY lastFlightDurationChanged)

    Q_PROPERTY(ParameterModel *parameters READ parameters NOTIFY parametersChanged)

public:
    enum VehicleType { UnknownVehicleType = 0, QuadCopter = 1 };
    Q_ENUM(VehicleType)

    enum ConnectionState { Disconnected = 0, Connecting = 1, Connected = 2, Ready = 3 };
    Q_ENUM(ConnectionState)

    enum FlyingState {
        Unknown = 0,
        Landed = 1,
        TakingOff = 2,
        Hovering = 3,
        Flying = 4,
        Landing = 5,
        // FIXME: (Parrot) Not handled yet.
        //             Emergency = 5,
        //             UserTakeoff = 6,
        //             MotorRamping = 7,
        //             EmergencyLanding = 8
    };
    Q_ENUM(FlyingState)

    enum VehicleAction {
        TakeOff = 1,
        Land = 2,
        MoveTo = 3,
        FlatTrim = 4,
        FlipForward = 5,
        FlipBackward = 6,
        FlipLeft = 7,
        FlipRight = 8,
        SwitchPerformanceMode = 9,
        SetMaxRollSpeed = 10,
        SetMaxPitchSpeed = 11,
        SetMaxYawSpeed = 12,
        SetMaxGazSpeed = 13,
        SetMaxTilt = 14,
        ToggleBankedTurns = 15,
        ToggleGeofence = 16,
        SetMaxAltitude = 17,
        SetMaxDistance = 18,
        ToggleVideoStream = 19,
        ToggleVideoStabilization = 20,
        TakePicture = 21,
        RecordVideo = 22
    };
    Q_ENUM(VehicleAction)

    enum PerformanceMode { CustomPerformance = 0, FilmPerformance = 1, SportPerformance = 2 };
    Q_ENUM(PerformanceMode)

    explicit AbstractVehicle(QObject *parent = nullptr);
    virtual ~AbstractVehicle() override;

    int id() const;

    virtual QString name() const = 0;
    virtual QString iconName() const;

    virtual VehicleType vehicleType() const;

    virtual ConnectionState connectionState() const;
    virtual FlyingState flyingState() const;
    bool connected() const;
    bool ready() const;
    bool flying() const;

    virtual QList<VehicleAction> supportedActions() const;
    Q_INVOKABLE virtual bool isActionSupported(VehicleAction action) const;

    virtual bool piloting() const;
    Q_INVOKABLE virtual void setPiloting(bool piloting);

    Q_INVOKABLE virtual void requestAction(VehicleAction action);
    Q_INVOKABLE virtual void requestTakeOff();
    Q_INVOKABLE virtual void requestLand();

    Q_INVOKABLE virtual void pilot(qint8 roll, qint8 pitch, qint8 yaw, qint8 gaz);
    Q_INVOKABLE virtual void requestMoveTo(QGeoCoordinate destination);

    virtual int flightTime() const;

    virtual PerformanceMode performanceMode() const;
    Q_INVOKABLE virtual void requestPerformanceMode(PerformanceMode mode);

    virtual float maxRollSpeed() const;
    Q_INVOKABLE virtual void requestMaxRollSpeed(float speed);
    virtual float maxRollSpeedMin() const;
    virtual float maxRollSpeedMax() const;

    virtual float maxPitchSpeed() const;
    Q_INVOKABLE virtual void requestMaxPitchSpeed(float speed);
    virtual float maxPitchSpeedMin() const;
    virtual float maxPitchSpeedMax() const;

    virtual float maxYawSpeed() const;
    Q_INVOKABLE virtual void requestMaxYawSpeed(float speed);
    virtual float maxYawSpeedMin() const;
    virtual float maxYawSpeedMax() const;

    virtual float maxGazSpeed() const;
    Q_INVOKABLE virtual void requestMaxGazSpeed(float speed);
    virtual float maxGazSpeedMin() const;
    virtual float maxGazSpeedMax() const;

    virtual float maxTilt() const;
    Q_INVOKABLE virtual void requestMaxTilt(float tilt);
    virtual float maxTiltMin() const;
    virtual float maxTiltMax() const;

    virtual bool bankedTurns() const;
    Q_INVOKABLE virtual void requestEnableBankedTurns(bool enable);

    virtual bool geofence() const;
    Q_INVOKABLE virtual void requestEnableGeofence(bool enable);

    virtual float maxAltitude() const;
    Q_INVOKABLE virtual void requestMaxAltitude(float altitude);
    virtual float maxAltitudeMin() const;
    virtual float maxAltitudeMax() const;

    virtual float maxDistance() const;
    Q_INVOKABLE virtual void requestMaxDistance(float distance);
    virtual float maxDistanceMin() const;
    virtual float maxDistanceMax() const;

    virtual float roll() const;
    virtual float pitch() const;
    virtual float yaw() const;

    virtual int signalStrength() const;
    virtual int batteryLevel() const;

    virtual bool gpsSupported() const;
    virtual bool gpsFix() const;
    virtual QGeoCoordinate gpsPosition() const;
    virtual float distance() const;
    virtual float altitude() const;
    Q_INVOKABLE virtual void setControllerGpsPosition(const QGeoCoordinate &position);
    Q_INVOKABLE virtual void requestReturnHome();

    virtual float speed() const;

    virtual QString videoSource() const;
    virtual bool videoStreamEnabled() const;
    Q_INVOKABLE virtual void requestEnableVideoStream(bool enable);
    virtual bool videoStabilization() const;
    Q_INVOKABLE virtual void requestEnableVideoStabilization(bool enable);
    virtual bool canTakePicture() const;
    virtual bool isRecordingVideo() const;

    virtual quint16 numberOfFlights() const;
    virtual quint16 lastFlightDuration() const;

    ParameterModel *parameters();

Q_SIGNALS:
    void nameChanged();
    void iconNameChanged();

    void connectionStateChanged();
    void flyingStateChanged();

    void pilotingChanged();

    void flightTimeChanged();

    void performanceModeChanged();

    void maxRollSpeedChanged();
    void maxRollSpeedMinChanged();
    void maxRollSpeedMaxChanged();

    void maxPitchSpeedChanged();
    void maxPitchSpeedMinChanged();
    void maxPitchSpeedMaxChanged();

    void maxYawSpeedChanged();
    void maxYawSpeedMinChanged();
    void maxYawSpeedMaxChanged();

    void maxGazSpeedChanged();
    void maxGazSpeedMinChanged();
    void maxGazSpeedMaxChanged();

    void maxTiltChanged();
    void maxTiltMinChanged();
    void maxTiltMaxChanged();

    void bankedTurnsChanged();

    void geofenceChanged();

    void maxAltitudeChanged();
    void maxAltitudeMinChanged();
    void maxAltitudeMaxChanged();

    void maxDistanceChanged();
    void maxDistanceMinChanged();
    void maxDistanceMaxChanged();

    void attitudeChanged();
    void rollChanged();
    void pitchChanged();
    void yawChanged();

    void signalStrengthChanged();
    void batteryLevelChanged();

    void gpsSupportedChanged();
    void gpsFixChanged();
    void gpsPositionChanged();
    void distanceChanged();
    void altitudeChanged();

    void speedChanged();

    void videoSourceChanged();
    void videoStreamEnabledChanged();
    void videoStabilizationChanged();
    void canTakePictureChanged();
    void isRecordingVideoChanged();

    void numberOfFlightsChanged();
    void lastFlightDurationChanged();

    void parametersChanged();

protected Q_SLOTS:
    virtual void setConnectionState(ConnectionState state);
    virtual void setFlyingState(FlyingState state);

private:
    class Private;
    QScopedPointer<Private> d;
};

}
