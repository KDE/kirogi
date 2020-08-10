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

#include "connectionconfiguration.h"

#include <QHostAddress>
#include <QObject>

#include "kirogicore_export.h"

/**
 * A class that stores properties of UDP connection.
 *
 * This class provides interfaces to read/write properties of UDP connection to QML.
 */
namespace Kirogi
{
class KIROGI_EXPORT UdpConfiguration : public ConnectionConfiguration
{
    Q_GADGET

    Q_PROPERTY(QHostAddress host READ host WRITE setHost)
    Q_PROPERTY(int port READ port WRITE setPort)

public:
    struct UdpClient {
        QHostAddress address;
        int port;
    };

    UdpConfiguration();
    ~UdpConfiguration();

    ConnectionConfiguration::Type type() const final
    {
        return ConnectionConfiguration::Type::UDP;
    }

    void setHost(QHostAddress host);
    void setPort(int port);

    QHostAddress host() const;
    int port() const;

private:
    UdpClient m_host;
};
}
