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

PillBox {
    id: modeRocker

    signal modeTapped(int mode)

    property int selectedMode: -1
    property bool showLabels: true
    property bool showIcons: false

    property string firstToolTipText: ""
    property string secondToolTipText: ""

    property alias firstLabelText: firstLabel.text
    property alias firstIconSource: firstIcon.source

    property alias secondLabelText: secondLabel.text
    property alias secondIconSource: secondIcon.source

    property alias firstModeEnabled: firstRow.enabled
    property alias secondModeEnabled: secondRow.enabled

    property Item foo: foo

    implicitWidth: contents.implicitWidth + Kirigami.Units.largeSpacing * 4
    height: Math.max(Kirigami.Units.iconSizes.small, fontMetrics.height) + Kirigami.Units.largeSpacing * 2

    Kirigami.Theme.colorSet: Kirigami.Theme.Button

    Row {
        id: contents

        x: Kirigami.Units.largeSpacing * 2

        height: parent.height

        spacing: Kirigami.Units.largeSpacing

        Item {
            id: foo

            width: childrenRect.width
            height: childrenRect.height
        }

        Row {
            id: firstRow

            height: parent.height

            property alias hovered: firstHoverHandler.hovered

            spacing: Kirigami.Units.largeSpacing

            Kirigami.Icon {
                id: firstIcon

                visible: modeRocker.showIcons && source

                anchors.verticalCenter: parent.verticalCenter

                width: visible ? Kirigami.Units.iconSizes.small : 0
                height: width

                color: (firstHoverHandler.hovered ? Kirigami.Theme.hoverColor
                    : ((modeRocker.selectedMode == 0) ? Kirigami.Theme.highlightColor : "white"))
                smooth: true
                isMask: true

                Behavior on color { ColorAnimation { duration: Kirigami.Units.shortDuration } }
            }

            QQC2.Label {
                id: firstLabel

                visible: modeRocker.showLabels && text

                anchors.verticalCenter: parent.verticalCenter

                width: visible ? Math.max(Math.round(firstLabel.paintedWidth), Math.round(secondLabel.paintedWidth)) : 0

                font.bold: true
                color: (firstHoverHandler.hovered ? Kirigami.Theme.hoverColor
                    : ((modeRocker.selectedMode == 0) ? Kirigami.Theme.highlightColor : "white"))

                horizontalAlignment: Text.AlignHCenter

                Behavior on color { ColorAnimation { duration: Kirigami.Units.shortDuration } }
            }

            HoverHandler {
                id: firstHoverHandler
            }

            TapHandler {
                id: firstTapHandler

                enabled: !page.touched

                gesturePolicy: TapHandler.ReleaseWithinBounds

                onTapped: modeRocker.modeTapped(0)
            }

            QQC2.ToolTip.visible: modeRocker.visible && firstHoverHandler.hovered && !firstTapHandler.pressed
            QQC2.ToolTip.delay: Kirigami.Units.toolTipDelay
            QQC2.ToolTip.text: firstToolTipText
        }

        PillBoxSeparator {}

        Row {
            id: secondRow

            height: parent.height

            property alias hovered: secondHoverHandler.hovered

            spacing: Kirigami.Units.largeSpacing

            Kirigami.Icon {
                id: secondIcon

                visible: firstIcon.visible

                anchors.verticalCenter: parent.verticalCenter

                width: firstIcon.width
                height: width

                color: (secondHoverHandler.hovered ? Kirigami.Theme.hoverColor
                    : ((modeRocker.selectedMode == 1) ? Kirigami.Theme.highlightColor : "white"))
                smooth: true
                isMask: true

                Behavior on color { ColorAnimation { duration: Kirigami.Units.shortDuration } }
            }

            QQC2.Label {
                id: secondLabel

                visible: firstLabel.visible

                anchors.verticalCenter: parent.verticalCenter

                width: firstLabel.width

                font.bold: true
                color: (secondHoverHandler.hovered ? Kirigami.Theme.hoverColor
                    : ((modeRocker.selectedMode == 1) ? Kirigami.Theme.highlightColor : "white"))

                horizontalAlignment: Text.AlignHCenter

                Behavior on color { ColorAnimation { duration: Kirigami.Units.shortDuration } }
            }

            HoverHandler {
                id: secondHoverHandler
            }

            TapHandler {
                id: secondTapHandler

                enabled: !page.touched

                gesturePolicy: TapHandler.ReleaseWithinBounds

                onTapped: modeRocker.modeTapped(1)
            }

            QQC2.ToolTip.visible: modeRocker.visible && secondHoverHandler.hovered && !secondTapHandler.pressed
            QQC2.ToolTip.delay: Kirigami.Units.toolTipDelay
            QQC2.ToolTip.text: secondToolTipText
        }
    }

    Component.onCompleted: (x = globalDrawer.modal
        ? (globalDrawer.handle.x + globalDrawer.handle.width + leftPillBox.y) : leftPillBox.y)
}
