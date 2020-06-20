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

#include <memory>

#include <QObject>

#include "abstractvehicle.h"

#include <QHostAddress>
#include <QPointer>
#include <QQueue>
#include <QTimer>
#include <QUdpSocket>

struct RyzeTelloCommand {
    QString text;
    int retry = 0; // NOTE: Setting to -1 before sending means infinite retries.
};

class RyzeTelloConnection : public QObject
{
    Q_OBJECT

public:
    explicit RyzeTelloConnection(const QString &vehicleName, QObject *parent = nullptr);
    ~RyzeTelloConnection();

public Q_SLOTS:
    void handshake();
    void reset();
    void sendCommand(const QString &command, bool retryForever = false);
    void pilot(qint8 roll, qint8 pitch, qint8 yaw, qint8 gaz);

Q_SIGNALS:
    void stateChanged(Kirogi::AbstractVehicle::ConnectionState state);
    void responseReceived(const QString &response);
    void stateReceived(const QByteArray &state);

private Q_SLOTS:
    void receiveData();
    void receiveState();
    void pumpCommandQueue();
    void sendPilotingCommand();

private:
    void initSockets();

    QString m_vehicleName;

    QHostAddress m_address;
    QPointer<QUdpSocket> m_controlSocket;
    QPointer<QUdpSocket> m_stateSocket;

    QQueue<RyzeTelloCommand> m_commandQueue;
    std::unique_ptr<QTimer> m_commandQueueTimer;

    std::unique_ptr<QTimer> m_pilotingTimer;
    qint8 m_roll;
    qint8 m_pitch;
    qint8 m_yaw;
    qint8 m_gaz;
};
