
/*
 * Copyright 2019 Eike Hein <hein@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "permissions.h"

#ifdef Q_OS_ANDROID
#include <QtAndroid>
#endif

Permissions::Permissions(const QStringList &permissions, QObject *parent)
    : QObject(parent)
    , m_permissions(permissions)
    , m_granted(false)
{
}

Permissions::~Permissions()
{
}

bool Permissions::granted() const
{
    return true;
}

void Permissions::request()
{
    if (m_granted) {
        return;
    }

#ifdef Q_OS_ANDROID
    QtAndroid::requestPermissions(m_permissions, [this](const QtAndroid::PermissionResultMap &permissions) {
        int granted = 0;

        for (const QString &permission : m_permissions) {
            if (permissions.contains(permission) && permissions.value(permission) == QtAndroid::PermissionResult::Granted) {
                ++granted;
            }
        }

        m_granted = (granted = m_permissions.count());
    });
#else
    m_granted = true;
    emit grantedChanged();
#endif
}
