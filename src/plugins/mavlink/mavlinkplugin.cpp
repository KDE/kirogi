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

#include "mavlinkudpconnection.h"
#include "mavlinkvehicle.h"

#include <KLocalizedString>
#include <KPluginFactory>

#include "mavlink_plugin_debug.h"

MAVLinkPlugin::MAVLinkPlugin(QObject *parent, const QVariantList & /* args */)
    : Kirogi::VehicleSupportPlugin(parent)
{
    qCDebug(KIROGI_VEHICLESUPPORT_MAVLINK_PLUGIN) << "MAVLink Vehicle Support Plugin initializing ...";
    qRegisterMetaType<mavlink_message_t>("mavlink_message_t");

    Kirogi::UdpConfiguration connectionConfiguration;
    connectionConfiguration.setName("Main Connection");
    connectionConfiguration.setHost(QHostAddress("0.0.0.0"));
    connectionConfiguration.setPort(14550);
    m_connection = QSharedPointer<MAVLinkUdpConnection>(new MAVLinkUdpConnection(connectionConfiguration, 0, this));
    m_connection->connect();

    MAVLinkVehicle::Configuration vehicleConfiguration;
    vehicleConfiguration.sysid = 0;
    vehicleConfiguration.compid = 0;
    vehicleConfiguration.type = MAV_TYPE_QUADROTOR;
    vehicleConfiguration.name = "Main Vehicle";
    m_vehicle = QSharedPointer<MAVLinkVehicle>(new MAVLinkVehicle(vehicleConfiguration, m_connection.data(), this));

    QMetaObject::invokeMethod(this, "vehicleAdded", Qt::QueuedConnection, Q_ARG(Kirogi::AbstractVehicle *, m_vehicle.data()));
}

MAVLinkPlugin::~MAVLinkPlugin()
{
    qCDebug(KIROGI_VEHICLESUPPORT_MAVLINK_PLUGIN) << "MAVLink Vehicle Support Plugin unloaded.";
}

QList<Kirogi::AbstractVehicle *> MAVLinkPlugin::vehicles() const
{
    return QList<Kirogi::AbstractVehicle *>() << m_vehicle.data();
}

K_PLUGIN_CLASS_WITH_JSON(MAVLinkPlugin, "kirogimavlinkplugin.json")

#include "mavlinkplugin.moc"
