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

#include <memory>

#include <QObject>

#include "abstractvehicle.h"

#include <QHostAddress>
#include <QPointer>
#include <QTimer>
#include <QUdpSocket>

// It's necessary to disable some warnings to avoid the amount of noise
// messages from MAVLink source code
#if defined(__GNUC__) || defined(__clang__)
#if __GNUC__ > 8 || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waddress-of-packed-member"
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wcast-align"
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

struct MAVLinkHelper {
    static constexpr uint8_t GCS_SYSTEM_ID = 255;
};

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
    mavlink_msg_heartbeat_pack(MAVLinkHelper::GCS_SYSTEM_ID, MAV_COMP_ID_MISSIONPLANNER, &message, MAV_TYPE_GCS, MAV_AUTOPILOT_INVALID, MAV_MODE_FLAG_SAFETY_ARMED, 0, MAV_STATE_ACTIVE);
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

    template<typename T> constexpr void sendMessage(const T &message) const
    {
        if constexpr (std::is_same<T, mavlink_message_t>::value) {
            const int length = mavlink_msg_to_send_buffer(m_buffer, &message);
            sendByteArray({reinterpret_cast<const char *>(m_buffer), length});

        } else if constexpr (std::is_same<T, mavlink_command_long_t>::value) {
            mavlink_message_t mavlink_message = {};
            mavlink_msg_command_long_encode(MAVLinkHelper::GCS_SYSTEM_ID, MAV_COMP_ID_MISSIONPLANNER, &mavlink_message, &message);
            sendMessage(mavlink_message);

        } else if constexpr (std::is_same<T, mavlink_manual_control_t>::value) {
            mavlink_message_t mavlink_message = {};
            mavlink_msg_manual_control_encode(MAVLinkHelper::GCS_SYSTEM_ID, MAV_COMP_ID_MISSIONPLANNER, &mavlink_message, &message);
            sendMessage(mavlink_message);

        } else if constexpr (std::is_same<T, mavlink_param_request_list_t>::value) {
            mavlink_message_t mavlink_message = {};
            mavlink_msg_param_request_list_encode(MAVLinkHelper::GCS_SYSTEM_ID, MAV_COMP_ID_MISSIONPLANNER, &mavlink_message, &message);
            sendMessage(mavlink_message);

        } else {
            static_assert(std::is_same<T, void>::value, "Invalid message type.");
        }
    }

public Q_SLOTS:
    void handshake();
    void reset();

    void sendByteArray(const QByteArray &byteArray);

Q_SIGNALS:
    void mavlinkMessage(const mavlink_message_t &message);
    void stateChanged(Kirogi::AbstractVehicle::ConnectionState state);
    void responseReceived(const QString &response);
    void stateReceived(const QByteArray &state);

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

    /* Allocating heartbeatTimer on the heap so that the QTimer's thread
    affinity doesn't get broken when the destructor is called. */
    std::unique_ptr<QTimer> heartbeatTimer;

    // Used by sendMessage
    mutable uint8_t m_buffer[1024];

    const QByteArray _heartbeatMessage = createHeartbeatMessage();
};
