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
import QtQuick.Controls 2.12 as QQC2

import org.kde.kirigami 2.6 as Kirigami

import org.kde.kirogi 0.1 as Kirogi
import org.kde.kirogi.video 0.1 as KirogiVideo

Kirigami.Page {
    id: page

    LayoutMirroring.enabled: false
    LayoutMirroring.childrenInherit: true

    readonly property int yardstick: Math.min(parent.width, parent.height)
    readonly property bool touched: leftTouchPoint.active || rightTouchPoint.active

    property alias gamepad: gamepadLoader.item

    leftPadding: 0
    rightPadding: 0
    topPadding: 0
    bottomPadding: 0

    Connections {
        target: kirogi

        onCurrentPageChanged: updatePilotingState()
        onCurrentVehicleChanged: updatePilotingState()
        onReadyChanged: updatePilotingState()
    }

    function updatePilotingState() {
        var vehicle = kirogi.currentVehicle;

        if (!vehicle || !vehicle.ready) {
            return;
        }

        vehicle.setPiloting(kirogi.currentPage == page);

        if (kirogi.currentPage == page && !kirogi.currentVehicle.videoStreamEnabled
            && vehicle.isActionSupported(Kirogi.AbstractVehicle.ToggleVideoStream)) {
            vehicle.requestEnableVideoStream(true);
        }
    }

    function setNewStickPosition() {
        if (kirogi.currentVehicle) {
            kirogi.currentVehicle.pilot(rightDPad.axisX, rightDPad.axisY, leftDPad.axisX, leftDPad.axisY);
        }
    }

    VideoElement {
        anchors.fill: parent
    }

    // FIXME TODO: This is a workaround around the org.kde.desktop+Breeze style engine
    // hijacking drag on the window.
    TapHandler {
        enabled: !Kirigami.Settings.isMobile
    }

    Item {
        anchors.left: parent.left

        width: parent.width / 2
        height: parent.height


        PointHandler {
            id: leftTouchPoint

            enabled: inputMode.selectedMode == 0 || kirogiSettings.alwaysShowDPads

            grabPermissions: PointerHandler.ApprovesTakeOverByAnything | PointerHandler.ApprovesCancellation
        }
    }

    Item {
        anchors.right: parent.right

        width: parent.width / 2
        height: parent.height

        PointHandler {
            id: rightTouchPoint

            enabled: inputMode.selectedMode == 0 || kirogiSettings.alwaysShowDPads
        }
    }

    TouchButton {
        id: leftButton

        anchors.top: leftPillBox.bottom
        anchors.topMargin: Math.round(leftPillBox.y * 1.5)
        anchors.left: parent.left
        anchors.leftMargin: leftPillBox.y

        icon: kirogi.currentVehicle ? kirogi.currentVehicle.iconName : "uav"
        toolTipText: i18nc("%1 = Keyboard shortcut", "Drone (%1)", vehiclePageAction.shortcut)

        onTapped: switchApplicationPage(vehiclePage)
    }

    PillBox {
        id: launchButton

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: Kirigami.Units.smallSpacing

        width: Math.min(launchButtonLabel.implicitWidth + Kirigami.Units.smallSpacing * 4,
            rightPillBox.x - leftPillBox.x - leftPillBox.width - (leftPillBox.x * 2))
        height: 2 * Math.round((leftPillBox.height * 1.12) / 2);

        readonly property var __color: {
            if (launchButtonMouseArea.containsMouse) {
                return Kirigami.Theme.hoverColor;
            }

            if (kirogi.connected) {
                if (kirogi.flying) {
                    return "red";
                } else if (kirogi.ready) {
                    return "green";
                } else {
                    return "yellow";
                }
            }

            return "red";
        }

        backgroundColor: "dark" + __color
        backgroundOpacity: 0.4

        borderWidth: 2
        borderRadius: height / 4
        borderColor: launchButtonLabel.color

        Text {
            id: launchButtonLabel

            anchors.fill: parent

            font.pixelSize: parent.height * 0.7
            font.bold: true
            color: launchButton.__color

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            elide: Text.ElideRight

            text: {
                if (kirogi.connected) {
                    if (kirogi.flying) {
                        return i18n("LAND");
                    } else if (kirogi.ready) {
                        return i18n("TAKE OFF");
                    } else {
                        return i18n("PREPARING")
                    }
                }

                return i18n("DISCONNECTED");
            }

            Behavior on color {
                enabled: !launchButtonMouseArea.pressed

                ColorAnimation { duration: Kirigami.Units.shortDuration }
            }
        }

        MouseArea {
            id: launchButtonMouseArea

            anchors.fill: parent

            enabled: kirogi.ready
            hoverEnabled: enabled

            onClicked: {
                if (kirogi.flying) {
                    kirogi.currentVehicle.requestLand();
                } else {
                    kirogi.currentVehicle.requestTakeOff();
                }
            }
        }
    }

    TouchButton {
        id: rightButton

        anchors.top: rightPillBox.bottom
        anchors.topMargin: leftButton.anchors.topMargin
        anchors.right: parent.right
        anchors.rightMargin: leftButton.anchors.leftMargin

        icon: "map-flat"
        toolTipText: i18nc("%1 = Keyboard shortcut", "Navigation Map (%1)", navigationMapPageAction.shortcut)

        onTapped: switchApplicationPage(navigationMapPage)
    }

    ModeRocker {
        id: flightMode

        enabled: kirogi.ready

        anchors.left: parent.left
        anchors.verticalCenter: shotButton.verticalCenter

        selectedMode: {
            if (kirogi.ready) {
                if (kirogi.currentVehicle.performanceMode == Kirogi.AbstractVehicle.FilmPerformance) {
                    return 0;
                } else if (kirogi.currentVehicle.performanceMode == Kirogi.AbstractVehicle.SportPerformance) {
                    return 1;
                }
            }

            return -1;
        }

        firstLabelText: i18n("FILM")
        firstToolTipText: i18n("Fly Slow")

        secondLabelText: i18n("SPORT")
        secondToolTipText: i18n("Fly Fast")

        onModeTapped: {
            if (selectedMode == mode) {
                return;
            }

            if (mode == 0) {
                kirogi.currentVehicle.requestPerformanceMode(Kirogi.AbstractVehicle.FilmPerformance);
            } else if (mode == 1) {
                kirogi.currentVehicle.requestPerformanceMode(Kirogi.AbstractVehicle.SportPerformance);
            }
        }

        Component.onCompleted: {
            var handleWidth = kirogi.LayoutMirroring.enabled ? parent.width - globalDrawer.handle.x
                : globalDrawer.handle.x + globalDrawer.handle.width;
            anchors.leftMargin = globalDrawer.modal ? handleWidth + leftPillBox.y : leftPillBox.y;
        }
    }

    ModeRocker {
        id: inputMode

        visible: gamepad && gamepad.connected && !kirogiSettings.alwaysShowDPads

        anchors.verticalCenter: shotButton.verticalCenter
        anchors.left: flightMode.right
        anchors.leftMargin: leftPillBox.y

        firstLabelText: i18n("SCREEN")
        firstIconSource: Kirigami.Settings.isMobile ? "phone-symbolic" : "computer-symbolic"
        firstToolTipText: i18n("Use Virtual D-Pads")

        secondLabelText: i18n("CONTROLLER")
        secondIconSource: "folder-games-symbolic"
        secondToolTipText: i18n("Use Gamepad Controller")

        showLabels: false
        showIcons: true

        // If there is no gamepad connected, the user will not be able to change back to virtual joystick mode.
        // It's necessary to force the virtual joystick as default if no joystick is connected.
        selectedMode: gamepad.connected ? kirogiSettings.lastInputMode : 0

        onModeTapped: {
            selectedMode = mode;
            kirogiSettings.lastInputMode = selectedMode;
        }

    }

    ModeRocker {
        id: shotMode

        enabled: kirogi.ready

        anchors.right: shotButton.right
        anchors.rightMargin: shotButton.width / 2
        anchors.verticalCenter: shotButton.verticalCenter

        width: (implicitWidth + shotButton.width / 2) - Kirigami.Units.largeSpacing

        property int requestedMode: 0

        firstModeEnabled: enabled && kirogi.currentVehicle.isActionSupported(Kirogi.AbstractVehicle.RecordVideo)
        secondModeEnabled: enabled && kirogi.currentVehicle.isActionSupported(Kirogi.AbstractVehicle.TakePicture)

        firstLabelText: i18n("VIDEO")
        firstIconSource: "emblem-videos-symbolic"
        firstToolTipText: i18n("Record Videos")

        secondLabelText: i18n("PHOTO")
        secondIconSource: "emblem-photos-symbolic"
        secondToolTipText: i18n("Take Photos")

        showLabels: false
        showIcons: true

        selectedMode: kirogi.ready ? requestedMode : -1

        onModeTapped: requestedMode = mode
    }

    TouchButton {
        id: shotButton

        enabled: {
            if (!kirogi.ready) {
                return false;
            }

            if ((shotMode.selectedMode == 0 && !kirogi.currentVehicle.isActionSupported(Kirogi.AbstractVehicle.RecordVideo))
                || (shotMode.selectedMode == 1 && (!kirogi.currentVehicle.isActionSupported(Kirogi.AbstractVehicle.TakePicture)
                || !kirogi.currentVehicle.canTakePicture))) {
                return false;
            }

            return true;
        }

        anchors.right: parent.right
        anchors.rightMargin: flightMode.anchors.leftMargin
        anchors.bottom: parent.bottom
        anchors.bottomMargin: launchButton.anchors.topMargin

        icon: "media-record-symbolic"
        iconColor: shotMode.selectedMode == 0 && (kirogi.currentVehicle && kirogi.currentVehicle.isRecordingVideo) ? "red" : "white"
        toolTipText: {
            if (shotMode.selectedMode) {
                return i18n("Take Photo");
            } else if (kirogi.currentVehicle && kirogi.currentVehicle.isRecordingVideo) {
                return i18n("Stop Recording Video");
            }

            return i18n("Record Video");
        }

        onTapped: {
            if (!kirogi.ready) {
                return;
            }

            if (shotMode.selectedMode == 0) {
                kirogi.currentVehicle.requestAction(Kirogi.AbstractVehicle.RecordVideo);
            } else if (shotMode.selectedMode == 1) {
                kirogi.currentVehicle.requestAction(Kirogi.AbstractVehicle.TakePicture);
            }
        }
    }

    TouchDPad {
        id: leftDPad

        visible: inputMode.selectedMode == 0 || kirogiSettings.alwaysShowDPads || (gamepad && !gamepad.connected)

        anchors.left: parent.left
        anchors.leftMargin: yardstick * 0.18
        anchors.bottom: parent.bottom
        anchors.bottomMargin: yardstick * 0.20

        width: Math.min(yardstick * 0.45, parent.width / 4)
        height: width

        leftIcon: "edit-undo"
        leftToolTipText: i18n("Turn Left")
        rightIcon: "edit-redo"
        rightToolTipText: i18n("Turn Right")
        topIcon: "arrow-up"
        topToolTipText: i18n("Move Up")
        bottomIcon: "arrow-down"
        bottomToolTipText: i18n("Move Down")

        onXChanged: moved = aboutToMove

        onAxisXChanged: setNewStickPosition()
        onAxisYChanged: setNewStickPosition()

        touchPos: {
            if (moved && leftTouchPoint) {
                var xDifference = 0;

                if (leftTouchPoint.point.scenePosition.x > leftTouchPoint.point.scenePressPosition.x) {
                    xDifference = xDifference + Math.abs(leftTouchPoint.point.scenePressPosition.x - leftTouchPoint.point.scenePosition.x);
                } else {
                    xDifference = xDifference - Math.abs(leftTouchPoint.point.scenePressPosition.x - leftTouchPoint.point.scenePosition.x)
                }

                var x = leftDPad.x + leftDPad.width / 2 + xDifference;

                var yDifference = 0;

                if (leftTouchPoint.point.scenePosition.y > leftTouchPoint.point.scenePressPosition.y) {
                    yDifference = yDifference + Math.abs(leftTouchPoint.point.scenePressPosition.y - leftTouchPoint.point.scenePosition.y);
                } else {
                    yDifference = yDifference - Math.abs(leftTouchPoint.point.scenePressPosition.y - leftTouchPoint.point.scenePosition.y)
                }

                var y = leftDPad.y + leftDPad.height / 2 + yDifference;

                return parent.mapToItem(leftDPad, x, y);
            }

            return null;
        }

        states: [
            State {
                name: "inactive"

                AnchorChanges {
                    target: leftDPad

                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                }

                PropertyChanges {
                    target: leftDPad

                    aboutToMove: false
                    moved: false
                }
            },
            State {
                name: "active"

                when: leftTouchPoint.active

                AnchorChanges {
                    target: leftDPad

                    anchors.left: undefined
                    anchors.bottom: undefined
                }

                PropertyChanges {
                    target: leftDPad

                    aboutToMove: true
                    x: Math.min((parent.width / 2) - width, Math.max(0, leftTouchPoint.point.scenePressPosition.x - width / 2))
                    y: Math.min(parent.height - height, Math.max(0, leftTouchPoint.point.scenePressPosition.y - height / 2))
                }
            }
        ]
    }

    TouchDPad {
        id: rightDPad

        visible: leftDPad.visible

        width: leftDPad.height
        height: width

        anchors.right: parent.right
        anchors.rightMargin: leftDPad.anchors.leftMargin
        anchors.bottom: parent.bottom
        anchors.bottomMargin: leftDPad.anchors.bottomMargin

        leftIcon: "go-previous"
        leftToolTipText: i18n("Move Left")
        rightIcon: "go-next"
        rightToolTipText: i18n("Move Right")
        topIcon: "go-up"
        topToolTipText: i18n("Move Forward")
        bottomIcon: "go-down"
        bottomToolTipText: i18n("Move Backward")

        onXChanged: moved = aboutToMove

        onAxisXChanged: setNewStickPosition()
        onAxisYChanged: setNewStickPosition()

        touchPos: {
            if (moved && rightTouchPoint.active) {
                var xDifference = 0;

                if (rightTouchPoint.point.scenePosition.x > rightTouchPoint.point.scenePressPosition.x) {
                    xDifference = xDifference + Math.abs(rightTouchPoint.point.scenePressPosition.x - rightTouchPoint.point.scenePosition.x);
                } else {
                    xDifference = xDifference - Math.abs(rightTouchPoint.point.scenePressPosition.x - rightTouchPoint.point.scenePosition.x)
                }

                var x = rightDPad.x + rightDPad.width / 2 + xDifference;

                var yDifference = 0;

                if (rightTouchPoint.point.scenePosition.y > rightTouchPoint.point.scenePressPosition.y) {
                    yDifference = yDifference + Math.abs(rightTouchPoint.point.scenePressPosition.y - rightTouchPoint.point.scenePosition.y);
                } else {
                    yDifference = yDifference - Math.abs(rightTouchPoint.point.scenePressPosition.y - rightTouchPoint.point.scenePosition.y)
                }

                var y = rightDPad.y + rightDPad.height / 2 + yDifference;

                return parent.mapToItem(rightDPad, x, y);
            }

            return null;
        }

        states: [
            State {
                name: "inactive"

                AnchorChanges {
                    target: rightDPad

                    anchors.left: parent.right
                    anchors.bottom: parent.bottom
                }

                PropertyChanges {
                    target: rightDPad

                    aboutToMove: false
                    moved: false
                }
            },
            State {
                name: "active"

                when: rightTouchPoint.active
                AnchorChanges {
                    target: rightDPad

                    anchors.right: undefined
                    anchors.bottom: undefined
                }

                PropertyChanges {
                    target: rightDPad

                    aboutToMove: true
                    x: Math.max(parent.width / 2, Math.min(parent.width - width, rightTouchPoint.point.scenePressPosition.x - width / 2))
                    y: Math.min(parent.height - height, Math.max(0, rightTouchPoint.point.scenePressPosition.y - height / 2))
                }
            }
        ]
    }

    PillBox {
        id: leftPillBox

        anchors.verticalCenter: launchButton.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: y

        width: leftPillBoxContents.implicitWidth + Kirigami.Units.largeSpacing * 4
        height: 2 * Math.round((Math.max(Kirigami.Units.iconSizes.small, fontMetrics.height) + Kirigami.Units.smallSpacing * 3) / 2);

        Row {
            id: leftPillBoxContents

            anchors.horizontalCenter: parent.horizontalCenter

            height: parent.height

            spacing: Kirigami.Units.largeSpacing

            Kirigami.Icon {
                anchors.verticalCenter: parent.verticalCenter

                width: Kirigami.Units.iconSizes.small
                height: width

                color: "white"
                smooth: true
                isMask: true

                source: "speedometer"
            }

            QQC2.Label {
                anchors.verticalCenter: parent.verticalCenter

                width: kirogi.currentVehicle ? Math.round(Math.max(implicitWidth, fontMetrics.tightBoundingRect(i18n("%1 m/s", "0.0")).width))
                    : Math.round(implicitWidth)

                color: "white"

                horizontalAlignment: Text.AlignRight

                text: {
                    if(kirogi.currentVehicle) {
                        return i18n("%1 m/s", kirogi.flying ? kirogi.currentVehicle.speed : "0");
                    }

                    return i18n("– m/s");
                }
            }

            PillBoxSeparator {}

            Kirigami.Icon {
                anchors.verticalCenter: parent.verticalCenter

                width: Kirigami.Units.iconSizes.small
                height: width

                color: "white"
                smooth: true
                isMask: true

                source: "kruler-west"
            }

            QQC2.Label {
                anchors.verticalCenter: parent.verticalCenter

                width: kirogi.ready ? Math.round(Math.max(implicitWidth, fontMetrics.tightBoundingRect(i18n("%1 m", "0.0")).width))
                    : Math.round(implicitWidth)

                color: "white"

                horizontalAlignment: Text.AlignRight

                text: {
                    if (kirogi.currentVehicle) {
                        return i18n("%1 m", kirogi.currentVehicle.altitude.toFixed(2))
                    }

                    return i18n("– m");
                }
            }

            PillBoxSeparator {}

            Kirigami.Icon {
                anchors.verticalCenter: parent.verticalCenter

                width: Kirigami.Units.iconSizes.small
                height: width

                color: "white"
                smooth: true
                isMask: true

                source: "kruler-south"
            }

            QQC2.Label {
                anchors.verticalCenter: parent.verticalCenter

                width: kirogi.currentVehicle ? Math.round(Math.max(implicitWidth, fontMetrics.tightBoundingRect(i18n("%1 m", "0.0")).width))
                    : Math.round(implicitWidth)

                color: "white"

                horizontalAlignment: Text.AlignRight

                text: {
                    if (kirogi.currentVehicle && kirogi.currentVehicle.distance >= 0) {
                        return i18n("%1 m", kirogi.currentVehicle.distance.toFixed(1));
                    }

                    if (kirogi.distance !== 0) {
                        return i18n("%1 m", kirogi.distance.toFixed(1));
                    }

                    return i18n("– m");
                }
            }
        }
    }

    PillBox {
        id: rightPillBox

        anchors.verticalCenter: launchButton.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: leftPillBox.anchors.leftMargin

        width: rightPillBoxContents.implicitWidth + Kirigami.Units.largeSpacing * 4
        height: leftPillBox.height

       Row {
           id: rightPillBoxContents

            x: Kirigami.Units.largeSpacing

            anchors.horizontalCenter: parent.horizontalCenter

            height: parent.height

            spacing: Kirigami.Units.largeSpacing

            Kirigami.Icon {
                anchors.verticalCenter: parent.verticalCenter

                width: Kirigami.Units.iconSizes.small
                height: width

                color: "white"
                smooth: true
                isMask: true

                source: "clock"
            }

            QQC2.Label {
                anchors.verticalCenter: parent.verticalCenter

                width: kirogi.currentVehicle ? Math.round(Math.max(implicitWidth, fontMetrics.tightBoundingRect(i18n("%1 m", "0:00")).width))
                    : Math.round(implicitWidth)

                color: "white"

                horizontalAlignment: Text.AlignRight

                text: {
                    if (kirogi.ready) {
                        var time = kirogi.flying ? kirogi.currentVehicle.flightTime : 0;
                        return i18n("%1 min", (time - (time %= 60)) / 60 + (9 < time ?':':':0') + time);
                    }

                    return i18n("– min");
                }
            }

            PillBoxSeparator {}

            Kirigami.Icon {
                anchors.verticalCenter: parent.verticalCenter

                width: Kirigami.Units.iconSizes.small
                height: width

                color: "white"
                smooth: true
                isMask: kirogi.currentVehicle

                source: {
                    if (kirogi.currentVehicle) {
                        if (kirogi.currentVehicle.signalStrength === 0) {
                            return "network-wireless-connected-00";
                        } else if (kirogi.currentVehicle.signalStrength < 25) {
                            return "network-wireless-connected-25";
                        } else if (kirogi.currentVehicle.signalStrength < 50) {
                            return "network-wireless-connected-50";
                        } else if (kirogi.currentVehicle.signalStrength < 75) {
                            return "network-wireless-connected-75";
                        } else if (kirogi.currentVehicle.signalStrength <= 100) {
                            return "network-wireless-connected-100";
                        }
                    }

                    if (kirogi.connected) {
                        return "network-wireless-acquiring";
                    }

                    return "network-wireless-disconnected";
                }
            }

            QQC2.Label {
                anchors.verticalCenter: parent.verticalCenter

                width: kirogi.currentVehicle ? Math.round(Math.max(implicitWidth, fontMetrics.tightBoundingRect(i18n("%1%", 00)).width))
                    : Math.round(implicitWidth)

                color: "white"

                horizontalAlignment: Text.AlignRight

                text: kirogi.currentVehicle ? i18n("%1%", kirogi.currentVehicle.signalStrength) : i18n("N/A")
            }

            PillBoxSeparator {}

            Kirigami.Icon {
                anchors.verticalCenter: parent.verticalCenter

                width: Kirigami.Units.iconSizes.small
                height: width

                color: "white"
                smooth: true
                isMask: kirogi.currentVehicle

                source: {
                    if (kirogi.currentVehicle) {
                        var roundedBatteryLevel = Math.round(kirogi.currentVehicle.batteryLevel / 10);
                        return "battery-" + roundedBatteryLevel.toString().padStart(2, "0") + "0";
                    }

                    return "battery-missing";
                }
            }

            QQC2.Label {
                anchors.verticalCenter: parent.verticalCenter

                width: kirogi.currentVehicle ? Math.round(Math.max(implicitWidth, fontMetrics.tightBoundingRect(i18n("%1%", 00)).width))
                    : Math.round(implicitWidth)

                color: "white"

                horizontalAlignment: Text.AlignRight

                text: kirogi.currentVehicle ?  i18n("%1%", kirogi.currentVehicle.batteryLevel) : i18n("N/A")
            }
        }
    }

    PitchBar {
        id: pitchBar

        anchors.centerIn: parent

        width: 25
        height: parent.height * 0.6

        pitch: kirogi.currentVehicle ? kirogi.currentVehicle.pitch * (180/Math.PI) : 0.0
    }

    VirtualHorizon {
        id: virtualHorizon

        anchors.centerIn: pitchBar

        width: 110

        roll: kirogi.currentVehicle ? kirogi.currentVehicle.roll * (180/Math.PI) : 0
    }

    YawBar {
        id: yawBar

        anchors.bottom: inputMode.visible ? inputMode.top : parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width / 2

        tickWidth: 10

        yaw: kirogi.currentVehicle ? kirogi.currentVehicle.yaw * (180 / Math.PI) : 0
    }

    VehicleActionsDrawer {
        enabled: kirogi.currentPage == page // FIXME TODO: Why doesn't come down from page.enabled?

        width: Kirigami.Units.gridUnit * 19

        edge: kirogi.LayoutMirroring.enabled ? Qt.LeftEdge : Qt.RightEdge

        handleClosedIcon.source: "configure"
    }

    Loader {
        id: gamepadLoader

        source: "Gamepad.qml"
        asynchronous: true

        // FIXME TODO: QtGamepad currently causes performance problems on
        // Android (blocking multi-tasking) that need to be investigated.
        active: !Kirigami.Settings.isMobile
    }
}
