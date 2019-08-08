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
    property real pitch: 0.0

    Rectangle {
        x: height
        y: height

        width: parent.width
        height: 1

        color: "grey"

        opacity: 0.5
    }

    Rectangle {
        y: 0

        width: parent.width - x
        height: 1

        opacity: 0.5
    }

    Rectangle {
        x: height
        y: height + ((parent.height / 30) * 1)

        width: parent.width
        height: 1

        color: "grey"

        opacity: 0.6
    }


    Rectangle {
        y: (parent.height / 30) * 1

        width: parent.width - x
        height: 1

        opacity: 0.6
    }

    Rectangle {
        x: height
        y: height + ((parent.height / 30) * 2)

        width: parent.width
        height: 1

        color: "grey"

        opacity: 0.7
    }

    Rectangle {
        y: (parent.height / 30) * 2

        width: parent.width - x
        height: 1

        opacity: 0.7
    }

    Rectangle {
        x: height
        y: height + ((parent.height / 30) * 3)

        width: parent.width
        height: 1

        color: "grey"

        opacity: 0.8
    }

    Rectangle {
        y: (parent.height / 30) * 3

        width: parent.width - x
        height: 1

        opacity: 0.8
    }

    Rectangle {
        x: height
        y: height + ((parent.height / 30) * 4)

        width: parent.width
        height: 1

        color: "grey"

        opacity: 0.9
    }

    Rectangle {
        y: (parent.height / 30) * 4

        width: parent.width - x
        height: 1

        opacity: 0.9
    }

    Rectangle {
        x: height
        y: height + ((parent.height / 30) * 5)

        width: parent.width
        height: 1

        color: "grey"
    }

    Rectangle {
        y: (parent.height / 30) * 5

        width: parent.width - x - x
        height: 1
    }

    Rectangle {
        x: height
        y: height + ((parent.height / 30) * 6)

        width: parent.width
        height: 1

        color: "grey"
    }

    Rectangle {
        y: (parent.height / 30) * 6

        width: parent.width - x
        height: 1
    }

    Rectangle {
        x: height
        y: height + ((parent.height / 30) * 7)

        width: parent.width
        height: 1

        color: "grey"
    }

    Rectangle {
        y: (parent.height / 30) * 7

        width: parent.width - x
        height: 1
    }

    Rectangle {
        x: height
        y: height + ((parent.height / 30) * 8)

        width: parent.width
        height: 1

        color: "grey"
    }

    Rectangle {
        y: (parent.height / 30) * 8

        width: parent.width - x
        height: 1
    }

    Rectangle {
        x: height
        y: height + ((parent.height / 30) * 9)

        width: parent.width
        height: 1

        color: "grey"
    }

    Rectangle {
        y: (parent.height / 30) * 9

        width: parent.width - x
        height: 1
    }

    Rectangle {
        x: height
        y: height + ((parent.height / 30) * 10)

        width: parent.width
        height: 1

        color: "grey"
    }

    Rectangle {
        y: (parent.height / 30) * 10

        width: parent.width - x
        height: 1
    }

    Rectangle {
        x: height
        y: height + ((parent.height / 30) * 11)

        width: parent.width
        height: 1

        color: "grey"
    }

    Rectangle {
        y: (parent.height / 30) * 11

        width: parent.width - x
        height: 1
    }

    Rectangle {
        x: height
        y: height + ((parent.height / 30) * 12)

        width: parent.width
        height: 1

        color: "grey"
    }

    Rectangle {
        y: (parent.height / 30) * 12

        width: parent.width - x
        height: 1
    }

    Rectangle {
        x: height
        y: height + ((parent.height / 30) * 13)

        width: parent.width
        height: 1

        color: "grey"
    }

    Rectangle {
        y: (parent.height / 30) * 13

        width: parent.width - x
        height: 1
    }

    Rectangle {
        x: height
        y: height + ((parent.height / 30) * 14)

        width: parent.width
        height: 1

        color: "grey"
    }

    Rectangle {
        y: (parent.height / 30) * 14

        width: parent.width - x
        height: 1
    }

    Rectangle {
        x: height
        y: height + ((parent.height / 30) * 15)

        width: parent.width
        height: 1

        color: "grey"
    }

    Rectangle {
        y: (parent.height / 30) * 15

        width: parent.width - x
        height: 1
    }

    Rectangle {
        x: height
        y: height + ((parent.height / 30) * 16)

        width: parent.width
        height: 1

        color: "grey"
    }

    Rectangle {
        y: (parent.height / 30) * 16

        width: parent.width - x
        height: 1
    }

    Rectangle {
        x: height
        y: height + ((parent.height / 30) * 17)

        width: parent.width
        height: 1

        color: "grey"
    }

    Rectangle {
        y: (parent.height / 30) * 17

        width: parent.width - x
        height: 1
    }

    Rectangle {
        x: height
        y: height + ((parent.height / 30) * 18)

        width: parent.width
        height: 1

        color: "grey"
    }

    Rectangle {
        y: (parent.height / 30) * 18

        width: parent.width - x
        height: 1
    }

    Rectangle {
        x: height
        y: height + ((parent.height / 30) * 19)

        width: parent.width
        height: 1

        color: "grey"
    }

    Rectangle {
        y: (parent.height / 30) * 19

        width: parent.width - x
        height: 1
    }

    Rectangle {
        x: height
        y: height + ((parent.height / 30) * 20)

        width: parent.width
        height: 1

        color: "grey"
    }

    Rectangle {
        y: (parent.height / 30) * 20

        width: parent.width - x
        height: 1
    }

    Rectangle {
        x: height
        y: height + ((parent.height / 30) * 21)

        width: parent.width
        height: 1

        color: "grey"
    }

    Rectangle {
        y: (parent.height / 30) * 21

        width: parent.width - x
        height: 1
    }

    Rectangle {
        x: height
        y: height + ((parent.height / 30) * 22)

        width: parent.width
        height: 1

        color: "grey"
    }

    Rectangle {
        y: (parent.height / 30) * 22

        width: parent.width - x
        height: 1
    }

    Rectangle {
        x: height
        y: height + ((parent.height / 30) * 23)

        width: parent.width
        height: 1

        color: "grey"
    }

    Rectangle {
        y: (parent.height / 30) * 23

        width: parent.width - x
        height: 1
    }

    Rectangle {
        x: height
        y: height + ((parent.height / 30) * 24)

        width: parent.width
        height: 1

        color: "grey"
    }

    Rectangle {
        y: (parent.height / 30) * 24

        width: parent.width - x
        height: 1
    }

    Rectangle {
        x: height
        y: height + ((parent.height / 30) * 25)

        width: parent.width
        height: 1

        color: "grey"
    }

    Rectangle {
        y: (parent.height / 30) * 25

        width: parent.width - x
        height: 1
    }

    Rectangle {
        x: height
        y: height + ((parent.height / 30) * 26)

        width: parent.width
        height: 1

        color: "grey"

        opacity: 0.9
    }

    Rectangle {
        y: (parent.height / 30) * 26

        width: parent.width - x
        height: 1

        opacity: 0.9
    }

    Rectangle {
        x: height
        y: height + ((parent.height / 30) * 27)

        width: parent.width
        height: 1

        color: "grey"

        opacity: 0.8
    }

    Rectangle {
        y: (parent.height / 30) * 27

        width: parent.width - x
        height: 1

        opacity: 0.8
    }

    Rectangle {
        x: height
        y: height + ((parent.height / 30) * 28)

        width: parent.width
        height: 1

        color: "grey"

        opacity: 0.7
    }

    Rectangle {
        y: (parent.height / 30) * 28

        width: parent.width - x
        height: 1

        opacity: 0.7
    }

    Rectangle {
        x: height
        y: height + ((parent.height / 30) * 29)

        width: parent.width
        height: 1

        color: "grey"

        opacity: 0.6
    }

    Rectangle {
        y: (parent.height / 30) * 29

        width: parent.width - x
        height: 1

        opacity: 0.6
    }

    Rectangle {
        x: height
        y: height + ((parent.height / 30) * 30)

        width: parent.width
        height: 1

        color: "grey"

        opacity: 0.5
    }

    Rectangle {
        y: (parent.height / 30) * 30

        width: parent.width - x
        height: 1

        opacity: 0.5
    }

    Rectangle {
        x: middleBar.x + 1
        y: middleBar.y + 1

        width: middleBar.width
        height: middleBar.height

        color: "grey"
    }

    Rectangle {
        id: middleBar

        anchors.horizontalCenter: parent.horizontalCenter

        y: {
            var yPos = parent.height / 2;

            if (kirogi.ready) {
                var percent = pitch / 180;
                var yPos = Math.round(parent.height * percent);
            }

            return (parent.height - yPos);
        }

        width: parent.width * 2
        height: 2

        color: "white"

        Behavior on y { NumberAnimation { duration: Kirigami.Units.shortDuration } }
    }
}
