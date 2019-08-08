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
import QtQuick.Window 2.12

import org.kde.kirigami 2.6 as Kirigami

Item {
    id: pillBox

    LayoutMirroring.enabled: false
    LayoutMirroring.childrenInherit: true

    property Item background: null
    property var backgroundColor: "black"
    property real backgroundOpacity: 0.31
    property var borderColor: "white"
    property var borderWidth: 2
    property var borderRadius: height / 2

    Item {
        id: pillShapeMask

        visible: false

        anchors.fill: parent

        Rectangle {
            visible: true

            anchors.left: parent.left

            width: parent.height
            height: width

            radius: borderRadius

            color: "black"
        }

        Rectangle {
            visible: true

            anchors.right: parent.right

            width: parent.height
            height: width

            radius: borderRadius

            color: "black"
        }

        Rectangle {
            visible: true

            anchors.left: parent.left
            anchors.leftMargin: parent.height / 2
            anchors.right: parent.right
            anchors.rightMargin: parent.height / 2

            height: parent.height

            color: "black"
        }
    }

    BlurredBackground {
        sourceItem: background
        mask: pillShapeMask
    }

    Rectangle {
        id: plate

        anchors.fill: parent

        color: backgroundColor
        opacity: backgroundOpacity

        layer.enabled: true
        layer.effect: OpacityMask {
            anchors.fill: parent
            maskSource: pillShapeMask
        }
    }

    Item {
        id: leftBorderMask

        visible: false

        anchors.fill: leftBorder

        Rectangle {
            visible: true

            anchors.fill: parent
            anchors.rightMargin: leftBorder.width - leftBorder.radius

            color: "black"
        }
    }

    Rectangle {
        id: leftBorder

        anchors.left: parent.left

        width: parent.height
        height: width

        radius: borderRadius

        color: "transparent"
        opacity: 0.69

        border.width: borderWidth
        border.color: borderColor

        layer.enabled: true
        layer.effect: OpacityMask {
            anchors.fill: leftBorder

            maskSource: leftBorderMask
        }
    }

    Item {
        id: rightBorderMask

        visible: false

        anchors.fill: rightBorder

        Rectangle {
            visible: true

            anchors.fill: parent
            anchors.leftMargin: rightBorder.width - rightBorder.radius

            color: "black"
        }
    }

    Rectangle {
        id: rightBorder

        anchors.right: parent.right

        width: parent.height
        height: width

        radius: borderRadius

        color: "transparent"
        opacity: leftBorder.opacity
        border.width: borderWidth
        border.color: borderColor

        layer.enabled: true
        layer.effect: OpacityMask {
            anchors.fill: rightBorder

            maskSource: rightBorderMask
        }
    }

    Item {
        id: middleBorderMask

        visible: false

        anchors.fill: middleBorder

        Rectangle {
            visible: true

            width: leftBorder.radius
            height: parent.height

            color: "black"
        }

        Rectangle {
            visible: true

            anchors.fill: parent
            anchors.leftMargin: leftBorder.radius
            anchors.rightMargin: rightBorder.radius
            anchors.topMargin: borderWidth
            anchors.bottomMargin: borderWidth

            color: "black"
        }

        Rectangle {
            visible: true

            anchors.right: parent.right

            width: rightBorder.radius
            height: parent.height

            color: "black"
        }
    }

    Rectangle {
        id: middleBorder

        anchors.fill: parent

        color: "transparent"
        opacity: 0.7

        border.width: borderWidth
        border.color: borderColor

        layer.enabled: true
        layer.effect: OpacityMask {
            anchors.fill: middleBorder

            maskSource: middleBorderMask
            invert: true
        }
    }
}

