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

#include "parametermodel.h"

#include <QDebug>
#include <QMetaEnum>

ParameterModel::ParameterModel(QObject *parent)
    : QAbstractListModel(parent)
{
    initializeRoleNames();

    // Initialize vector.
    for (const auto &key : m_roleNames.keys()) {
        m_vectors.insert(key, {});
    }

    // Allow the usage of this class in different threads.
    connect(this, &ParameterModel::update, this, &ParameterModel::doUpdate);
}

void ParameterModel::initializeRoleNames()
{
    Q_ASSERT(m_roleNames.empty());

    auto desCapitalize = [](const char *input) -> QByteArray {
        QByteArray array(input);
        return array.left(1).toLower() + array.mid(1);
    };

    QMetaEnum e = metaObject()->enumerator(metaObject()->indexOfEnumerator("Roles"));
    for (int i{0}; i < e.keyCount(); i++) {
        m_roleNames.insert(e.value(i), desCapitalize(e.key(i)));
    }
}

void ParameterModel::loadDescriptionFile(const QString &path)
{
    Q_UNUSED(path)
    // TODO: Deal with description files.
}

void ParameterModel::doUpdate(const QMap<ParameterModel::Roles, QVariant> &parameter)
{
    // Check if parameter argument contains name.
    if (!parameter.contains(ParameterModel::Roles::Name)) {
        qDebug() << "No name identification in parameter";
        return;
    };

    // If name already exists in the model, get the index and update the other keys.
    int nameIndex = -1;
    if (m_vectors[ParameterModel::Roles::Name].contains(parameter[ParameterModel::Roles::Name])) {
        nameIndex = m_vectors[ParameterModel::Roles::Name].indexOf(parameter[ParameterModel::Roles::Name]);
    }

    const int line = rowCount();

    beginInsertRows(QModelIndex(), line, line);
    // Run over all keys in the parameter argument and update the model.
    QMapIterator<ParameterModel::Roles, QVariant> parameterIterator{parameter};
    while (parameterIterator.hasNext()) {
        parameterIterator.next();
        // Ignore name if already exist in the model.
        if (nameIndex >= 0 && parameterIterator.key() == ParameterModel::Roles::Name) {
            continue;
        }

        // Update the key if name already exist otherwise append it in the model.
        if (nameIndex >= 0) {
            m_vectors[parameterIterator.key()][nameIndex] = parameterIterator.value();
        } else {
            m_vectors[parameterIterator.key()].append(parameterIterator.value());
        }
    }
    const auto &indexRow = index(line);
    endInsertRows();

    emit dataChanged(indexRow, indexRow, QVector<int>::fromList(m_vectors.keys()));
    emit countChanged();
}

QVariant ParameterModel::data(const QModelIndex &index, int role) const
{
    if (!m_roleNames.contains(role)) {
        return {"No valid data"};
    }

    return m_vectors[role][index.row()];
}

QHash<int, QByteArray> ParameterModel::roleNames() const
{
    return m_roleNames;
}
