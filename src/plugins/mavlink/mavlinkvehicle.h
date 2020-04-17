/*
 * Copyright 2019 Patrick José Pereira <patrickjp@kde.org>
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

#include "abstractvehicle.h"
#include "mavlinkconnection.h"

#include <QThread>
#include <QTimer>

class MAVLinkConnection;
class QTimer;

class MAVLinkVehicle : public Kirogi::AbstractVehicle
{
    Q_OBJECT

public:
    explicit MAVLinkVehicle(QObject *parent = nullptr);
    ~MAVLinkVehicle() override;

    QString name() const override;
    QString iconName() const override;

    Kirogi::AbstractVehicle::VehicleType vehicleType() const override;

    QList<Kirogi::AbstractVehicle::VehicleAction> supportedActions() const override;

    Q_INVOKABLE void requestAction(Kirogi::AbstractVehicle::VehicleAction action) override;
    Q_INVOKABLE void pilot(qint8 roll, qint8 pitch, qint8 yaw, qint8 gaz) override;

    float roll() const override;
    float pitch() const override;
    float yaw() const override;

    int signalStrength() const override;
    int batteryLevel() const override;

    bool gpsSupported() const override;
    bool gpsFix() const override;
    QGeoCoordinate gpsPosition() const override;
    float altitude() const override;

    QString videoSource() const override;

public Q_SLOTS:
    void connectToVehicle();

private Q_SLOTS:
    void fetchParameters() const;
    void processMavlinkMessage(const mavlink_message_t &message);

private:
    float m_roll;
    float m_pitch;
    float m_yaw;

    int m_signalStrength;
    int m_batteryLevel;

    bool m_gpsFix;
    QGeoCoordinate m_gpsPosition;

    struct {
        bool altitudeMessage {false};
        float altitude {0.0f};
    } m_altitudeSource;

    QThread m_connectionThread;
    MAVLinkConnection *m_connection;

    typedef struct {
        bool command_int;
        float params[7];
        MAV_CMD command;
        uint8_t target_system;
        uint8_t target_component;
        uint8_t confirmation;
        MAV_FRAME frame; // Coordinate system of the command. (COMMAND_INT)
        uint8_t current; // False: 0, True: 1.
        uint8_t autocontinue; // Continue to next wp automatically.
    } CommandQueueEntry_t;

    QList<CommandQueueEntry_t> m_commandQueue;
    QTimer m_commandTimer;
    int m_cmdResendCnt;

    void sendCommandInQueue();
};
