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

#ifndef RYZETELLOVEHICLE_H
#define RYZETELLOVEHICLE_H

#include "abstractvehicle.h"

#include <QThread>

class RyzeTelloConnection;

class QTimer;

class RyzeTelloVehicle : public Kirogi::AbstractVehicle
{
    Q_OBJECT

public:
    explicit RyzeTelloVehicle(QObject *parent = nullptr);
    ~RyzeTelloVehicle() override;

    QString name() const override;
    QString iconName() const override;

    Kirogi::AbstractVehicle::VehicleType vehicleType() const override;

    QList<Kirogi::AbstractVehicle::VehicleAction> supportedActions() const override;

    Q_INVOKABLE void requestAction(Kirogi::AbstractVehicle::VehicleAction action) override;

    Q_INVOKABLE void pilot(qint8 roll, qint8 pitch, qint8 yaw, qint8 gaz) override;

    Kirogi::AbstractVehicle::PerformanceMode performanceMode() const override;
    void requestPerformanceMode(Kirogi::AbstractVehicle::PerformanceMode mode) override;

    float roll() const override;
    float pitch() const override;
    float yaw() const override;

    int signalStrength() const override;
    int batteryLevel() const override;

    bool gpsSupported() const override;
    float distance() const override;
    float altitude() const override;

    float speed() const override;

public Q_SLOTS:
    void connectToVehicle();

private Q_SLOTS:
    void processIncomingResponse(const QString &response);
    void processIncomingState(const QByteArray &state);
    void pollSignalStrength();

private:
    void initVehicle();
    void sendCommand(const QString &command, bool retryForever = false);

    int m_stateSeq;

    int m_motorOnTime;
    int m_oldMotorOnTime;

    float m_roll;
    float m_pitch;
    float m_yaw;

    int m_signalStrength;
    int m_batteryLevel;

    float m_distance;
    float m_altitude;

    float m_speed;

    QTimer *m_signalStrengthTimer;
    QTimer *m_disconnectTimer;

    QThread m_connectionThread;
    RyzeTelloConnection *m_connection;
};

#endif
