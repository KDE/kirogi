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
import QtLocation 5.6
import QtPositioning 5.6
import QtQuick.Window 2.12
import QtQuick.Controls 2.12 as QQC2
import QtQuick.Shapes 1.12

import QtGraphicalEffects 1.12

import org.kde.kirigami 2.6 as Kirigami

Kirigami.Page {
    id: page

    readonly property var eVoffice: QtPositioning.coordinate(52.5035334, 13.4091622) // KDE e.V. office.

    leftPadding: 0
    rightPadding: 0
    topPadding: 0
    bottomPadding: 0

    Connections {
        target: kirogi.currentVehicle

        onGpsPositionChanged: {
            if (kirogi.currentVehicle.gpsPosition.isValid
                && (kirogi.currentPage != page || map.center == eVoffice)) {
                map.center = kirogi.currentVehicle.gpsPosition;
            }
        }

        onMaxDistanceChanged: geoFence.radius = kirogi.currentVehicle.maxDistance
    }

    Connections {
        target: kirogi

        onPositionChanged: {
            if (kirogi.position != null
                && (kirogi.currentPage != page || map.center == eVoffice)
                && (!kirogi.currentVehicle || !kirogi.currentVehicle.gpsPosition.isValid)) {
                map.center = kirogi.position;
            }
        }
    }

    Map {
        id: map

        anchors.fill: parent

        center: eVoffice
        zoomLevel: 19

        gesture.acceptedGestures: MapGestureArea.PinchGesture | MapGestureArea.PanGesture | MapGestureArea.FlickGesture

        plugin: Plugin {
            preferred: ["osm"]

            PluginParameter {
                name: "osm.mapping.highdpi_tiles"
                value: Screen.devicePixelRatio > 1 ? "true" : "false"
            }
        }

        onMapReadyChanged: {
// FIXME TODO: Preferrably get a satellite map.
//             if (mapReady) {
//                 for (var i = 0; i < supportedMapTypes.length; i++) {
//                     if (supportedMapTypes[i].style == MapType.SatelliteMapDay) {
//                         activeMapType = supportedMapTypes[i];
//                         break;
//                     }
//                 }
//             }

            addMapItem(geoFence);
            addMapItem(droneLocationMarker);
            addMapItem(deviceLocationMarker);
        }

        TapHandler {
            onTapped: {
                if (kirogi.flying && kirogi.currentVehicle.gpsFix
                    && kirogi.currentVehicle.gpsPosition.isValid) {
                    var coord = map.toCoordinate(eventPoint.position, true);
                    coord.altitude = kirogi.currentVehicle.gpsPosition.altitude;
                    kirogi.currentVehicle.requestMoveTo(coord);
                }
            }
        }
    }

    MapCircle {
        id: geoFence

        visible: (kirogi.currentVehicle && kirogi.currentVehicle.geofence
            && kirogi.currentVehicle.gpsPosition.isValid)

        center: (kirogi.currentVehicle && kirogi.currentVehicle.gpsPosition.isValid
            ? kirogi.currentVehicle.gpsPosition : map.center)

        border.width: Kirigami.Units.devicePixelRatio
        border.color: Kirigami.Theme.negativeTextColor
    }

    MapQuickItem {
        id: droneLocationMarker

        visible: (kirogi.currentVehicle && kirogi.currentVehicle.gpsSupported
            && kirogi.currentVehicle.gpsPosition.isValid)

        coordinate: (kirogi.currentVehicle && kirogi.currentVehicle.gpsPosition.isValid
            ? kirogi.currentVehicle.gpsPosition : map.center)
        anchorPoint: Qt.point(sourceItem.width / 2, sourceItem.height / 2)

        sourceItem: Kirigami.Icon {
            width: Kirigami.Units.iconSizes.medium
            height: width
            source: centerOnDroneButton.icon

            LinearGradient {
                anchors.fill: parent
                source: parent
                start: Qt.point(0, 0)
                end: Qt.point(0, height)
                gradient: Gradient {
                    GradientStop { position: 0.49; color: "green" }
                    GradientStop { position: 0.51; color: "red" }
                }
                transform: Rotation {
                    origin.x: droneLocationMarker.width / 2
                    origin.y: droneLocationMarker.height / 2
                    angle: kirogi.currentVehicle.yaw * 180 / 3.1415}
            }
        }
    }

    MapQuickItem {
        id: deviceLocationMarker

        visible: kirogi.position != null

        coordinate: kirogi.position != null ? kirogi.position : map.center
        anchorPoint: Qt.point(sourceItem.width / 2, sourceItem.height / 2)

        sourceItem: Kirigami.Icon {
            width: Kirigami.Units.iconSizes.medium
            height: width

            color: "blue"
            source: centerOnDeviceButton.icon
        }
    }

    PillBox {
        visible: mapHeading.text

        anchors.top: parent.top
        anchors.topMargin: Kirigami.Units.gridUnit
        anchors.horizontalCenter: parent.horizontalCenter

        width: mapHeading.implicitWidth + Kirigami.Units.largeSpacing * 4
        height: 2 * Math.round((Math.max(Kirigami.Units.iconSizes.small, fontMetrics.height) + Kirigami.Units.smallSpacing * 3) / 2);

        background: map
        backgroundColor: Kirigami.Theme.negativeTextColor
        backgroundOpacity: 0.4
        borderColor: Kirigami.Theme.negativeTextColor

        QQC2.Label {
            id: mapHeading

            anchors.horizontalCenter: parent.horizontalCenter

            height: parent.height

            verticalAlignment: Text.AlignVCenter

            color: Kirigami.Theme.negativeTextColor
            font.bold: true

            text: {
                if (map.error == Map.ConnectionError) {
                    return i18n("Unable to load map.");
                } else if (kirogi.currentVehicle && !kirogi.currentVehicle.gpsSupported) {
                    return i18n("%1 does not support GPS.", kirogi.currentVehicle.name);
                } else if (kirogi.currentVehicle && (!kirogi.currentVehicle.gpsFix || !kirogi.currentVehicle.gpsPosition.isValid)) {
                    return i18n("%1 does not have a GPS fix.", kirogi.currentVehicle.name);
                } else if (kirogi.position == null) {
                    return i18n("No device location available.");
                }

                return "";
            }
        }
    }

    TouchButton {
        anchors.top: parent.top
        anchors.topMargin: Kirigami.Units.gridUnit
        anchors.left: parent.left
        anchors.leftMargin: Kirigami.Units.gridUnit

        background: map

        icon: "transform-move"
        toolTipText: i18nc("%1 = Keyboard shortcut", "Flight Controls (%1)", flightControlsPageAction.shortcut)

        onTapped: switchApplicationPage(flightControlsPage)
    }

    TouchButton {
        id: centerOnDroneButton

        enabled: kirogi.currentVehicle && kirogi.currentVehicle.gpsSupported && kirogi.currentVehicle.gpsPosition.isValid

        anchors.bottom: centerOnDeviceButton.top
        anchors.bottomMargin: Kirigami.Units.largeSpacing
        anchors.right: parent.right
        anchors.rightMargin: Kirigami.Units.gridUnit

        background: map

        icon: kirogi.currentVehicle ? kirogi.currentVehicle.iconName : "uav"
        toolTipText: i18n("Center Map on Drone")

        onTapped: map.center = kirogi.currentVehicle.gpsPosition
    }

    TouchButton {
        id: centerOnDeviceButton

        enabled: kirogi.position != null

        anchors.bottom: parent.bottom
        anchors.bottomMargin: Kirigami.Units.gridUnit
        anchors.right: parent.right
        anchors.rightMargin: Kirigami.Units.gridUnit

        background: map

        icon: Kirigami.Settings.isMobile ? "phone-symbolic" : "computer-symbolic"
        toolTipText: i18n("Center Map on Device")

        onTapped: map.center = kirogi.position
    }
}
