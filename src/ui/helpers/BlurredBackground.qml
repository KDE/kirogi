/*
 * Copyright 2019 Eike Hein <hein@kde.org>
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

import QtQuick 2.12
import QtGraphicalEffects 1.12

ShaderEffectSource {
    id: backgroundCrop

    property Item mask: null

    anchors.fill: parent

    sourceRect: Qt.rect(parent.x, parent.y, parent.width, parent.height)

    layer.enabled: true
    layer.effect: OpacityMask {
        anchors.fill: parent
        maskSource: mask
    }

    FastBlur {
        id: blur

        anchors.fill: parent

        source: parent
        radius: 16
    }
}
