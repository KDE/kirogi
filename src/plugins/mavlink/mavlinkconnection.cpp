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

#include <QNetworkDatagram>
#include <QNetworkProxy>

MAVLinkConnection::MAVLinkConnection(const QString &vehicleName, QObject *parent)
    : QObject(parent)
    , m_vehicleName(vehicleName)
    , m_connectionHost({QHostAddress("0.0.0.0"), 14550})
{
    qRegisterMetaType<mavlink_message_t>("mavlink_message_t");

    // Send periodically heartbeats to say that GCS is alive
    connect(&heartbeatTimer, &QTimer::timeout, this, [this] { sendByteArray(_heartbeatMessage); });
    heartbeatTimer.start(1000);
}

void MAVLinkConnection::sendByteArray(const QByteArray &byteArray) const
{
    if (m_controlSocket) {
        for (const auto &sender : m_senders) {
            m_controlSocket->writeDatagram(byteArray, sender.address, sender.port);
        }
    }
}

void MAVLinkConnection::sendMessage(const mavlink_message_t &message) const
{
    const int length = mavlink_msg_to_send_buffer(m_buffer, &message);
    sendByteArray({reinterpret_cast<const char *>(m_buffer), length});
}

void MAVLinkConnection::handshake()
{
    initSockets();
}

void MAVLinkConnection::reset()
{
    if (m_controlSocket) {
        m_controlSocket->abort();
        delete m_controlSocket;
    }

    emit stateChanged(Kirogi::AbstractVehicle::Disconnected);
}

void MAVLinkConnection::receiveData()
{
    static mavlink_message_t message;
    mavlink_status_t status;
    while (m_controlSocket->hasPendingDatagrams()) {
        const QNetworkDatagram &datagram = m_controlSocket->receiveDatagram();
        m_senders.append({datagram.senderAddress(), datagram.senderPort()});

        if (datagram.isValid()) {
            const QByteArray &data = datagram.data();
            for (const auto &byte : data) {
                if (mavlink_parse_char(0, byte, &message, &status)) {
                    emit mavlinkMessage(message);
                }
            }
        }
    }

    m_senders.erase(std::unique(m_senders.begin(), m_senders.end(), [](const auto first, const auto second) { return (first.address == second.address && first.port == second.port); }), m_senders.end());
}

void MAVLinkConnection::initSockets()
{
    m_controlSocket = new QUdpSocket(this);
    QObject::connect(m_controlSocket, &QUdpSocket::readyRead, this, &MAVLinkConnection::receiveData);

    m_controlSocket->setProxy(QNetworkProxy::NoProxy);
    const bool bindState = m_controlSocket->bind(QHostAddress::AnyIPv4, m_connectionHost.port, QAbstractSocket::ReuseAddressHint | QUdpSocket::ShareAddress);

    if (bindState) {
        // The All Hosts multicast group addresses all hosts on the same network
        // segment.
        m_controlSocket->joinMulticastGroup(QHostAddress("224.0.0.1"));
    }

    emit stateChanged(Kirogi::AbstractVehicle::Connecting);
}
