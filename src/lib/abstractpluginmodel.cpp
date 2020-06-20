/*
 * Copyright 2020 Tomaz Canabrava <tcanabrava@kde.org>
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

#include "abstractpluginmodel.h"
#include "debug.h"

#include <KPluginFactory>
#include <KPluginLoader>

#include <QCoreApplication>
#include <QMetaEnum>

namespace Kirogi
{
class Q_DECL_HIDDEN AbstractPluginModel::Private
{
public:
    explicit Private(AbstractPluginModel *q);
    ~Private();

    QVector<KPluginMetaData> plugins;

    // This is QMap so `AbstractPluginModel::loadedPlugins` returns a stable sort.
    QMap<QString, QObject *> loadedPlugins;

    void loadPluginByService(const QString &serviceType);

private:
    AbstractPluginModel *m_q;
};

AbstractPluginModel::Private::Private(AbstractPluginModel *q)
    : m_q(q)
{
}
AbstractPluginModel::Private::~Private() = default;

AbstractPluginModel::AbstractPluginModel(QObject *parent)
    : QAbstractListModel(parent)
    , d(new AbstractPluginModel::Private(this))
{
}

AbstractPluginModel::~AbstractPluginModel()
{
    unloadAllPlugins();
    delete d;
}

void AbstractPluginModel::Private::loadPluginByService(const QString &serviceType)
{
    auto filterLambda = [serviceType](const KPluginMetaData &metaData) -> bool { return metaData.serviceTypes().contains(serviceType); };
    const QString lowercaseMetadata = serviceType.toLower();

    // Look for plugins in a relative path, covers the case when the application is
    // not installed in the system.
    const QString possiblePluginPath = QCoreApplication::applicationDirPath() + QStringLiteral("/../lib/plugins/%1").arg(lowercaseMetadata);

    plugins = KPluginLoader::findPlugins(possiblePluginPath, filterLambda);
    plugins += KPluginLoader::findPlugins(lowercaseMetadata, filterLambda);

    // Unload plugins that apparently got uninstalled at runtime.
    for (const QString &id : loadedPlugins.keys()) {
        const bool found = std::any_of(plugins.constBegin(), plugins.constEnd(), [id](const auto &md) { return md.pluginId() == id; });
        if (!found) {
            loadedPlugins.take(id)->deleteLater();
        }
    }
}

void AbstractPluginModel::loadPluginByService(const QString &serviceType)
{
    d->loadPluginByService(serviceType);
}

bool AbstractPluginModel::loadPluginByIndex(int row)
{
    const KPluginMetaData &md = d->plugins.at(row);

    if (d->loadedPlugins.contains(md.pluginId())) {
        return false;
    }

    KPluginLoader loader(md.fileName(), this);
    KPluginFactory *factory = loader.factory();

    if (!factory) {
        qCWarning(KIROGI_CORE) << "Error loading plugin:" << md.pluginId() << "-" << loader.errorString();
        return false;
    }

    QObject *plugin = requestFromFactory(factory);

    if (!plugin) {
        qCWarning(KIROGI_CORE) << "Scheduling invalid plugin to be deleted:" << md.pluginId() << "/" << factory;
        factory->deleteLater();
        return false;
    }

    qCWarning(KIROGI_CORE) << "Loaded plugin with id:" << md.pluginId();
    d->loadedPlugins[md.pluginId()] = plugin;

    emit pluginLoaded(md.pluginId(), md.name(), plugin);
    const QModelIndex &idx = index(row, 0);
    emit dataChanged(idx, idx);

    return true;
}

bool AbstractPluginModel::loadPluginById(const QString &id)
{
    for (int i = 0; i < d->plugins.count(); ++i) {
        const KPluginMetaData &md = d->plugins.at(i);

        if (md.pluginId() == id) {
            return loadPluginByIndex(i);
        }
    }

    return false;
}

bool AbstractPluginModel::unloadPlugin(int row)
{
    const QString &id = d->plugins.at(row).pluginId();

    if (!d->loadedPlugins.contains(id)) {
        return false;
    }

    d->loadedPlugins.take(id)->deleteLater();

    const QModelIndex &idx = index(row, 0);
    emit dataChanged(idx, idx);

    return true;
}

bool AbstractPluginModel::unloadAllPlugins()
{
    if (!d->loadedPlugins.count()) {
        return false;
    }

    for (int i = 0; i < d->plugins.count(); ++i) {
        const KPluginMetaData &md = d->plugins.at(i);

        QObject *plugin = d->loadedPlugins.take(md.pluginId());

        if (plugin) {
            plugin->deleteLater();

            const QModelIndex &idx = index(i, 0);
            emit dataChanged(idx, idx);
        }
    }

    return true;
}

QHash<int, QByteArray> AbstractPluginModel::roleNames() const
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

int AbstractPluginModel::rowCount(const QModelIndex &parent) const
{
    if (!checkIndex(parent, CheckIndexOption::ParentIsInvalid)) {
        return 0;
    }

    return d->plugins.count();
}

KPluginMetaData AbstractPluginModel::metadataAt(int row) const
{
    Q_ASSERT(rowCount() > row);
    return d->plugins.at(row);
}

QObject *AbstractPluginModel::pluginForId(const QString &id) const
{
    return d->loadedPlugins.value(id, nullptr);
}

}
