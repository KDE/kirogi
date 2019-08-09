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
import QtGamepad 1.12

Gamepad {
    id: gamepad

    deviceId: GamepadManager.connectedGamepads.length > 0 ? GamepadManager.connectedGamepads[0] : -1

    onAxisLeftXChanged: {
        if (kirogi.flying && inputMode.selectedMode == 1) {
            kirogi.currentVehicle.pilot(axisRightX * 100, (axisRightY < 0 ? Math.abs(axisRightY) : -axisLeftY) * 100,
                axisLeftX * 100, (axisLeftY < 0 ? Math.abs(axisLeftY) : -axisLeftY) * 100);
        }
    }

    onAxisLeftYChanged: {
        if (kirogi.flying && inputMode.selectedMode == 1) {
            kirogi.currentVehicle.pilot(axisRightX * 100, (axisRightY < 0 ? Math.abs(axisRightY) : -axisLeftY) * 100,
                axisLeftX * 100, (axisLeftY < 0 ? Math.abs(axisLeftY) : -axisLeftY) * 100);
        }
    }

    onAxisRightXChanged: {
        if (kirogi.flying && inputMode.selectedMode == 1) {
            kirogi.currentVehicle.pilot(axisRightX * 100, (axisRightY < 0 ? Math.abs(axisRightY) : -axisLeftY) * 100,
                axisLeftX * 100, (axisLeftY < 0 ? Math.abs(axisLeftY) : -axisLeftY) * 100);
        }
    }

    onAxisRightYChanged: {
        if (kirogi.flying && inputMode.selectedMode == 1) {
            kirogi.currentVehicle.pilot(axisRightX * 100, (axisRightY < 0 ? Math.abs(axisRightY) : -axisLeftY) * 100,
                axisLeftX * 100, (axisLeftY < 0 ? Math.abs(axisLeftY) : -axisLeftY) * 100);
        }
    }
}
