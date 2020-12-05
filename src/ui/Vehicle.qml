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

Kirigami.Page {
    id: page

    leftPadding: 0
    rightPadding: 0
    topPadding: 0
    bottomPadding: 0

    Image {
        anchors.left: parent.left
        anchors.leftMargin: (((LayoutMirroring.enabled ? (parent.width - flightChecklist.x - flightChecklist.width) : flightChecklist.x) / 2) - (width / 2))
        anchors.verticalCenter: parent.verticalCenter

        width: height
        height: Math.min(parent.height, LayoutMirroring.enabled ? (parent.width - flightChecklist.x - flightChecklist.width) : flightChecklist.x) * 0.94

        fillMode: Image.PreserveAspectFit
        smooth: true

        source: "goose.webp"

        asynchronous: true
    }

    Component {
        id: pluginItemComponent

        Kirigami.BasicListItem {
            id: pluginItem

            height: Math.max(implicitHeight, Kirigami.Units.iconSizes.medium)

            supportsMouseEvents: model.status == Kirogi.VehicleSupportPluginModel.PluginNotLoaded

            reserveSpaceForIcon: kirogi.currentPlugin
            icon: model.status == Kirogi.VehicleSupportPluginModel.PluginLoaded ? "emblem-default-symbolic" : ""

            label: model.display

            onClicked: {
                pluginSheet.sheetOpen = false;
                pluginModel.unloadAllPlugins();
                pluginModel.loadPluginByIndex(index);
            }
        }
    }

    Kirigami.OverlaySheet {
        id: pluginSheet

        parent: kirogi

        topPadding: 0
        leftPadding: 0
        rightPadding: 0
        bottomPadding: 0

        header: Kirigami.Heading { text: i18nc("@title:window", "Select Model") }

        ListView {
            id: pluginList

            implicitWidth: 18 * Kirigami.Units.gridUnit

            model: pluginSheet.sheetOpen ? pluginModel : null

            delegate: Kirigami.DelegateRecycler {
                width: parent.width

                sourceComponent: pluginItemComponent
            }
        }
    }

    Kirigami.AbstractCard {
        id: flightChecklist

        width: (page.width / 3) + (Kirigami.Units.gridUnit * 2)

        implicitHeight: height // FIXME TODO Silence Kirigami warning.

        anchors.right: parent.right
        anchors.rightMargin: Kirigami.Units.gridUnit
        anchors.top: parent.top
        anchors.topMargin: Kirigami.Units.gridUnit
        anchors.bottom: parent.bottom
        anchors.bottomMargin: Kirigami.Units.gridUnit

        topPadding: Kirigami.Settings.isMobile ? header.height * 0.5 : header.height

        header: Kirigami.Heading {
            text:  i18n("Flight Checklist")
            level: 1
            horizontalAlignment: Text.AlignHCenter
            elide: Text.ElideRight
        }

        contentItem: QQC2.ScrollView {
            id: checksList

            clip: true

            property int scrollBarWidth: QQC2.ScrollBar.vertical && QQC2.ScrollBar.vertical.visible
                ? QQC2.ScrollBar.vertical.width : 0

            Column {
                id: col

                width: checksList.width - checksList.scrollBarWidth
                height: implicitHeight

                Kirigami.BasicListItem {
                    implicitHeight: Kirigami.Units.iconSizes.medium + (Kirigami.Units.smallSpacing * 2)

                    supportsMouseEvents: false
                    separatorVisible: !Kirigami.Settings.isMobile

                    contentItem: RowLayout {
                        Kirigami.Icon {
                            Layout.alignment: Qt.AlignVCenter

                            width: Kirigami.Units.iconSizes.medium
                            height: width

                            source: kirogi.currentPlugin ? "emblem-default-symbolic" : "emblem-important-symbolic"
                            color: kirogi.currentPlugin ? Kirigami.Theme.positiveTextColor : Kirigami.Theme.negativeTextColor
                        }

                        QQC2.Label {
                            Layout.fillWidth: true

                            Layout.alignment: Qt.AlignVCenter

                            text: i18n("<b>Model:</b> %1", kirogi.currentPlugin ? kirogi.currentPluginName : i18n("Select now"))
                            color: kirogi.currentPlugin ? Kirigami.Theme.textColor : Kirigami.Theme.negativeTextColor

                            elide: Text.ElideRight
                        }

                        QQC2.ToolButton {
                            Layout.alignment: Qt.AlignVCenter

                            implicitWidth: Kirigami.Units.iconSizes.medium
                            implicitHeight: implicitWidth

                            icon.name: "configure"

                            onClicked: pluginSheet.sheetOpen = !pluginSheet.sheetOpen

                            QQC2.ToolTip.visible: modelButtonHoverHandler.hovered
                            QQC2.ToolTip.delay: Kirigami.Units.toolTipDelay
                            QQC2.ToolTip.text: i18n("Select Model")

                            HoverHandler {
                                id: modelButtonHoverHandler
                            }
                        }
                    }
                }

                Kirigami.BasicListItem {
                    implicitHeight: Kirigami.Units.iconSizes.medium + (Kirigami.Units.smallSpacing * 2)

                    supportsMouseEvents: false
                    separatorVisible: !Kirigami.Settings.isMobile

                    contentItem: RowLayout {
                        Kirigami.Icon {
                            visible: !kirogi.currentPlugin || kirogi.currentPlugin && kirogi.currentVehicle

                            Layout.alignment: Qt.AlignVCenter

                            width: Kirigami.Units.iconSizes.medium
                            height: width

                            source: kirogi.currentVehicle ? "emblem-default-symbolic" : "question"
                            color: kirogi.currentVehicle ? Kirigami.Theme.positiveTextColor : Kirigami.Theme.disabledTextColor
                        }

                        QQC2.BusyIndicator {
                            visible: kirogi.currentPlugin && !kirogi.currentVehicle

                            Layout.alignment: Qt.AlignVCenter

                            implicitWidth: Kirigami.Units.iconSizes.medium
                            implicitHeight: implicitWidth

                            topPadding: 0
                            bottomPadding: 0
                            leftPadding: 0
                            rightPadding: 0

                            running: visible
                        }

                        QQC2.Label {
                            Layout.fillWidth: true

                            Layout.alignment: Qt.AlignVCenter

                            text: {
                                var state = i18n("N/A");

                                if (kirogi.currentPlugin) {
                                    state = i18n("Searching ...");
                                }

                                return i18n("<b>Drone:</b> %1", kirogi.currentVehicle ? kirogi.currentVehicle.name : state);
                            }

                            color: kirogi.currentPlugin ? Kirigami.Theme.textColor : Kirigami.Theme.disabledTextColor

                            elide: Text.ElideRight
                        }
                    }
                }

                Kirigami.BasicListItem {
                    implicitHeight: Kirigami.Units.iconSizes.medium + (Kirigami.Units.smallSpacing * 2)

                    supportsMouseEvents: false
                    separatorVisible: !Kirigami.Settings.isMobile

                    contentItem: RowLayout {
                        Kirigami.Icon {
                            visible: !kirogi.currentVehicle || kirogi.ready

                            Layout.alignment: Qt.AlignVCenter

                            width: Kirigami.Units.iconSizes.medium
                            height: width

                            source: kirogi.currentVehicle ? "emblem-default-symbolic" : "question"
                            color: kirogi.currentVehicle ? Kirigami.Theme.positiveTextColor : Kirigami.Theme.disabledTextColor
                        }

                        QQC2.BusyIndicator {
                            visible: kirogi.currentVehicle && !kirogi.ready

                            Layout.alignment: Qt.AlignVCenter

                            implicitWidth: Kirigami.Units.iconSizes.medium
                            implicitHeight: implicitWidth

                            topPadding: 0
                            bottomPadding: 0
                            leftPadding: 0
                            rightPadding: 0

                            running: visible
                        }

                        QQC2.Label {
                            Layout.fillWidth: true

                            Layout.alignment: Qt.AlignVCenter

                            text: {
                                var state = "N/A";

                                if (kirogi.flying) {
                                    state = i18n("Flying");
                                } else if (kirogi.ready) {
                                    state = i18n("Ready");
                                } else if (kirogi.connected) {
                                    state = i18n("Preparing ...");
                                } else if (kirogi.currentVehicle && (kirogi.currentVehicle.connectionState == Kirogi.AbstractVehicle.Connecting
                                    || kirogi.currentVehicle.connectionState == Kirogi.AbstractVehicle.Disconnected)) {
                                    state = i18n("Connecting ...");
                                }

                                i18n("<b>Status:</b> %1", state);
                            }

                            color: kirogi.currentVehicle ? Kirigami.Theme.textColor : Kirigami.Theme.disabledTextColor

                            elide: Text.ElideRight
                        }
                    }
                }

                Kirigami.BasicListItem {
                    implicitHeight: Kirigami.Units.iconSizes.medium + (Kirigami.Units.smallSpacing * 2)

                    supportsMouseEvents: false
                    separatorVisible: !Kirigami.Settings.isMobile

                    contentItem: RowLayout {
                        Kirigami.Icon {
                            Layout.alignment: Qt.AlignVCenter

                            width: Kirigami.Units.iconSizes.medium
                            height: width

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

                            color: kirogi.currentVehicle ? Kirigami.Theme.textColor : Kirigami.Theme.disabledTextColor
                        }

                        QQC2.Label {
                            Layout.fillWidth: true

                            Layout.alignment: Qt.AlignVCenter

                            text: {
                                var state = kirogi.currentVehicle ? i18n("%1%", kirogi.currentVehicle.signalStrength) : i18n("N/A");
                                return i18n("<b>Signal:</b> %1", state);
                            }

                            color: kirogi.connected ? Kirigami.Theme.textColor : Kirigami.Theme.disabledTextColor

                            elide: Text.ElideRight
                        }
                    }
                }

                Kirigami.BasicListItem {
                    implicitHeight: Kirigami.Units.iconSizes.medium + (Kirigami.Units.smallSpacing * 2)

                    supportsMouseEvents: false
                    separatorVisible: !Kirigami.Settings.isMobile

                    contentItem: RowLayout {
                        Kirigami.Icon {
                            Layout.alignment: Qt.AlignVCenter

                            width: Kirigami.Units.iconSizes.medium
                            height: width

                            source: {
                                if (kirogi.currentVehicle) {
                                    var roundedBatteryLevel = Math.round(kirogi.currentVehicle.batteryLevel / 10);
                                    return "battery-" + roundedBatteryLevel.toString().padStart(2, "0") + "0";
                                }

                                return "battery-missing";
                            }

                            color: kirogi.currentVehicle ? Kirigami.Theme.textColor : Kirigami.Theme.disabledTextColor
                        }

                        QQC2.Label {
                            Layout.fillWidth: true

                            Layout.alignment: Qt.AlignVCenter

                            text: {
                                var state = kirogi.currentVehicle ? i18n("%1%", kirogi.currentVehicle.batteryLevel) : i18n("N/A");
                                return i18n("<b>Battery:</b> %1", state);
                            }

                            color: kirogi.connected ? Kirigami.Theme.textColor : Kirigami.Theme.disabledTextColor

                            elide: Text.ElideRight
                        }
                    }
                }

                Kirigami.BasicListItem {
                    implicitHeight: Kirigami.Units.iconSizes.medium + (Kirigami.Units.smallSpacing * 2)

                    supportsMouseEvents: false
                    separatorVisible: !Kirigami.Settings.isMobile

                    contentItem: RowLayout {
                        Kirigami.Icon {
                            Layout.alignment: Qt.AlignVCenter

                            width: Kirigami.Units.iconSizes.medium
                            height: width

                            source: "gps"

                            color: {
                                if (kirogi.connected) {
                                    if (kirogi.currentVehicle.gpsFix) {
                                        return Kirigami.Theme.positiveTextColor;
                                    } else if (kirogi.currentVehicle.gpsSupported) {
                                        return Kirigami.Theme.negativeTextColor;
                                    }
                                }

                                return Kirigami.Theme.disabledTextColor;
                            }
                        }

                        QQC2.Label {
                            Layout.fillWidth: true

                            Layout.alignment: Qt.AlignVCenter

                            text: {
                                var state = i18n("N/A");

                                if (kirogi.currentVehicle) {
                                    if (kirogi.currentVehicle.gpsFix) {
                                        state = i18n("Got fix");
                                    } else if (kirogi.currentVehicle.gpsSupported) {
                                        state = i18n("No fix");
                                    } else {
                                        state = i18n("Not supported");
                                    }
                                }

                                return i18n("<b>GPS:</b> %1", state)
                            }

                            color: {
                                if (kirogi.connected) {
                                    if (kirogi.currentVehicle.gpsFix) {
                                        return Kirigami.Theme.positiveTextColor;
                                    } else if (kirogi.currentVehicle.gpsSupported) {
                                        return Kirigami.Theme.textColor;
                                    }
                                }

                                return Kirigami.Theme.disabledTextColor;
                            }

                            elide: Text.ElideRight
                        }
                    }
                }
            }
        }

        footer: QQC2.Button {
            text: {
                if (!kirogi.currentPlugin) {
                    return i18n("Select Model");
                } else if (kirogi.ready) {
                    return i18n("Go to Flight Controls");
                }

                return i18n("Try Flight Controls");
            }

            onClicked: {
                if (!kirogi.currentPlugin) {
                    pluginSheet.sheetOpen = !pluginSheet.sheetOpen;

                    return;
                }

                switchApplicationPage(flightControlsPage);
            }
        }
    }
}
