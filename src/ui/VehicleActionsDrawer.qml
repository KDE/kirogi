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
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12 as QQC2

import org.kde.kirigami 2.6 as Kirigami

import org.kde.kirogi 0.1 as Kirogi

// FIXME TODO: For lack of time this is currently written in an unrolled/static style,
// but the drawer contents should be generated procedurally later to be more extensible.

Kirigami.OverlayDrawer {
    id: drawer

    readonly property var __categoryActions: [flipAction, geofenceAction, performanceAction]

    topPadding: 0
    bottomPadding: 0
    leftPadding: 0
    rightPadding: 0

    dragMargin: 0 // FIXME TODO: Interferes with shell buttons on Android in landscape orientation.

    onDrawerOpenChanged: {
        if (!drawerOpen) {
            toggleCategory();
        }
    }

    function toggleCategory(category) {
        if (category && !category.collapsed) {
            category.collapsed = true;
            scrollView.scrollToBottom();
        } else {
            for (var i = 0; i < __categoryActions.length; ++i) {
                if (__categoryActions[i] != category) {
                    __categoryActions[i].collapsed = true;
                }
            }

            if (category) {
                category.collapsed = false;
            }
        }
    }

    Connections {
        target: kirogi

        onCurrentPageChanged: {
            if (kirogi.currentPage != flightControlsPage) {
                drawer.drawerOpen = false;
            }
        }
    }

    QQC2.ScrollView {
        id: scrollView

        states: State {
            when: !Kirigami.Settings.isMobile
            
            AnchorChanges {
                target: scrollView
                anchors.top: parent.top
            }
        }

        width: parent.width
        height: parent.height

        contentHeight: actionsColumn.implicitHeight
        contentWidth: drawer.width - scrollBarWidth

        onContentHeightChanged: {
            if (contentHeight < parent.height) {
                return;
            }

            for (var i = 0; i < __categoryActions.length; ++i) {
                var category = __categoryActions[i];

                if (!category.collapsed) {
                    Qt.callLater(scrollView.scrollTo, category);
                }
            }
        }

        function scrollTo(item) {
            QQC2.ScrollBar.vertical.position = item.y / contentHeight;
        }

        function scrollToBottom() {
            QQC2.ScrollBar.vertical.position = 1.0;
        }

        property int scrollBarWidth: QQC2.ScrollBar.vertical && QQC2.ScrollBar.vertical.visible
            ? QQC2.ScrollBar.vertical.width : 0

        Column {
            id: actionsColumn

            width: drawer.width - scrollView.scrollBarWidth
            height: parent.height

            spacing: Kirigami.Units.smallSpacing

            Kirigami.BasicListItem {
                id: flatTrimAction

                enabled: (kirogi.currentVehicle
                    && kirogi.currentVehicle.isActionSupported(Kirogi.AbstractVehicle.FlatTrim))

                property bool collapsed: true

                separatorVisible: false
                icon: "format-align-vertical-bottom"
                label: i18n("Flat Trim")

                onClicked: kirogi.currentVehicle.requestAction(Kirogi.AbstractVehicle.FlatTrim)
            }

            Kirigami.BasicListItem {
                id: flipAction

                property bool collapsed: true

                separatorVisible: false
                icon: "chronometer-reset"
                label: i18n("Flip")

                onClicked: toggleCategory(flipAction)

                Kirigami.Icon {
                    Layout.alignment: Qt.AlignVCenter
                    Layout.preferredHeight: Kirigami.Units.iconSizes.smallMedium
                    Layout.preferredWidth: Layout.preferredHeight

                    source: flipAction.collapsed ? "go-down-symbolic" : "go-up-symbolic"
                    isMask: true
                }
            }

            Kirigami.AbstractListItem {
                id: flipForwardAction

                visible: !flipAction.collapsed

                enabled: (kirogi.currentVehicle && kirogi.currentVehicle.ready
                    && kirogi.currentVehicle.isActionSupported(Kirogi.AbstractVehicle.FlipForward))

                implicitHeight: flipAction.height

                leftPadding: Kirigami.Units.largeSpacing * 4

                separatorVisible: false

                contentItem: QQC2.Label { text: i18n("Flip Forward") }

                onClicked: kirogi.currentVehicle.requestAction(Kirogi.AbstractVehicle.FlipForward)
            }

            Kirigami.AbstractListItem {
                id: flipBackwardAction

                visible: !flipAction.collapsed

                enabled: (kirogi.currentVehicle && kirogi.currentVehicle.ready
                    && kirogi.currentVehicle.isActionSupported(Kirogi.AbstractVehicle.FlipBackward))

                implicitHeight: flipAction.height

                leftPadding: Kirigami.Units.largeSpacing * 4

                separatorVisible: false

                contentItem: QQC2.Label { text: i18n("Flip Backward") }

                onClicked: kirogi.currentVehicle.requestAction(Kirogi.AbstractVehicle.FlipBackward)
            }

            Kirigami.AbstractListItem {
                id: flipLeftAction

                visible: !flipAction.collapsed

                enabled: (kirogi.currentVehicle && kirogi.currentVehicle.ready
                    && kirogi.currentVehicle.isActionSupported(Kirogi.AbstractVehicle.FlipLeft))

                implicitHeight: flipAction.height

                leftPadding: Kirigami.Units.largeSpacing * 4

                separatorVisible: false

                contentItem: QQC2.Label { text: i18n("Flip Left") }

                onClicked: kirogi.currentVehicle.requestAction(Kirogi.AbstractVehicle.FlipLeft)
            }

            Kirigami.AbstractListItem {
                id: flipRightAction

                visible: !flipAction.collapsed

                enabled: (kirogi.currentVehicle && kirogi.currentVehicle.ready
                    && kirogi.currentVehicle.isActionSupported(Kirogi.AbstractVehicle.FlipRight))

                implicitHeight: flipAction.height

                leftPadding: Kirigami.Units.largeSpacing * 4

                separatorVisible: false

                contentItem: QQC2.Label { text: i18n("Flip Right") }

                onClicked: kirogi.currentVehicle.requestAction(Kirogi.AbstractVehicle.FlipRight)
            }

            Kirigami.BasicListItem {
                id: geofenceAction

                property bool collapsed: true

                separatorVisible: false
                icon: "map-flat"
                label: {
                    if (kirogi.currentVehicle && kirogi.currentVehicle.ready) {
                        var status;

                        if (kirogi.currentVehicle.geofence) {
                            status = i18n("Enabled");
                        } else {
                            status = i18n("Disabled");
                        }

                        return i18n("Geofence: %1", status);
                    }

                    return i18n("Geofence");
                }

                onClicked: toggleCategory(geofenceAction)

                Kirigami.Icon {
                    Layout.alignment: Qt.AlignVCenter
                    Layout.preferredHeight: Kirigami.Units.iconSizes.smallMedium
                    Layout.preferredWidth: Layout.preferredHeight

                    source: geofenceAction.collapsed ? "go-down-symbolic" : "go-up-symbolic"
                    isMask: true
                }
            }

            Kirigami.AbstractListItem {
                visible: !geofenceAction.collapsed

                enabled: (kirogi.currentVehicle
                    && kirogi.currentVehicle.isActionSupported(Kirogi.AbstractVehicle.ToggleGeofence))

                implicitHeight: geofenceAction.height

                leftPadding: Kirigami.Units.largeSpacing * 4

                separatorVisible: false

                contentItem: QQC2.Label {
                    verticalAlignment: Text.AlignVCenter

                    text: {
                        if (kirogi.currentVehicle && kirogi.currentVehicle.geofence) {
                            return i18n("Disable Geofence");
                        }

                        return i18n("Enable Geofence");
                    }
                }

                onClicked: kirogi.currentVehicle.requestAction(Kirogi.AbstractVehicle.ToggleGeofence)
            }

            Kirigami.AbstractListItem {
                visible: !geofenceAction.collapsed

                enabled: (kirogi.currentVehicle
                    && kirogi.currentVehicle.isActionSupported(Kirogi.AbstractVehicle.SetMaxAltitude))

                leftPadding: Kirigami.Units.largeSpacing * 4

                separatorVisible: false
                supportsMouseEvents: false

                contentItem: GridLayout {
                    columns: 2
                    rowSpacing: Kirigami.Units.smallSpacing
                    columnSpacing: rowSpacing

                    QQC2.Label {
                        Layout.columnSpan: 2

                        text: i18n("Maximum altitude:")
                    }

                    QQC2.Label {
                        Layout.minimumWidth: Kirigami.Units.gridUnit * 3
                        Layout.maximumWidth: Kirigami.Units.gridUnit * 3

                        text: i18n("%1 m", (kirogi.currentVehicle && kirogi.currentVehicle.ready) ? kirogi.currentVehicle.maxAltitude : 0)
                    }

                    QQC2.Slider {
                        Layout.fillWidth: true

                        live: true

                        from: (kirogi.currentVehicle && kirogi.currentVehicle.ready) ? kirogi.currentVehicle.maxAltitudeMin : 0
                        to: (kirogi.currentVehicle && kirogi.currentVehicle.ready) ? kirogi.currentVehicle.maxAltitudeMax : 0

                        onPressedChanged: {
                            if (!pressed && value != kirogi.currentVehicle.maxAltitude) {
                                kirogi.currentVehicle.requestMaxAltitude(Math.round(value));
                            }
                        }

                        Connections {
                            target: kirogi.currentVehicle

                            onMaxDistanceChanged: parent.value = kirogi.currentVehicle.maxAltitude
                        }

                        Rectangle {
                            visible: parent.pressed

                            z: -1

                            width: Kirigami.Units.devicePixelRatio
                            height: parent.height

                            color: Kirigami.Theme.disabledTextColor

                            onVisibleChanged: {
                                x = parent.leftPadding + (parent.visualPosition * (parent.availableWidth - width));
                            }
                        }
                    }
                }
            }

            Kirigami.AbstractListItem {
                visible: !geofenceAction.collapsed

                enabled: (kirogi.currentVehicle
                    && kirogi.currentVehicle.isActionSupported(Kirogi.AbstractVehicle.SetMaxDistance))

                leftPadding: Kirigami.Units.largeSpacing * 4

                separatorVisible: false
                supportsMouseEvents: false

                contentItem: GridLayout {
                    columns: 2
                    rowSpacing: Kirigami.Units.smallSpacing
                    columnSpacing: rowSpacing

                    QQC2.Label {
                        Layout.columnSpan: 2

                        text: i18n("Maximum distance:")
                    }

                    QQC2.Label {
                        Layout.minimumWidth: Kirigami.Units.gridUnit * 3
                        Layout.maximumWidth: Kirigami.Units.gridUnit * 3

                        text: i18n("%1 m", (kirogi.currentVehicle && kirogi.currentVehicle.ready) ? kirogi.currentVehicle.maxDistance : 0)
                    }

                    QQC2.Slider {
                        Layout.fillWidth: true

                        live: true

                        from: (kirogi.currentVehicle && kirogi.currentVehicle.ready) ? kirogi.currentVehicle.maxDistanceMin : 0
                        to: (kirogi.currentVehicle && kirogi.currentVehicle.ready) ? kirogi.currentVehicle.maxDistanceMax : 0

                        onPressedChanged: {
                            if (!pressed && value != kirogi.currentVehicle.maxDistance) {
                                kirogi.currentVehicle.requestMaxDistance(Math.round(value));
                            }
                        }

                        Connections {
                            target: kirogi.currentVehicle

                            onMaxDistanceChanged: parent.value = kirogi.currentVehicle.maxDistance
                        }

                        Rectangle {
                            visible: parent.pressed

                            z: -1

                            width: Kirigami.Units.devicePixelRatio
                            height: parent.height

                            color: Kirigami.Theme.disabledTextColor

                            onVisibleChanged: {
                                x = parent.leftPadding + (parent.visualPosition * (parent.availableWidth - width));
                            }
                        }
                    }
                }
            }

            Kirigami.BasicListItem {
                id: performanceAction

                property bool collapsed: true

                separatorVisible: false
                icon: "speedometer"
                label: {
                    if (kirogi.currentVehicle && kirogi.currentVehicle.ready) {
                        var status;

                        if (kirogi.currentVehicle.performanceMode == Kirogi.AbstractVehicle.FilmPerformance) {
                            status = i18n("Film");
                        } else if (kirogi.currentVehicle.performanceMode == Kirogi.AbstractVehicle.SportPerformance) {
                            status = i18n("Sport");
                        } else {
                            status = i18n("Custom");
                        }

                        return i18n("Performance: %1", status);
                    }

                    return i18n("Performance");
                }

                onClicked: toggleCategory(performanceAction)

                Kirigami.Icon {
                    Layout.alignment: Qt.AlignVCenter
                    Layout.preferredHeight: Kirigami.Units.iconSizes.smallMedium
                    Layout.preferredWidth: Layout.preferredHeight

                    source: performanceAction.collapsed ? "go-down-symbolic" : "go-up-symbolic" 
                    isMask: true
                }
            }

            Kirigami.AbstractListItem {
                visible: !performanceAction.collapsed

                enabled: (kirogi.currentVehicle
                    && kirogi.currentVehicle.isActionSupported(Kirogi.AbstractVehicle.SetMaxRollSpeed))

                leftPadding: Kirigami.Units.largeSpacing * 4

                separatorVisible: false
                supportsMouseEvents: false

                contentItem: GridLayout {
                    columns: 2
                    rowSpacing: Kirigami.Units.smallSpacing
                    columnSpacing: rowSpacing

                    QQC2.Label {
                        Layout.columnSpan: 2

                        text: i18n("Maximum roll speed:")
                    }

                    QQC2.Label {
                        Layout.minimumWidth: Kirigami.Units.gridUnit * 3
                        Layout.maximumWidth: Kirigami.Units.gridUnit * 3

                        text: i18n("%1 °/s", (kirogi.currentVehicle && kirogi.currentVehicle.ready) ? kirogi.currentVehicle.maxRollSpeed : 0)
                    }

                    QQC2.Slider {
                        Layout.fillWidth: true

                        live: true

                        from: (kirogi.currentVehicle && kirogi.currentVehicle.ready) ? kirogi.currentVehicle.maxRollSpeedMin : 0
                        to: (kirogi.currentVehicle && kirogi.currentVehicle.ready) ? kirogi.currentVehicle.maxRollSpeedMax : 0

                        onPressedChanged: {
                            if (!pressed && value != kirogi.currentVehicle.maxRollSpeed) {
                                kirogi.currentVehicle.requestMaxRollSpeed(Math.round(value));
                            }
                        }

                        Connections {
                            target: kirogi.currentVehicle

                            onMaxDistanceChanged: parent.value = kirogi.currentVehicle.maxRollSpeed
                        }

                        Rectangle {
                            visible: parent.pressed

                            z: -1

                            width: Kirigami.Units.devicePixelRatio
                            height: parent.height

                            color: Kirigami.Theme.disabledTextColor

                            onVisibleChanged: {
                                x = parent.leftPadding + (parent.visualPosition * (parent.availableWidth - width));
                            }
                        }
                    }
                }
            }

            Kirigami.AbstractListItem {
                visible: !performanceAction.collapsed

                enabled: (kirogi.currentVehicle
                    && kirogi.currentVehicle.isActionSupported(Kirogi.AbstractVehicle.SetMaxPitchSpeed))

                leftPadding: Kirigami.Units.largeSpacing * 4

                separatorVisible: false
                supportsMouseEvents: false

                contentItem: GridLayout {
                    columns: 2
                    rowSpacing: Kirigami.Units.smallSpacing
                    columnSpacing: rowSpacing

                    QQC2.Label {
                        Layout.columnSpan: 2

                        text: i18n("Maximum pitch speed:")
                    }

                    QQC2.Label {
                        Layout.minimumWidth: Kirigami.Units.gridUnit * 3
                        Layout.maximumWidth: Kirigami.Units.gridUnit * 3

                        text: i18n("%1 °/s", (kirogi.currentVehicle && kirogi.currentVehicle.ready) ? kirogi.currentVehicle.maxPitchSpeed : 0)
                    }

                    QQC2.Slider {
                        Layout.fillWidth: true

                        live: true

                        from: (kirogi.currentVehicle && kirogi.currentVehicle.ready) ? kirogi.currentVehicle.maxPitchSpeedMin : 0
                        to: (kirogi.currentVehicle && kirogi.currentVehicle.ready) ? kirogi.currentVehicle.maxPitchSpeedMax : 0

                        onPressedChanged: {
                            if (!pressed && value != kirogi.currentVehicle.maxPitchSpeed) {
                                kirogi.currentVehicle.requestMaxPitchSpeed(Math.round(value));
                            }
                        }

                        Connections {
                            target: kirogi.currentVehicle

                            onMaxDistanceChanged: parent.value = kirogi.currentVehicle.maxPitchSpeed
                        }

                        Rectangle {
                            visible: parent.pressed

                            z: -1

                            width: Kirigami.Units.devicePixelRatio
                            height: parent.height

                            color: Kirigami.Theme.disabledTextColor

                            onVisibleChanged: {
                                x = parent.leftPadding + (parent.visualPosition * (parent.availableWidth - width));
                            }
                        }
                    }
                }
            }

            Kirigami.AbstractListItem {
                visible: !performanceAction.collapsed

                enabled: (kirogi.currentVehicle
                    && kirogi.currentVehicle.isActionSupported(Kirogi.AbstractVehicle.SetMaxYawSpeed))

                leftPadding: Kirigami.Units.largeSpacing * 4

                separatorVisible: false
                supportsMouseEvents: false

                contentItem: GridLayout {
                    columns: 2
                    rowSpacing: Kirigami.Units.smallSpacing
                    columnSpacing: rowSpacing

                    QQC2.Label {
                        Layout.columnSpan: 2

                        text: i18n("Maximum yaw speed:")
                    }

                    QQC2.Label {
                        Layout.minimumWidth: Kirigami.Units.gridUnit * 3
                        Layout.maximumWidth: Kirigami.Units.gridUnit * 3

                        text: i18n("%1 °/s", (kirogi.currentVehicle && kirogi.currentVehicle.ready) ? kirogi.currentVehicle.maxYawSpeed : 0)
                    }

                    QQC2.Slider {
                        Layout.fillWidth: true

                        live: true

                        from: (kirogi.currentVehicle && kirogi.currentVehicle.ready) ? kirogi.currentVehicle.maxYawSpeedMin : 0
                        to: (kirogi.currentVehicle && kirogi.currentVehicle.ready) ? kirogi.currentVehicle.maxYawSpeedMax : 0

                        onPressedChanged: {
                            if (!pressed && value != kirogi.currentVehicle.maxYawSpeed) {
                                kirogi.currentVehicle.requestMaxYawSpeed(Math.round(value));
                            }
                        }

                        Connections {
                            target: kirogi.currentVehicle

                            onMaxDistanceChanged: parent.value = kirogi.currentVehicle.maxYawSpeed
                        }

                        Rectangle {
                            visible: parent.pressed

                            z: -1

                            width: Kirigami.Units.devicePixelRatio
                            height: parent.height

                            color: Kirigami.Theme.disabledTextColor

                            onVisibleChanged: {
                                x = parent.leftPadding + (parent.visualPosition * (parent.availableWidth - width));
                            }
                        }
                    }
                }
            }

            Kirigami.AbstractListItem {
                visible: !performanceAction.collapsed

                enabled: (kirogi.currentVehicle
                    && kirogi.currentVehicle.isActionSupported(Kirogi.AbstractVehicle.SetMaxGazSpeed))

                leftPadding: Kirigami.Units.largeSpacing * 4

                separatorVisible: false
                supportsMouseEvents: false

                contentItem: GridLayout {
                    columns: 2
                    rowSpacing: Kirigami.Units.smallSpacing
                    columnSpacing: rowSpacing

                    QQC2.Label {
                        Layout.columnSpan: 2

                        text: i18n("Maximum vertical speed:")
                    }

                    QQC2.Label {
                        Layout.minimumWidth: Kirigami.Units.gridUnit * 3
                        Layout.maximumWidth: Kirigami.Units.gridUnit * 3

                        text: i18n("%1 m/s", (kirogi.currentVehicle && kirogi.currentVehicle.ready) ? kirogi.currentVehicle.maxGazSpeed : 0)
                    }

                    QQC2.Slider {
                        Layout.fillWidth: true

                        live: true

                        from: (kirogi.currentVehicle && kirogi.currentVehicle.ready) ? kirogi.currentVehicle.maxGazSpeedMin : 0
                        to: (kirogi.currentVehicle && kirogi.currentVehicle.ready) ? kirogi.currentVehicle.maxGazSpeedMax : 0

                        onPressedChanged: {
                            if (!pressed && value != kirogi.currentVehicle.maxGazSpeed) {
                                kirogi.currentVehicle.requestMaxGazSpeed(Math.round(value));
                            }
                        }

                        Connections {
                            target: kirogi.currentVehicle

                            onMaxDistanceChanged: parent.value = kirogi.currentVehicle.maxGazSpeed
                        }

                        Rectangle {
                            visible: parent.pressed

                            z: -1

                            width: Kirigami.Units.devicePixelRatio
                            height: parent.height

                            color: Kirigami.Theme.disabledTextColor

                            onVisibleChanged: {
                                x = parent.leftPadding + (parent.visualPosition * (parent.availableWidth - width));
                            }
                        }
                    }
                }
            }

            Kirigami.AbstractListItem {
                visible: !performanceAction.collapsed

                enabled: (kirogi.currentVehicle
                    && kirogi.currentVehicle.isActionSupported(Kirogi.AbstractVehicle.SetMaxTilt))

                leftPadding: Kirigami.Units.largeSpacing * 4

                separatorVisible: false
                supportsMouseEvents: false

                contentItem: GridLayout {
                    columns: 2
                    rowSpacing: Kirigami.Units.smallSpacing
                    columnSpacing: rowSpacing

                    QQC2.Label {
                        Layout.columnSpan: 2

                        text: i18n("Maximum tilt:")
                    }

                    QQC2.Label {
                        Layout.minimumWidth: Kirigami.Units.gridUnit * 3
                        Layout.maximumWidth: Kirigami.Units.gridUnit * 3

                        text: i18n("%1°", (kirogi.currentVehicle && kirogi.currentVehicle.ready) ? kirogi.currentVehicle.maxTilt : 0)
                    }

                    QQC2.Slider {
                        Layout.fillWidth: true

                        live: true

                        from: (kirogi.currentVehicle && kirogi.currentVehicle.ready) ? kirogi.currentVehicle.maxTiltMin : 0
                        to: (kirogi.currentVehicle && kirogi.currentVehicle.ready) ? kirogi.currentVehicle.maxTiltMax : 0

                        onPressedChanged: {
                            if (!pressed && value != kirogi.currentVehicle.maxTilt) {
                                kirogi.currentVehicle.requestMaxTilt(Math.round(value));
                            }
                        }

                        Connections {
                            target: kirogi.currentVehicle

                            onMaxDistanceChanged: parent.value = kirogi.currentVehicle.maxTilt
                        }

                        Rectangle {
                            visible: parent.pressed

                            z: -1

                            width: Kirigami.Units.devicePixelRatio
                            height: parent.height

                            color: Kirigami.Theme.disabledTextColor

                            onVisibleChanged: {
                                x = parent.leftPadding + (parent.visualPosition * (parent.availableWidth - width));
                            }
                        }
                    }
                }
            }

            Kirigami.AbstractListItem {
                visible: !performanceAction.collapsed

                enabled: (kirogi.currentVehicle
                    && kirogi.currentVehicle.isActionSupported(Kirogi.AbstractVehicle.ToggleBankedTurns))

                implicitHeight: performanceAction.height

                leftPadding: Kirigami.Units.largeSpacing * 4

                separatorVisible: false

                contentItem: QQC2.Label {
                    verticalAlignment: Text.AlignVCenter

                    text: {
                        if (enabled && kirogi.currentVehicle.bankedTurns) {
                            return i18n("Disable Banked Turns");
                        }

                        return i18n("Enable Banked Turns");
                    }
                }

                onClicked: kirogi.currentVehicle.requestAction(Kirogi.AbstractVehicle.ToggleBankedTurns)
            }

            Kirigami.AbstractListItem {
                visible: !performanceAction.collapsed

                enabled: (kirogi.currentVehicle
                    && kirogi.currentVehicle.isActionSupported(Kirogi.AbstractVehicle.ToggleVideoStabilization))

                implicitHeight: performanceAction.height

                leftPadding: Kirigami.Units.largeSpacing * 4

                separatorVisible: false

                contentItem: QQC2.Label {
                    verticalAlignment: Text.AlignVCenter

                    text: {
                        if (enabled && kirogi.currentVehicle.videoStabilization) {
                            return i18n("Disable Video Stabilization");
                        }

                        return i18n("Enable Video Stabilization");
                    }
                }

                onClicked: kirogi.currentVehicle.requestAction(Kirogi.AbstractVehicle.ToggleVideoStabilization)
            }
        }
    }
}
