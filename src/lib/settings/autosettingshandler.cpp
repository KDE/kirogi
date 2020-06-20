/*
 * Copyright 2019 Kevin Ottens <kevin.ottens@enioka.com>
 * Copyright 2019 Tomaz Canabrava <tcanabrava@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "autosettingshandler.h"

#include <KCoreConfigSkeleton>
#include <QDebug>
#include <QMetaProperty>

void AutoSettingsHandler::addSettings(KCoreConfigSkeleton *setting)
{
    m_settings.append(setting);
    auto settingsChangedSlotIndex = metaObject()->indexOfMethod("triggerSave()");
    auto settingsChangedSlot = metaObject()->method(settingsChangedSlotIndex);

    for (const auto &item : setting->items()) {
        const auto signallingItem = dynamic_cast<KConfigCompilerSignallingItem *>(item);

        if (!signallingItem) {
            continue;
        }

        QString name = signallingItem->name();
        name[0] = name[0].toLower();

        const auto metaObject = setting->metaObject();
        const auto propertyIndex = metaObject->indexOfProperty(name.toUtf8().constData());

        const auto property = metaObject->property(propertyIndex);
        if (!property.hasNotifySignal()) {
            continue;
        }

        const auto changedSignal = property.notifySignal();
        QObject::connect(setting, changedSignal, this, settingsChangedSlot);
    }
}

void AutoSettingsHandler::triggerSave()
{
    for (const auto &setting : qAsConst(m_settings)) {
        setting->save();
    }
}
