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

#include "positionsource.h"

#include "debug.h"

#include <QQmlEngine>

PositionSource::PositionSource()
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    createPositionSource();
}

void PositionSource::createPositionSource()
{
    // The smartpointer will take care of the pointer, that's why we are using nullptr as parent.
    m_geoPositionSource.reset(QGeoPositionInfoSource::createDefaultSource(nullptr));
    if (m_geoPositionSource.isNull()) {
        qCWarning(POSITIONSOURCE) << "Position source is not available for this device.";
        return;
    }

    qCDebug(POSITIONSOURCE) << "New position source:" << m_geoPositionSource->sourceName();

    // Set the update interval to 3s.
    m_geoPositionSource->setUpdateInterval(3000);
    m_geoPositionSource->setPreferredPositioningMethods(QGeoPositionInfoSource::SatellitePositioningMethods);
    m_geoPositionSource->startUpdates();

    connect(m_geoPositionSource.get(),
            QOverload<QGeoPositionInfoSource::Error>::of(&QGeoPositionInfoSource::error),
            this,
            &PositionSource::setPositionSourceError);
    connect(m_geoPositionSource.get(), &QGeoPositionInfoSource::positionUpdated, this, &PositionSource::setPositionInfo);
}

bool PositionSource::enabled() const
{
    return m_enabled;
}

void PositionSource::setEnabled(bool enabled)
{
    if (m_enabled == enabled) {
        return;
    }

    m_enabled = enabled;
    emit enabledChanged(m_enabled);
}

void PositionSource::setPositionInfo(const QGeoPositionInfo &geoPositionInfo)
{
    if (!geoPositionInfo.isValid()) {
        qCWarning(POSITIONSOURCE) << "Not valid position info from position source:" << geoPositionInfo;
        return;
    }

    const auto geoCoordinate = geoPositionInfo.coordinate();
    if (m_geoCoordinate == geoCoordinate) {
        return;
    }

    m_geoCoordinate = geoCoordinate;
    emit coordinateChanged(m_geoCoordinate);
}

QGeoCoordinate PositionSource::coordinate() const
{
    return m_geoCoordinate;
}

const QGeoPositionInfoSource *PositionSource::positionInfoSource() const
{
    return m_geoPositionSource.get();
}

void PositionSource::setPositionSourceError(QGeoPositionInfoSource::Error positioningError)
{
    qWarning(POSITIONSOURCE) << "Position source error:" << positioningError;
}

QObject *PositionSource::qmlSingletonRegister(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return &self();
}

PositionSource &PositionSource::self()
{
    static PositionSource self;
    return self;
}
