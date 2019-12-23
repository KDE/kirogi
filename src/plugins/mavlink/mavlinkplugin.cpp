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

#include "mavlinkplugin.h"
#include "debug.h"
#include "mavlinkvehicle.h"

#include <KLocalizedString>
#include <KPluginFactory>

MAVLinkPlugin::MAVLinkPlugin(QObject *parent, const QVariantList & /* args */)
    : Kirogi::VehicleSupportPlugin(parent)
    , m_vehicle(nullptr)
{
    qCDebug(KIROGI_VEHICLESUPPORT_MAVLINK) << "MAVLink Vehicle Support Plugin initializing ...";

    m_vehicle = new MAVLinkVehicle(this);
    m_vehicle->connectToVehicle();

    QMetaObject::invokeMethod(this, "vehicleAdded", Qt::QueuedConnection, Q_ARG(Kirogi::AbstractVehicle *, m_vehicle));
}

MAVLinkPlugin::~MAVLinkPlugin()
{
    qCDebug(KIROGI_VEHICLESUPPORT_MAVLINK) << "MAVLink Vehicle Support Plugin unloaded.";
}

QList<Kirogi::AbstractVehicle *> MAVLinkPlugin::vehicles() const
{
    return QList<Kirogi::AbstractVehicle *>() << m_vehicle;
}

K_PLUGIN_CLASS_WITH_JSON(MAVLinkPlugin, "kirogimavlinkplugin.json")

#include "mavlinkplugin.moc"
