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

#pragma once

#include <QAbstractListModel>

/**
 * @brief ParameterModel for qml interface.
 *
 */
class ParameterModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    /**
     * @brief Construct a new ParameterModel.
     *
     * @param parent
     */
    ParameterModel(QObject *parent = nullptr);

    /**
     * @brief Load description file
     *
     * @param path
     */
    void loadDescriptionFile(const QString &path);

    /**
     * @brief Roles
     *
     */
    enum Roles {
        Name = Qt::UserRole, // ACRO_BAL_PITCH
        HumanName, // Acro Balance Pitch
        Description, // rate at which pitch angle returns...
        Index, // 0
        Value, // X
        ValidValues, // { 0: "Disabled", "0.1": "Very Low", ... }
        Bitmask, // True/False
        Units, // deg
        Increment, // 0.1
        Range, // [0, 3]
        UserType, // Advanced/Standard
    };
    Q_ENUM(Roles)

    /**
     * @brief Return data.
     *
     * @param index
     * @param role
     * @return QVariant
     */
    QVariant data(const QModelIndex &index, int role) const override;

    /**
     * @brief Get role names.
     *
     * @return QHash<int, QByteArray>
     */
    QHash<int, QByteArray> roleNames() const override;

    /**
     * @brief Return ParameterModel size
     *
     * @param parent
     * @return int
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        Q_UNUSED(parent);
        return m_vectors[ParameterModel::Roles::Name].size();
    };

Q_SIGNALS:
    void countChanged();

    /**
     * @brief Update parameter
     *
     * @param parameter
     */
    void update(const QMap<ParameterModel::Roles, QVariant> &parameter);

private:
    Q_DISABLE_COPY(ParameterModel)

    /**
     * @brief Do update inside ParameterModel thread.
     *
     * @param parameter
     */
    void doUpdate(const QMap<ParameterModel::Roles, QVariant> &parameter);

    void initializeRoleNames();

    QHash<int, QByteArray> m_roleNames;
    QHash<int, QVector<QVariant>> m_vectors;
};
