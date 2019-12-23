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

#pragma once

#include <QAbstractListModel>

#include "kirogicore_export.h"

namespace Kirogi
{
class VehicleSupportPlugin;

class KIROGI_EXPORT VehicleSupportPluginModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum AdditionalRoles { Id = Qt::UserRole + 1, Status, Plugin };
    Q_ENUM(AdditionalRoles)

    enum PluginStatus { PluginNotLoaded = 0, PluginLoaded = 1 };
    Q_ENUM(PluginStatus)

    explicit VehicleSupportPluginModel(QObject *parent = nullptr);
    ~VehicleSupportPluginModel() override;

    QHash<int, QByteArray> roleNames() const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    Q_INVOKABLE bool loadPlugin(int row);
    Q_INVOKABLE bool loadPluginById(const QString &id);
    Q_INVOKABLE bool unloadPlugin(int row);

    Q_INVOKABLE bool unloadAllPlugins();

Q_SIGNALS:
    // FIXME TODO: QObject -> VehicleSupportPlugin
    void pluginLoaded(const QString &pluginId, const QString &name, QObject *plugin) const;

private:
    class Private;
    QScopedPointer<Private> d;
};

}
