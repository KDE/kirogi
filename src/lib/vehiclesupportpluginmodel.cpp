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
#include <KPluginLoader>
#include <KPluginMetaData>

#include <QCoreApplication>
#include <QMetaEnum>

namespace Kirogi
{
class Q_DECL_HIDDEN VehicleSupportPluginModel::Private
{
public:
    Private(VehicleSupportPluginModel *q);
    ~Private();

    QVector<KPluginMetaData> plugins;

    // This is QMap so `VehicleSupportPluginModel::loadedPlugins` returns a stable sort.
    QMap<QString, VehicleSupportPlugin *> loadedPlugins;

    void findPlugins();

private:
    VehicleSupportPluginModel *m_q;
};

VehicleSupportPluginModel::Private::Private(VehicleSupportPluginModel *q)
    : m_q(q)
{
}

VehicleSupportPluginModel::Private::~Private()
{
}

void VehicleSupportPluginModel::Private::findPlugins()
{
    auto filter = [](const KPluginMetaData &metaData) { return metaData.serviceTypes().contains(QStringLiteral("Kirogi/VehicleSupport")); };

    // Look for plugins in a relative path, covers the case when the application is
    // not installed in the system.
    plugins = KPluginLoader::findPlugins(QCoreApplication::applicationDirPath() + QStringLiteral("/../lib/plugins/kirogi/vehiclesupport"), filter);
    plugins += KPluginLoader::findPlugins(QStringLiteral("kirogi/vehiclesupport"), filter);

    // Unload plugins that apparently got uninstalled at runtime.
    for (const QString &id : loadedPlugins.keys()) {
        const bool found = std::any_of(plugins.constBegin(), plugins.constEnd(), [id](const auto& md) { return md.pluginId() == id; });
        if (!found) {
            delete loadedPlugins.take(id);
        }
    }
}

VehicleSupportPluginModel::VehicleSupportPluginModel(QObject *parent)
    : QAbstractListModel(parent)
    , d(new Private(this))
{
    // FIXME TODO: Watch KSycoca and reload when new plugins are installed at runtime.
    d->findPlugins();
}

VehicleSupportPluginModel::~VehicleSupportPluginModel()
{
}

QHash<int, QByteArray> VehicleSupportPluginModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();

    QMetaEnum e = metaObject()->enumerator(metaObject()->indexOfEnumerator("AdditionalRoles"));

    auto desCapitalize = [](const char *input) -> QByteArray {
        QByteArray array(input);
        return array.left(1).toLower() + array.mid(1);
    };

    for (int i = 0; i < e.keyCount(); ++i) {
        roles.insert(e.value(i), desCapitalize(e.key(i)));
    }

    return roles;
}

int VehicleSupportPluginModel::rowCount(const QModelIndex &parent) const
{
    if (!checkIndex(parent, CheckIndexOption::ParentIsInvalid)) {
        return 0;
    }

    return d->plugins.count();
}

QVariant VehicleSupportPluginModel::data(const QModelIndex &index, int role) const
{
    if (!checkIndex(index, CheckIndexOption::IndexIsValid | CheckIndexOption::ParentIsInvalid)) {
        return QVariant();
    }

    switch (role) {
    case Qt::DisplayRole: {
        return d->plugins.at(index.row()).name();
    }
    case Id: {
        return d->plugins.at(index.row()).pluginId();
    }
    case Status: {
        const QString &id = d->plugins.at(index.row()).pluginId();

        if (d->loadedPlugins.contains(id)) {
            return PluginLoaded;
        } else {
            return PluginNotLoaded;
        }
    }
    case Plugin: {
        const QString &id = d->plugins.at(index.row()).pluginId();

        return QVariant::fromValue(d->loadedPlugins.value(id));
    }
    }

    return QVariant();
}

bool VehicleSupportPluginModel::loadPlugin(int row)
{
    const KPluginMetaData &md = d->plugins.at(row);

    if (d->loadedPlugins.contains(md.pluginId())) {
        return false;
    }

    KPluginLoader loader(md.fileName(), this);
    KPluginFactory *factory = loader.factory();

    if (!factory) {
        qCWarning(KIROGI_CORE) << "Error loading plugin:" << md.pluginId() << "-" << loader.errorString();
    } else {
        VehicleSupportPlugin *vehicleSupportPlugin = factory->create<VehicleSupportPlugin>(this);

        if (!vehicleSupportPlugin) {
            qCWarning(KIROGI_CORE) << "Scheduling invalid plugin to be deleted:" << md.pluginId() << "/" << factory;
            factory->deleteLater();
        } else {
            qCWarning(KIROGI_CORE) << "Loaded plugin with id:" << md.pluginId();

            d->loadedPlugins[md.pluginId()] = vehicleSupportPlugin;

            emit pluginLoaded(md.pluginId(), md.name(), vehicleSupportPlugin);

            const QModelIndex &idx = index(row, 0);
            emit dataChanged(idx, idx, QVector<int> {Status, Plugin});
        }
    }

    return false;
}

bool VehicleSupportPluginModel::loadPluginById(const QString &id)
{
    for (int i = 0; i < d->plugins.count(); ++i) {
        const KPluginMetaData &md = d->plugins.at(i);

        if (md.pluginId() == id) {
            return loadPlugin(i);
        }
    }

    return false;
}

bool VehicleSupportPluginModel::unloadPlugin(int row)
{
    const QString &id = d->plugins.at(row).pluginId();

    if (!d->loadedPlugins.contains(id)) {
        return false;
    }

    delete d->loadedPlugins.take(id);

    const QModelIndex &idx = index(row, 0);
    emit dataChanged(idx, idx, QVector<int> {Status, Plugin});

    return true;
}

bool VehicleSupportPluginModel::unloadAllPlugins()
{
    if (!d->loadedPlugins.count()) {
        return false;
    }

    for (int i = 0; i < d->plugins.count(); ++i) {
        const KPluginMetaData &md = d->plugins.at(i);

        VehicleSupportPlugin *plugin = d->loadedPlugins.take(md.pluginId());

        if (plugin) {
            delete plugin;

            const QModelIndex &idx = index(i, 0);
            emit dataChanged(idx, idx, QVector<int> {Status, Plugin});
        }
    }

    return true;
}

}
