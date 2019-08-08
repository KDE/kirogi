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
import QtQuick.Controls 2.12 as QQC2

import org.kde.kirigami 2.6 as Kirigami

Item {
    id: button

    width: Math.max(Kirigami.Units.iconSizes.medium, fontMetrics.height) + Kirigami.Units.smallSpacing * 2
    height: width

    signal tapped

    property string iconColor: "white"
    property string toolTipText: ""

    property alias background: blurredBackground.sourceItem
    property alias icon: icon.source
    property alias backgroundOpacity: plate.opacity
    property alias pressed: tapHandler.pressed
    property alias hovered: hoverHandler.hovered
    property alias containsMouse: hoverHandler.hovered

    Kirigami.Theme.colorSet: Kirigami.Theme.Button

    BlurredBackground { id: blurredBackground; mask: plate }

    Rectangle {
        id: plate

        anchors.fill: parent

        radius: width / 2

        color: "black"
        opacity: 0.31

        Behavior on border.color { ColorAnimation { duration: Kirigami.Units.shortDuration } }
    }

    Rectangle {
        id: border

        anchors.fill: parent

        radius: width / 2

        color: "transparent"
        opacity: (button.containsMouse || button.pressed) ? 1.0 : 0.7

        border.width: 2
        border.color: button.containsMouse ? Kirigami.Theme.hoverColor : (button.pressed ? Kirigami.Theme.highlightColor : "white")

        Behavior on opacity  { NumberAnimation { duration: Kirigami.Units.shortDuration } }
        Behavior on border.color { ColorAnimation { duration: Kirigami.Units.shortDuration } }
    }

    Kirigami.Icon {
        id: icon

        anchors.centerIn: parent

        width: parent.height * 0.65
        height: width

        color: button.containsMouse ? Kirigami.Theme.hoverColor : (button.pressed ? Kirigami.Theme.highlightColor : iconColor)
        smooth: true
        isMask: true

        Behavior on color { ColorAnimation { duration: Kirigami.Units.shortDuration } }
    }

    QQC2.ToolTip.visible: containsMouse && !tapHandler.pressed
    QQC2.ToolTip.delay: Kirigami.Units.toolTipDelay
    QQC2.ToolTip.text: toolTipText

    HoverHandler {
        id: hoverHandler

        enabled: !page.touched
    }

    TapHandler {
        id: tapHandler

        gesturePolicy: TapHandler.ReleaseWithinBounds

        onTapped: button.tapped()
    }
}
