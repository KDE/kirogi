/*
 * Copyright 2020 Kitae Kim <develoot@gmail.com>
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

#include "mavlinkudpconnection.h"

#include <QNetworkDatagram>
#include <QNetworkProxy>
#include <QString>

#include "mavlink_connection_debug.h"

MAVLinkUdpConnection::MAVLinkUdpConnection(Kirogi::UdpConfiguration configuration, uint8_t channel, QObject *parent)
    : MAVLinkConnection(channel, parent)
    , m_configuration(configuration)
{
}

MAVLinkUdpConnection::~MAVLinkUdpConnection()
{
    disconnect();
}

Kirogi::ConnectionConfiguration *MAVLinkUdpConnection::configuration()
{
    return &m_configuration;
}

bool MAVLinkUdpConnection::connect()
{
    if (state() > State::Disconnected) {
        qCDebug(KIROGI_VEHICLESUPPORT_MAVLINK_CONNECTION) << QString("Tried to create UDP connection %1 when it's not disconnected.").arg(m_configuration.name());
        return false;
    }

    qCDebug(KIROGI_VEHICLESUPPORT_MAVLINK_CONNECTION) << QString("Connecting connection %1.").arg(m_configuration.name());

    m_socket = QSharedPointer<QUdpSocket>(new QUdpSocket(this));
    m_socket->setProxy(QNetworkProxy::NoProxy);
    if (!m_socket->bind(QHostAddress::AnyIPv4, m_configuration.port(), QAbstractSocket::ReuseAddressHint | QUdpSocket::ShareAddress)) {
        qCDebug(KIROGI_VEHICLESUPPORT_MAVLINK_CONNECTION) << QString("Failed to bind connection %1 to address.").arg(m_configuration.name());
        m_socket->deleteLater();
        return false;
    }
    // The 'All Hosts' multicast group addresses all hosts on the same network segment.
    m_socket->joinMulticastGroup(QHostAddress("224.0.0.1"));

    QObject::connect(m_socket.data(), &QUdpSocket::readyRead, this, &MAVLinkUdpConnection::processDataOnSocket);
    setState(State::Connected);

    return true;
}

bool MAVLinkUdpConnection::disconnect()
{
    if (state() < State::Connected) {
        qCDebug(KIROGI_VEHICLESUPPORT_MAVLINK_CONNECTION) << QString("Tried to disconnect connection %1 when it's not connected.").arg(m_configuration.name());
        return false;
    }

    qCDebug(KIROGI_VEHICLESUPPORT_MAVLINK_CONNECTION) << QString("Disconnecting connection %1.").arg(m_configuration.name());

    m_socket->deleteLater();
    m_socket = nullptr;

    setState(State::Disconnected);

    return true;
}

void MAVLinkUdpConnection::processDataOnSocket()
{
    while (m_socket->hasPendingDatagrams()) {
        const QNetworkDatagram &datagram = m_socket->receiveDatagram();
        auto checkOverlapped = [datagram](auto &target) { return datagram.senderAddress() == target.address && datagram.senderPort() == target.port; };

        if (std::find_if(m_targets.begin(), m_targets.end(), checkOverlapped) == m_targets.end()) {
            m_targets.append({datagram.senderAddress(), datagram.senderPort()});
        }

        parseData(datagram.data());
    }
}

void MAVLinkUdpConnection::sendBytes(const QByteArray &bytes)
{
    if (state() < State::Connected) {
        qCDebug(KIROGI_VEHICLESUPPORT_MAVLINK_CONNECTION) << QString("Tried to send bytes on connection %1 when it's not connected.").arg(m_configuration.name());
        return;
    }

    for (const auto &target : m_targets) {
        if (m_socket->writeDatagram(bytes, target.address, target.port) < 0) {
            qCDebug(KIROGI_VEHICLESUPPORT_MAVLINK_CONNECTION) << "Error occured while writing data to " << target.address << ":" << target.port;
        }
    }
}
