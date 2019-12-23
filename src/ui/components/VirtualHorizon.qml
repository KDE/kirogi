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
import QtQuick.Window 2.12

import org.kde.kirigami 2.6 as Kirigami

Item {
    property alias roll: rollBar.rotation

    height: width

    clip: true
    antialiasing: true

    Item {
        anchors.fill: parent

        id: rollBar

        Behavior on rotation { NumberAnimation { duration: Kirigami.Units.shortDuration } }

        Rectangle {
            x: leftBar.x + 1
            y: leftBar.y + 1

            width: leftBar.width
            height: leftBar.height

            color: "grey"

            antialiasing: true
        }

        Rectangle {
            id: leftBar

            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left

            width: parent.width * 0.25
            height: 2

            color: "white"

            antialiasing: true
        }

        Rectangle {
            x: rightBar.x + 1
            y: rightBar.y + 1

            width: rightBar.width
            height: rightBar.height

            color: "grey"

            antialiasing: true
        }

        Rectangle {
            id: rightBar

            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right

            width: parent.width * 0.25
            height: 2

            color: "white"

            antialiasing: true
        }
    }

    // Clip circle to create semi-circle effect
    Item {
        anchors.centerIn: parent
        width: parent.width
        height: width * 0.3
        clip: true

        Rectangle {
            id: circle

            width: parent.width
            height: width
            radius: width / 2

            anchors.centerIn: parent

            color: "transparent"

            border.width: 2
            border.color: "white"

            antialiasing: true
        }
    }
}
