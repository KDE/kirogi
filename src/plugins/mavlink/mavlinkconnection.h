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

#include <QObject>

#include "abstractvehicle.h"

#include <QHostAddress>
#include <QPointer>
#include <QTimer>
#include <QUdpSocket>

// It's necessary to disable some warnings to avoid the amount of noise
// messages from MAVLink source code
#ifdef __GNUC__
#if __GNUC__ > 8
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waddress-of-packed-member"
#pragma GCC diagnostic ignored "-Wpedantic"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#endif
#else
#pragma warning(push, 0)
#endif

#include "mavlink.h"

#ifdef __GNUC__
#pragma GCC diagnostic pop
#else
#pragma warning(pop, 0)
#endif

namespace
{
/**
 * @brief Create a QByteArray with Heartbeat Message content
 *
 * @return QByteArray
 */
QByteArray createHeartbeatMessage()
{
    mavlink_message_t message;
    uint8_t buffer[1024];
    mavlink_msg_heartbeat_pack(255, MAV_COMP_ID_MISSIONPLANNER, &message, MAV_TYPE_GCS, MAV_AUTOPILOT_INVALID, MAV_MODE_FLAG_SAFETY_ARMED, 0, MAV_STATE_ACTIVE);
    auto length = mavlink_msg_to_send_buffer(buffer, &message);
    return QByteArray(reinterpret_cast<const char *>(buffer), length);
}
}

class MAVLinkConnection : public QObject
{
    Q_OBJECT

public:
    explicit MAVLinkConnection(const QString &vehicleName, QObject *parent = nullptr);
    ~MAVLinkConnection() = default;

public Q_SLOTS:
    void handshake();
    void reset();

    void sendByteArray(const QByteArray &byteArray) const;
    void sendMessage(const mavlink_message_t &message) const;

Q_SIGNALS:
    void mavlinkMessage(const mavlink_message_t &message) const;
    void stateChanged(Kirogi::AbstractVehicle::ConnectionState state) const;
    void responseReceived(const QString &response) const;
    void stateReceived(const QByteArray &state) const;

private Q_SLOTS:
    void receiveData();

private:
    void initSockets();

    QString m_vehicleName;

    struct AddressPort {
        QHostAddress address;
        int port;
    };

    AddressPort m_connectionHost;
    QPointer<QUdpSocket> m_controlSocket;
    QVector<AddressPort> m_senders;

    QTimer heartbeatTimer;

    // Used by sendMessage
    mutable uint8_t m_buffer[1024];

    const QByteArray _heartbeatMessage = createHeartbeatMessage();
};
