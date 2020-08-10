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

#include <QObject>
#include <QString>

#include "kirogicore_export.h"

/**
 * A class that stores common properties of connections.
 *
 * This class provides interfaces to read/write properties to QML.
 */
namespace Kirogi
{
class KIROGI_EXPORT ConnectionConfiguration
{
    Q_GADGET

    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(bool autoconnect READ autoconnect WRITE setAutoconnect)
    Q_PROPERTY(Type type READ type CONSTANT)

public:
    enum class Type { UDP, TCP, Serial };
    Q_ENUM(Type);

    ConnectionConfiguration();
    virtual ~ConnectionConfiguration() = default;

    QString name() const;
    bool autoconnect() const;
    virtual Type type() const = 0;

    void setName(const QString &name);
    void setAutoconnect(bool autoconnect);

private:
    QString m_name;
    bool m_autoconnect;
};
}
