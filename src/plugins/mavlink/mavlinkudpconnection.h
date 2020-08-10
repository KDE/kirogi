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

#pragma once

#include "udpconfiguration.h"

#include "mavlinkconnection.h"

#include <QHostAddress>
#include <QSharedPointer>
#include <QUdpSocket>

/**
 * An UDP connection implementation of mavlink plugin.
 */
class MAVLinkUdpConnection : public MAVLinkConnection
{
    Q_OBJECT

public:
    MAVLinkUdpConnection(Kirogi::UdpConfiguration configuration, uint8_t channel, QObject *parent = nullptr);
    ~MAVLinkUdpConnection();

    Kirogi::ConnectionConfiguration *configuration() override;

    bool connect() override;
    bool disconnect() override;

public Q_SLOTS:
    void processDataOnSocket();
    void sendBytes(const QByteArray &bytes) override;

private:
    QSharedPointer<QUdpSocket> m_socket;
    Kirogi::UdpConfiguration m_configuration;
    QVector<Kirogi::UdpConfiguration::UdpClient> m_targets;
};
