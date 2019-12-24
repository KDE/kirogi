/*
 * Copyright 2019 Eike Hein <hein@kde.org>
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

    property real pitch: 0.0
    property real tickWidth: 10
    width: tickWidth * 2

    ColumnLayout {
        id: layout
        Layout.margins: 0
        spacing: 0
        height: parent.height

        // Calculate position based in the pitch value.
        y: {
            var middlePos = parent.height / 2 + height / 2;
            var perc = ((pitch % 10) / repeater.model) / 10;
            var yPos = Math.round(middlePos + perc * height);

            return (parent.height - yPos);
        }

        // Worst case scenario width to allow right alignment.
        TextMetrics {
            id: textMetrics
            font: Text.font
            text: "000"
        }

        // Create bars.
        Repeater {
            id: repeater

            // Use an odd value to create middle bar.
            model: 11

            Rectangle {
                id: rectangle
                color: "white"
                width: root.tickWidth
                height: 2

                // Calculate opacity based in the index.
                opacity: Math.abs(1 / (index - repeater.model / 2 )) * 2

                Text {
                    id: text

                    // Display only positive values, the anchors will inform the direction.
                    text: Math.abs(value * 10)
                    width: textMetrics.width
                    height: 10
                    color: "white"
                    horizontalAlignment: Text.AlignRight

                    // Calculate real value.
                    readonly property var value: {
                        var multiplier = pitch > 0 ? Math.floor(pitch / 10) : Math.ceil(pitch / 10);
                        return (index - (repeater.model - 1) / 2) + multiplier;
                    }

                    // Change anchors based in value.
                    onValueChanged: {
                        if(state !== "positive" && value > 0) {
                            state = "positive";
                            return
                        }

                        if(state !== "negative" && value < 0) {
                            state = "negative";
                            return
                        }

                        if(state !== "zero" && value === 0) {
                            state = "zero";
                            return
                        }
                    }

                    states: [
                        State {
                            name: "zero"
                            AnchorChanges {
                                target: text
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.top: undefined
                                anchors.bottom: undefined
                            }
                        },
                        State {
                            name: "positive"
                            AnchorChanges {
                                target: text
                                anchors.verticalCenter: undefined
                                anchors.top: parent.bottom
                                anchors.bottom: undefined
                            }
                        },
                        State {
                            name: "negative"
                            AnchorChanges {
                                target: text
                                anchors.verticalCenter: undefined
                                anchors.top: undefined
                                anchors.bottom: parent.top
                            }
                        }
                    ]
                    anchors.left: parent.right
                    anchors.leftMargin: 5
                    anchors.verticalCenterOffset: -2
                    anchors.bottomMargin: 2
                    anchors.topMargin: -3
                }
            }

        }
    }

    Rectangle {
        id: middleBar

        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.left
        anchors.rightMargin: 2

        width: tickWidth
        height: 2

        color: "white"
    }
}
