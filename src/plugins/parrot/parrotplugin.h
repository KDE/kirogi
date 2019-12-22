/*
 * Copyright 2019 Eike Hein <hein@kde.org>
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

#ifndef PARROTPLUGIN_H
#define PARROTPLUGIN_H

#include "abstractvehicle.h"
#include "vehiclesupportplugin.h"

#include <QHash>

class ParrotVehicle;

#ifndef Q_OS_ANDROID
namespace KDNSSD
{
class ServiceBrowser;
}
#endif

class ParrotPlugin : public Kirogi::VehicleSupportPlugin
{
    Q_OBJECT

public:
    ParrotPlugin(QObject *parent, const QVariantList &args);
    ~ParrotPlugin() override;

    QList<Kirogi::AbstractVehicle *> vehicles() const override;

private:
    QHash<QString, Kirogi::AbstractVehicle *> m_vehicles;
#ifndef Q_OS_ANDROID
    KDNSSD::ServiceBrowser *m_bebop2Browser;
    KDNSSD::ServiceBrowser *m_anafiBrowser;
#endif
};

#endif
