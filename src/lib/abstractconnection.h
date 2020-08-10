/*
 * Copyright 2020 Kitae Kim <develoot@gmail.com>
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

#include "connectionconfiguration.h"

#include <QObject>

#include "kirogicore_export.h"

namespace Kirogi
{
/**
 * An abstract class of UDP, TCP and Serial connections.
 *
 * Plugin authors have to inherit this class to implement their own connection class.
 */
class KIROGI_EXPORT AbstractConnection : public QObject
{
    Q_OBJECT

    Q_PROPERTY(ConnectionConfiguration *configuration READ configuration CONSTANT)
    Q_PROPERTY(State state READ state NOTIFY stateChanged)

public:
    enum class State { Disconnected, Connected };
    Q_ENUM(State);

    AbstractConnection(QObject *parent = nullptr);
    ~AbstractConnection();

    virtual bool connect() = 0;
    virtual bool disconnect() = 0;

    virtual ConnectionConfiguration *configuration() = 0;

    State state() const;

Q_SIGNALS:
    void stateChanged();

public Q_SLOTS:
    virtual void sendBytes(const QByteArray &bytes) = 0;

protected:
    void setState(State state);

private:
    State m_state;
};
}
