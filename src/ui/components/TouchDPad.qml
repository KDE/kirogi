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
    id: pad

    readonly property real axisX: Math.round((((nub.x + (nub.width / 2)) - (width / 2))
        / (plate.radius - (nub.width / 2) - border.border.width)) * 100)
    readonly property real axisY: {
        var axisY = Math.round((((nub.y + (nub.height / 2)) - (height / 2))
        / (plate.radius - (nub.height / 2) - border.border.width)) * 100);

        if (axisY < 0) {
            return Math.abs(axisY);
        }

        return -axisY;
    }

    property bool aboutToMove: false
    property bool moved: false
    property var touchPos: null

    property string leftToolTipText: leftToolTip.text
    property string rightToolTipText: rightToolTip.text
    property string topToolTipText: topToolTip.text
    property string bottomToolTipText: bottomToolTip.text

    property alias background: blurredBackground.sourceItem
    property alias backgroundOpacity: plate.opacity
    property alias leftIcon: leftIcon.source
    property alias rightIcon: rightIcon.source
    property alias topIcon: topIcon.source
    property alias bottomIcon: bottomIcon.source

    Kirigami.Theme.colorSet: Kirigami.Theme.Button

    onTouchPosChanged: {
        if (touchPos) {
            var padCenter = width / 2;
            var nubCenter = nub.width / 2;
            var maxDistance = plate.radius - nubCenter - border.border.width;

            if ((touchPos.x - (padCenter)) * (touchPos.x - (padCenter))
                + (touchPos.y - (padCenter)) * (touchPos.y - (padCenter))
                <= maxDistance * maxDistance) {
                nub.x = touchPos.x - nubCenter;
                nub.y = touchPos.y - nubCenter;
            } else {
                var angle = Math.atan2(touchPos.y - padCenter, touchPos.x - padCenter);
                nub.x = Math.round(Math.cos(angle) * maxDistance + padCenter) - nubCenter;
                nub.y = Math.round(Math.sin(angle) * maxDistance + padCenter) - nubCenter;
            }
        } else {
            nub.x = Qt.binding(function() { return width / 2 - nub.width / 2; });
            nub.y = Qt.binding(function() { return width / 2 - nub.width / 2; });
        }
    }

    BlurredBackground { id: blurredBackground; mask: plate }

    Rectangle {
        id: plate

        anchors.fill: parent

        radius: width / 2

        color: "black"
        opacity: 0.15
    }

    Rectangle {
        id: border

        anchors.fill: parent

        radius: width / 2

        color: "transparent"
        opacity: 0.6

        border.width: 1
        border.color: "white"
    }

    Rectangle {
        id: nub

        width: 2 * Math.round((parent.width / 4.3) / 2);
        height: width

        radius: width / 2

        color: touchPos ? Kirigami.Theme.highlightColor : "white"
        opacity: 0.4

        Behavior on border.color { ColorAnimation { duration: Kirigami.Units.shortDuration } }
    }

    Rectangle {
        anchors.centerIn: nub

        width: 2 * Math.round((parent.width / 5) / 2)
        height: width

        radius: width / 2

        color: "white"
        opacity: 0.74
    }

    Kirigami.Icon {
        id: leftIcon

        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: parent.height * 0.06

        width: parent.height * 0.2
        height: width

        property alias hovered: leftHoverHandler.hovered

        color: "white"
        smooth: true

        HoverHandler {
            id: leftHoverHandler
        }

        QQC2.ToolTip.visible: leftHoverHandler.hovered && !page.touched
        QQC2.ToolTip.delay: Kirigami.Units.toolTipDelay
        QQC2.ToolTip.text: leftToolTipText
    }

    Kirigami.Icon {
        id: rightIcon

        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: parent.height * 0.06

        width: parent.height * 0.2
        height: width

        property alias hovered: rightHoverHandler.hovered

        color: "white"
        smooth: true

        HoverHandler {
            id: rightHoverHandler
        }

        QQC2.ToolTip.visible: rightHoverHandler.hovered && !page.touched
        QQC2.ToolTip.delay: Kirigami.Units.toolTipDelay
        QQC2.ToolTip.text: rightToolTipText
    }

    Kirigami.Icon {
        id: topIcon

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: parent.height * 0.06

        width: parent.height * 0.2
        height: width

        property alias hovered: topHoverHandler.hovered

        color: "white"
        smooth: true

        HoverHandler {
            id: topHoverHandler
        }

        QQC2.ToolTip.visible: topHoverHandler.hovered && !page.touched
        QQC2.ToolTip.delay: Kirigami.Units.toolTipDelay
        QQC2.ToolTip.text: topToolTipText
    }

    Kirigami.Icon {
        id: bottomIcon

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height * 0.06

        width: parent.height * 0.2
        height: width

        property alias hovered: bottomHoverHandler.hovered

        color: "white"
        smooth: true

        HoverHandler {
            id: bottomHoverHandler
        }

        QQC2.ToolTip.visible: bottomHoverHandler.hovered && !page.touched
        QQC2.ToolTip.delay: Kirigami.Units.toolTipDelay
        QQC2.ToolTip.text: bottomToolTipText
    }
}
