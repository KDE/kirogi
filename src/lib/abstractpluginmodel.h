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

#pragma once

#include <KPluginFactory>
#include <KPluginMetaData>
#include <QAbstractListModel>

#include "kirogicore_export.h"

namespace Kirogi
{
/* This class is the base class of different models for plugins in Kirogi
 */
class KIROGI_EXPORT AbstractPluginModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit AbstractPluginModel(QObject *parent = nullptr);
    ~AbstractPluginModel() override;

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override final;
    QObject *pluginForId(const QString &id) const;
    KPluginMetaData metadataAt(int row) const;

    void loadPluginByService(const QString &serviceType);
    Q_INVOKABLE bool loadPluginByIndex(int idx);
    Q_INVOKABLE bool loadPluginById(const QString &id);

    Q_INVOKABLE bool unloadPlugin(int row);
    Q_INVOKABLE bool unloadAllPlugins();

Q_SIGNALS:
    void pluginLoaded(const QString &pluginId, const QString &name, QObject *plugin) const;

protected:
    virtual QObject *requestFromFactory(KPluginFactory *factory) = 0;

private:
    class Private;
    Private *d;
};

}
