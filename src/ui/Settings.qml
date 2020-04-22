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

Kirigami.ScrollablePage {
    id: page

    title: i18n("Settings")

    Kirigami.FormLayout {
        Kirigami.Heading { text: page.title }

        Item {
            height: Kirigami.Units.gridUnit
        }

        QQC2.CheckBox {
            id: allowLocationRequests
            text: i18n("Allow Kirogi to request the location of this device")
            checked: kirogiSettings.allowLocationRequests
            onToggled: kirogiSettings.allowLocationRequests = checked
        }

        QQC2.CheckBox {
            id: alwaysShowDPads
            text: i18n("Show D-pads when a gamepad controller is connected")
            checked: kirogiSettings.alwaysShowDPads
            onToggled: kirogiSettings.alwaysShowDPads = checked
        }

        Kirigami.Heading { text: "Video" }

        QQC2.CheckBox {
            id: stretchVideo

            text: i18n("Stretch video to fit device screen.")

            checked: kirogiSettings.stretchVideo
            onToggled: kirogiSettings.stretchVideo = checked
        }

        Kirigami.Heading { text: "Joystick" }

        QQC2.CheckBox {
            id: exponentialFitting

            text: i18n("Enable exponential fitting for joysticks")

            checked: kirogiSettings.exponentialFitting
            onToggled: kirogiSettings.exponentialFitting = checked
        }
    }
}
