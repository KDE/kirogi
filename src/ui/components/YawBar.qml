/*
 * Copyright 2019 Patrick José Pereira <patrickjp@kde.org>
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
import QtQuick.Layouts 1.0

Item {
    id: root

    property real yaw: 0
    property real tickWidth: 10
    height: tickWidth * 2

    RowLayout {
        id: layout

        Layout.margins: 0
        spacing: 0
        width: parent.width

        // Set position of ticks and values.
        x: {
            var xPos = root.width / 2 + width / 2 - (root.width / (repeater.model * 2))

            if (kirogi.ready) {
                var perc = ((yaw % 15) / repeater.model) / 15;
                xPos = Math.round(xPos + perc * width);
            }
            return parent.width - xPos - 1;
        }

        // Create bars.
        Repeater {
            id: repeater

            model: 11

            Rectangle {
                id: rectangle

                color: "white"
                width: 2
                height: root.tickWidth

                opacity: Math.abs(2 / (index - repeater.model / 2 ))

                Text {
                    id: text

                    width: 10
                    height: 10
                    color: "white"

                    text: getDirectionFromAngle(scaledValue)

                    // Scaled value to use in visualization.
                    property var scaledValue: (value * 15) % 360

                    // Calculate the actual value based in yaw.
                    property var value: {
                        var multiplier = yaw > 0 ? Math.floor(yaw / 15) : Math.ceil(yaw / 15)
                        return (index - (repeater.model - 1) / 2) + multiplier
                    }

                    // Return wind rose direction.
                    function getDirectionFromAngle(angle) {
                        switch(angle) {
                            case 0:
                                return 'N';

                            case 90:
                                return 'L';

                            case -180:
                            case 180:
                                return 'S';

                            case -90:
                            case 270:
                                return 'O';

                            default:
                                return angle;
                        }
                    }

                    horizontalAlignment: Text.AlignHCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.top
                    anchors.bottomMargin: 4
                }
            }

        }
    }

    Rectangle {
        id: middleBar

        anchors.horizontalCenter: parent.horizontalCenter

        anchors.top: layout.bottom
        anchors.topMargin: 2

        height: tickWidth
        width: 2

        color: "white"
    }
}
