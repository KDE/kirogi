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

#ifndef PARROTVEHICLE_H
#define PARROTVEHICLE_H

#include "abstractvehicle.h"

#include "parrotprotocol.h"

#include <QThread>

class ParrotConnection;

class ParrotVehicle : public Kirogi::AbstractVehicle
{
    Q_OBJECT

    public:
        enum Type {
            Bebop2 = 0,
            Anafi
        };
        Q_ENUM(Type)

        explicit ParrotVehicle(Type type, const QString &hostName,
            int port, const QString &productSerial, QObject *parent = nullptr);
        ~ParrotVehicle() override;

        QString name() const override;
        QString iconName() const override;

        Kirogi::AbstractVehicle::VehicleType vehicleType() const override;

        QList<Kirogi::AbstractVehicle::VehicleAction> supportedActions() const override;

        bool piloting() const override;
        Q_INVOKABLE void setPiloting(bool piloting) override;

        Q_INVOKABLE void requestAction(Kirogi::AbstractVehicle::VehicleAction action) override;

        Q_INVOKABLE void pilot(qint8 roll, qint8 pitch, qint8 yaw, qint8 gaz) override;
        Q_INVOKABLE void requestMoveTo(QGeoCoordinate destination) override;

        Kirogi::AbstractVehicle::PerformanceMode performanceMode() const override;
        void requestPerformanceMode(Kirogi::AbstractVehicle::PerformanceMode mode) override;

        float maxRollSpeed() const override;
        void requestMaxRollSpeed(float speed) override;
        float maxRollSpeedMin() const override;
        float maxRollSpeedMax() const override;

        float maxPitchSpeed() const override;
        void requestMaxPitchSpeed(float speed) override;
        float maxPitchSpeedMin() const override;
        float maxPitchSpeedMax() const override;

        float maxYawSpeed() const override;
        void requestMaxYawSpeed(float speed) override;
        float maxYawSpeedMin() const override;
        float maxYawSpeedMax() const override;

        float maxGazSpeed() const override;
        void requestMaxGazSpeed(float speed) override;
        float maxGazSpeedMin() const override;
        float maxGazSpeedMax() const override;

        float maxTilt() const override;
        void requestMaxTilt(float tilt) override;
        float maxTiltMin() const override;
        float maxTiltMax() const override;

        bool bankedTurns() const override;
        void requestEnableBankedTurns(bool bankedTurns) override;

        bool geofence() const override;
        void requestEnableGeofence(bool enable) override;

        float maxAltitude() const override;
        void requestMaxAltitude(float altitude) override;
        float maxAltitudeMin() const override;
        float maxAltitudeMax() const override;

        float maxDistance() const override;
        void requestMaxDistance(float distance) override;
        float maxDistanceMin() const override;
        float maxDistanceMax() const override;

        float roll() const override;
        float pitch() const override;
        float yaw() const override;

        int signalStrength() const override;
        int batteryLevel() const override;

        bool gpsSupported() const override;
        bool gpsFix() const override;
        QGeoCoordinate gpsPosition() const override;
        float altitude() const override;
        void setControllerGpsPosition(const QGeoCoordinate &position) override;
        void requestReturnHome() override;

        float speed() const override;

        quint16 numberOfFlights() const override;
        quint16 lastFlightDuration() const override;

        QString videoSource() const override;
        bool videoStreamEnabled() const override;
        Q_INVOKABLE void requestEnableVideoStream(bool enable) override;
        bool videoStabilization() const override;
        void requestEnableVideoStabilization(bool enable) override;
        bool canTakePicture() const override;
        bool isRecordingVideo() const override;

    public Q_SLOTS:
        void connectToVehicle();

    private Q_SLOTS:
        void processIncomingCommand(const ParrotCommand &command);

    private:
        void initVehicle();
        void sendCommand(Parrot::Command command, const QVariantList &arguments = QVariantList(),
            bool retryForever = false);

        Type m_type;

        QString m_hostName;
        QString m_productSerial;

        int m_initialized;

        bool m_piloting;

        float m_maxRollPitchSpeed;
        float m_maxRollPitchSpeedMin;
        float m_maxRollPitchSpeedMax;

        float m_maxYawSpeed;
        float m_maxYawSpeedMin;
        float m_maxYawSpeedMax;

        float m_maxGazSpeed;
        float m_maxGazSpeedMin;
        float m_maxGazSpeedMax;

        float m_maxTilt;
        float m_maxTiltMin;
        float m_maxTiltMax;

        bool m_bankedTurns;

        bool m_geofence;

        float m_maxAltitude;
        float m_maxAltitudeMin;
        float m_maxAltitudeMax;

        float m_maxDistance;
        float m_maxDistanceMin;
        float m_maxDistanceMax;

        float m_roll;
        float m_pitch;
        float m_yaw;

        float m_defaultCameraOrientationTilt;
        float m_defaultCameraOrientationPan;

        qint16 m_signalStrength;
        int m_batteryLevel;

        bool m_gpsFix;
        QGeoCoordinate m_gpsPosition;
        float m_altitude;

        float m_speed;

        bool m_videoStreamEnabled;
        qint32 m_videoStreamMode;
        qint32 m_videoStabilizationMode;
        bool m_canTakePicture;
        bool m_isRecordingVideo;

        quint16 m_numberOfFlights;
        quint16 m_lastFlightDuration;

        QThread m_connectionThread;
        ParrotConnection *m_connection;
};

#endif
