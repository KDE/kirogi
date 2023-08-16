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
import QtPositioning 5.12

import org.kde.kirigami 2.20 as Kirigami
import org.kde.kirigamiaddons.formcard 1.0 as FormCard

import org.kde.kirogi 0.1 as Kirogi

Kirigami.ApplicationWindow {
    id: kirogi

    width: 800
    height: 450

    property QtObject currentVehicle: null
    readonly property bool connected: currentVehicle && currentVehicle.connected
    readonly property bool ready: currentVehicle && currentVehicle.ready
    readonly property bool flying: currentVehicle && currentVehicle.flying

    property var currentPage: pageStack.currentItem
    property alias currentPlugin: pluginModel.currentPlugin
    property alias currentPluginName: pluginModel.currentPluginName

    readonly property var position: Kirogi.PositionSource.coordinate
    readonly property real distance: {
        if (!position || !position.isValid) {
            return 0.0;
        }

        // If currentVehicle is null or there is no valid position
        // set distance to zero.
        if (!currentVehicle || !currentVehicle.gpsPosition.isValid) {
            return 0.0;
        }

        return position.distanceTo(currentVehicle.gpsPosition);
    }

    onPositionChanged: {
        // Position via internet IP does not provide a valid altitude.
        if (position.isValid && currentVehicle) {
            currentVehicle.setControllerGpsPosition(position)
        }
    }

    pageStack.interactive: false
    pageStack.defaultColumnWidth: width
    pageStack.globalToolBar.style: Kirigami.ApplicationHeaderStyle.None

    globalDrawer: Kirigami.GlobalDrawer {
        title: i18n("Kirogi")
        titleIcon: "kirogi"

        width: Kirigami.Units.gridUnit * 14

        actions: [
            Kirigami.Action {
                id: vehiclePageAction

                checked: currentPage == vehiclePage

                iconName: "uav-quadcopter"
                text: i18n("Drone")

                tooltip: "Alt+1"
                shortcut: tooltip

                onTriggered: switchApplicationPage(vehiclePage)
            },
            Kirigami.Action {
                id: flightControlsPageAction

                checked: currentPage == flightControlsPage

                iconName: "transform-move"
                text: i18n("Flight Controls")

                tooltip: "Alt+2"
                shortcut: tooltip

                onTriggered: switchApplicationPage(flightControlsPage)
            },
            Kirigami.Action {
                id: navigationMapPageAction

                checked: currentPage == navigationMapPage

                iconName: "map-flat"
                text: i18n("Navigation Map")

                tooltip: "Alt+3"
                shortcut: tooltip

                onTriggered: switchApplicationPage(navigationMapPage)
            },
            Kirigami.Action {
                id: settingsPageAction

                checked: currentPage == settingsPage

                iconName: "configure"
                text: i18n("Settings")

                tooltip: "Alt+4"
                shortcut: tooltip

                onTriggered: switchApplicationPage(settingsPage)
            },
            Kirigami.Action {
                id: aboutPageAction

                checked: currentPage == aboutPage

                iconName: "help-about"
                text: i18n("About")

                tooltip: "Alt+5"
                shortcut: tooltip

                onTriggered: pageStack.pushDialogLayer(aboutPage)
            }
        ]
    }

    onConnectedChanged: {
        if (connected && kirogiSettings.allowLocationRequests && !locationPermissions.granted) {
            locationPermissions.request();
        }
    }

    onFlyingChanged: {
        kirogiSettings.flying = flying;
    }

    function switchApplicationPage(page) {
        if (!page || currentPage == page) {
            return;
        }

        pageStack.removePage(page);
        pageStack.push(page);
        page.forceActiveFocus();
    }

    Kirogi.VehicleSupportPluginModel {
        id: pluginModel

        property QtObject currentPlugin: null
        property string currentPluginName: ""

        onPluginLoaded: {
            kirogiSettings.lastPlugin = pluginId;
            currentPluginName = name;
            currentPlugin = plugin;
        }

        Component.onCompleted: {
            if (kirogiSettings.lastPlugin) {
                loadPluginById(kirogiSettings.lastPlugin);
            }
        }
    }

    Connections {
        target: currentPlugin

        onVehicleAdded: {
            currentVehicle = vehicle;
        }
    }

    FontMetrics {
        id: fontMetrics
    }

    Vehicle {
        id: vehiclePage

        enabled: currentPage == vehiclePage
        visible: enabled
    }

    FlightControls {
        id: flightControlsPage

        enabled: currentPage == flightControlsPage
        visible: enabled
    }

    NavigationMap {
        id: navigationMapPage

        enabled: currentPage == navigationMapPage
        visible: enabled
    }

    Settings {
        id: settingsPage

        enabled: currentPage == settingsPage
        visible: enabled
    }

    Component {
        id: aboutPage
        FormCard.AboutPage {
            id: aboutPage
            aboutData: kirogiAboutData
        }
    }

    Timer {
        id: resetPersistentFlyingStateTimer

        interval: 3000
        repeat: false

        onTriggered: {
            kirogiSettings.flying = flying;
        }
    }

    Component.onCompleted: {
        switchApplicationPage(kirogiSettings.flying ? flightControlsPage : vehiclePage);
        resetPersistentFlyingStateTimer.start();
        Kirogi.PositionSource.enabled = Qt.binding(function() { return kirogiSettings.allowLocationRequests && locationPermissions.granted });
    }
}
