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

#include "mavlinkconnection.h"

MAVLinkConnection::MAVLinkConnection(uint8_t channel, QObject *parent)
    : Kirogi::AbstractConnection(parent)
    , m_channel(channel)
{
    m_gcsHeartbeatTimer.setInterval(1000);
    QObject::connect(&m_gcsHeartbeatTimer, &QTimer::timeout, this, &MAVLinkConnection::sendHeartbeat);
    m_gcsHeartbeatTimer.start();
}

MAVLinkConnection::~MAVLinkConnection()
{
}

void MAVLinkConnection::parseData(const QByteArray &bytes)
{
    mavlink_status_t *status = mavlink_get_channel_status(m_channel);

    for (const auto &byte : bytes) {
        if (!mavlink_parse_char(m_channel, byte, &m_message, status)) {
            continue;
        }
        if (m_message.sysid == MAVLinkPluginConfig::instance().sysid()) {
            continue;
        }
        if (state() < State::Connected) {
            setState(State::Connected);
        }
        emit messageReceived(this, m_message);
    }
}

void MAVLinkConnection::sendHeartbeat()
{
    mavlink_message_t message;
    mavlink_msg_heartbeat_pack_chan(MAVLinkPluginConfig::instance().sysid(),
                                    MAVLinkPluginConfig::instance().compid(),
                                    m_channel,
                                    &message,
                                    MAV_TYPE_GCS,
                                    MAV_AUTOPILOT_INVALID,
                                    MAV_MODE_MANUAL_ARMED,
                                    0,
                                    MAV_STATE_ACTIVE);
    sendMessage(message);
}
