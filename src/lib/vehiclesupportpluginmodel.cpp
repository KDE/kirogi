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

#include "vehiclesupportpluginmodel.h"
#include "debug.h"
#include "vehiclesupportplugin.h"

#include <KPluginFactory>

#include <QCoreApplication>
#include <QMetaEnum>

namespace Kirogi
{
VehicleSupportPluginModel::VehicleSupportPluginModel(QObject *parent)
    : AbstractPluginModel(parent)
{
    loadPluginByType(QStringLiteral("vehiclesupport"));
}

VehicleSupportPluginModel::~VehicleSupportPluginModel() = default;

QVariant VehicleSupportPluginModel::data(const QModelIndex &index, int role) const
{
    if (!checkIndex(index, CheckIndexOption::IndexIsValid | CheckIndexOption::ParentIsInvalid)) {
        return QVariant();
    }

    auto metadata = metadataAt(index.row());
    const QString &id = metadata.pluginId();

    switch (role) {
    case Qt::DisplayRole:
        return metadata.name();
    case Id:
        return metadata.pluginId();
    case Status:
        return pluginForId(id) ? PluginLoaded : PluginNotLoaded;
    case Plugin:
        return QVariant::fromValue(pluginForId(id));
    }

    return QVariant();
}

QObject *VehicleSupportPluginModel::requestFromFactory(KPluginFactory *factory)
{
    return factory->create<VehicleSupportPlugin>(this);
}

}
