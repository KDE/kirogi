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

#include "abstractconnection.h"

#include "mavlinkpluginconfig.h"

#include <QByteArray>
#include <QObject>
#include <QTimer>

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

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#else
#pragma warning(pop)
#endif

/**
 * A base class of UDP, TCP and Serial type connection of mavlink plugin.
 *
 * This class defines common functionalities of all types of mavlink connections.
 */
class MAVLinkConnection : public Kirogi::AbstractConnection
{
    Q_OBJECT

public:
    MAVLinkConnection(uint8_t channel, QObject *parent = nullptr);
    ~MAVLinkConnection();

    template<typename T>
    constexpr void sendMessage(const T &message)
    {
        if constexpr (std::is_same<T, mavlink_message_t>::value) {
            const int length = mavlink_msg_to_send_buffer(m_buffer, &message);
            sendBytes({reinterpret_cast<const char *>(m_buffer), length});
        } else if constexpr (std::is_same<T, mavlink_command_long_t>::value) {
            mavlink_message_t mavlink_message = {};
            mavlink_msg_command_long_encode(MAVLinkPluginConfig::instance().sysid(), MAV_COMP_ID_MISSIONPLANNER, &mavlink_message, &message);
            sendMessage(mavlink_message);
        } else if constexpr (std::is_same<T, mavlink_manual_control_t>::value) {
            mavlink_message_t mavlink_message = {};
            mavlink_msg_manual_control_encode(MAVLinkPluginConfig::instance().sysid(), MAV_COMP_ID_MISSIONPLANNER, &mavlink_message, &message);
            sendMessage(mavlink_message);
        } else if constexpr (std::is_same<T, mavlink_param_request_list_t>::value) {
            mavlink_message_t mavlink_message = {};
            mavlink_msg_param_request_list_encode(MAVLinkPluginConfig::instance().sysid(), MAV_COMP_ID_MISSIONPLANNER, &mavlink_message, &message);
            sendMessage(mavlink_message);
        }
    }

public Q_SLOTS:
    void parseData(const QByteArray &bytes);
    void sendHeartbeat();

Q_SIGNALS:
    void messageReceived(MAVLinkConnection *connection, const mavlink_message_t &message);

protected:
    uint8_t m_channel; ///< A channel that mavlink_parse_char() uses to manage internal buffers.

private:
    QTimer m_gcsHeartbeatTimer;
    mavlink_message_t m_message; ///< Used for parsing incoming mavlink messages.
    mutable uint8_t m_buffer[1024]; ///< Buffer that sendMessage() uses.
};
