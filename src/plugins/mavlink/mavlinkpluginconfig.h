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

/**
 * A singleton class for storing plugin-wide configurations.
 *
 * This class stores plugin-wide configurations of mavlink plugin and provides interfaces for
 * managing them to QML.
 */
class MAVLinkPluginConfig
{
    Q_GADGET

    Q_PROPERTY(int sysid READ sysid WRITE setSysid)
    Q_PROPERTY(int compid READ compid WRITE setCompid)

public:
    static MAVLinkPluginConfig &instance()
    {
        static MAVLinkPluginConfig instance;
        return instance;
    }

    int sysid() const;  ///< Returns system id of this GCS.
    int compid() const; ///< Returns default component id of this GCS.

    void setSysid(int sysid);
    void setCompid(int compid);

private:
    explicit MAVLinkPluginConfig();
    ~MAVLinkPluginConfig();

    int m_sysid;  ///< A system id of this GCS.
    int m_compid; ///< A component id of this GCS.
};
