/*
 * Copyright 2019 Patrick José Pereira <patrickjp@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mavlinkvehicle.h"
#include "debug.h"
#include "parametermodel.h"

#include <QDataStream>
#include <QDateTime>
#include <QStandardPaths>
#include <qloggingcategory.h>

MAVLinkVehicle::MAVLinkVehicle(QObject *parent)
    : Kirogi::AbstractVehicle(parent)
    , m_roll(0.0)
    , m_pitch(0.0)
    , m_yaw(0.0)
    , m_signalStrength(-1)
    , m_batteryLevel(-1)
    , m_gpsFix(false)
    , m_connection(nullptr)
{
    m_connection = new MAVLinkConnection(name());

    // Queued connections across thread boundaries.
    QObject::connect(m_connection, &MAVLinkConnection::stateChanged, this, &MAVLinkVehicle::setConnectionState, Qt::QueuedConnection);

    // Request vehicle parameters if we are in connected state.
    QObject::connect(this, &MAVLinkVehicle::connectionStateChanged, this, &MAVLinkVehicle::fetchParameters, Qt::QueuedConnection);

    QObject::connect(m_connection, &MAVLinkConnection::mavlinkMessage, this, &MAVLinkVehicle::processMavlinkMessage);

    // Do the networking on a separate thread, so our fixed-tick work never gets
    // blocked by activity on the main thread.
    m_connection->moveToThread(&m_connectionThread);
    QObject::connect(&m_connectionThread, &QThread::finished, m_connection, &QObject::deleteLater);

    m_connectionThread.setObjectName(QLatin1String("MAVLinkConnectionThread"));
    m_connectionThread.start();

    m_commandTimer.setSingleShot(true);
    // TODO: Set appropriate resending interval.
    m_commandTimer.setInterval(3000);
    QObject::connect(&m_commandTimer, &QTimer::timeout, this, &MAVLinkVehicle::sendCommandInQueue);
}

MAVLinkVehicle::~MAVLinkVehicle()
{
    m_connectionThread.quit();
    m_connectionThread.wait();
}

void MAVLinkVehicle::processMavlinkMessage(const mavlink_message_t &message)
{
    switch (message.msgid) {
    case MAVLINK_MSG_ID_HEARTBEAT: {
        mavlink_heartbeat_t heartbeat;
        mavlink_msg_heartbeat_decode(&message, &heartbeat);

        switch (heartbeat.system_status) {
        case MAV_STATE::MAV_STATE_UNINIT:
        case MAV_STATE::MAV_STATE_BOOT:
        case MAV_STATE::MAV_STATE_CALIBRATING:
        case MAV_STATE::MAV_STATE_CRITICAL:
        case MAV_STATE::MAV_STATE_EMERGENCY:
            setConnectionState(Kirogi::AbstractVehicle::ConnectionState::Connected);
            break;

        case MAV_STATE::MAV_STATE_STANDBY:
        case MAV_STATE::MAV_STATE_ACTIVE:
            setConnectionState(Kirogi::AbstractVehicle::ConnectionState::Ready);
            break;

        case MAV_STATE::MAV_STATE_FLIGHT_TERMINATION:
        case MAV_STATE::MAV_STATE_POWEROFF:
            setConnectionState(Kirogi::AbstractVehicle::ConnectionState::Disconnected);
            break;

        // We did receive a message so it's connected..
        default:
            setConnectionState(Kirogi::AbstractVehicle::ConnectionState::Connected);
            break;
        }

        break;
    }

    case MAVLINK_MSG_ID_COMMAND_ACK: {
        mavlink_command_ack_t command_ack;
        mavlink_msg_command_ack_decode(&message, &command_ack);

        // TODO: Need to more stuffs related to ack command.
        if (command_ack.command == m_commandQueue[0].command) {
            m_cmdResendCnt = 0;
            m_commandTimer.stop();
            m_commandQueue.removeFirst();
            sendCommandInQueue();
        } else {
            qCDebug(KIROGI_VEHICLESUPPORT_MAVLINK) << "Received unintended ack message.";
        }

        break;
    }

    case MAVLINK_MSG_ID_RC_CHANNELS_SCALED: {
        mavlink_rc_channels_scaled_t rc_channels_scaled;
        mavlink_msg_rc_channels_scaled_decode(&message, &rc_channels_scaled);

        m_signalStrength = rc_channels_scaled.rssi >= 255 ? -1 : static_cast<float>(rc_channels_scaled.rssi) / 2.54f;

        emit signalStrengthChanged();
        break;
    }

    case MAVLINK_MSG_ID_RC_CHANNELS_RAW: {
        mavlink_rc_channels_raw_t rc_channels_raw;
        mavlink_msg_rc_channels_raw_decode(&message, &rc_channels_raw);

        m_signalStrength = rc_channels_raw.rssi >= 255 ? -1 : static_cast<float>(rc_channels_raw.rssi) / 2.54f;

        emit signalStrengthChanged();
        break;
    }

    case MAVLINK_MSG_ID_RC_CHANNELS: {
        mavlink_rc_channels_t rc_channels;
        mavlink_msg_rc_channels_decode(&message, &rc_channels);
        m_signalStrength = rc_channels.rssi >= 255 ? -1 : static_cast<float>(rc_channels.rssi) / 2.54f;

        emit signalStrengthChanged();
        break;
    }

    case MAVLINK_MSG_ID_ALTITUDE: {
        mavlink_altitude_t altitude;
        mavlink_msg_altitude_decode(&message, &altitude);

        m_altitudeSource.altitudeMessage = true;
        m_altitudeSource.altitude = altitude.altitude_terrain;

        emit altitudeChanged();
        break;
    }

    case MAVLINK_MSG_ID_ATTITUDE: {
        mavlink_attitude_t attitude;
        mavlink_msg_attitude_decode(&message, &attitude);

        m_yaw = attitude.yaw;
        m_roll = attitude.roll;
        m_pitch = attitude.pitch;

        emit rollChanged();
        emit pitchChanged();
        emit yawChanged();
        emit attitudeChanged();
        break;
    }

    case MAVLINK_MSG_ID_BATTERY_STATUS: {
        mavlink_battery_status_t battery_status;
        mavlink_msg_battery_status_decode(&message, &battery_status);

        m_batteryLevel = battery_status.battery_remaining;

        emit batteryLevelChanged();
        break;
    }

    case MAVLINK_MSG_ID_GLOBAL_POSITION_INT: {
        // Both latitude and longitude are provided in degE7
        // Altitude is provided in mm
        mavlink_global_position_int_t global_position_int;
        mavlink_msg_global_position_int_decode(&message, &global_position_int);

        m_gpsFix = !(qFuzzyCompare(global_position_int.lat, 0.0) && qFuzzyCompare(global_position_int.lon, 0.0));
        emit gpsFixChanged();

        if (m_gpsFix) {
            m_gpsPosition.setLatitude(global_position_int.lat / 1e7);
            m_gpsPosition.setLongitude(global_position_int.lon / 1e7);
        } else {
            // Invalidate position if there is no fix
            m_gpsPosition.setLatitude(qQNaN());
            m_gpsPosition.setLongitude(qQNaN());
        }

        m_gpsPosition.setAltitude(global_position_int.alt / 1e3);
        emit gpsPositionChanged();

        // Check if altitude message was received
        if (!m_altitudeSource.altitudeMessage) {
            m_altitudeSource.altitude = m_gpsPosition.altitude();
            emit altitudeChanged();
        }
        break;
    }

    case MAVLINK_MSG_ID_PARAM_VALUE: {
        mavlink_param_value_t param_value;
        mavlink_msg_param_value_decode(&message, &param_value);

        parameters()->update({
            {ParameterModel::Roles::Name, QString(param_value.param_id)},
            {ParameterModel::Roles::Value, static_cast<float>(param_value.param_value)},
            {ParameterModel::Roles::Index, static_cast<int>(param_value.param_index)},
        });
    }

    default: {
        break;
    }
    }
}

QString MAVLinkVehicle::name() const
{
    return QStringLiteral("MAVLink");
}

QString MAVLinkVehicle::iconName() const
{
    return QStringLiteral("uav-quadcopter");
}

Kirogi::AbstractVehicle::VehicleType MAVLinkVehicle::vehicleType() const
{
    // TODO: Get this from mavlink messages
    return Kirogi::AbstractVehicle::QuadCopter;
}

QList<Kirogi::AbstractVehicle::VehicleAction> MAVLinkVehicle::supportedActions() const
{
    // TODO: Improve this to work with mavlink infraestructure
    QList<Kirogi::AbstractVehicle::VehicleAction> actions;
    actions << Kirogi::AbstractVehicle::TakeOff;
    actions << Kirogi::AbstractVehicle::Land;
    actions << Kirogi::AbstractVehicle::FlipForward;
    actions << Kirogi::AbstractVehicle::FlipBackward;
    actions << Kirogi::AbstractVehicle::FlipLeft;
    actions << Kirogi::AbstractVehicle::FlipRight;
    actions << Kirogi::AbstractVehicle::SwitchPerformanceMode;

    return actions;
}

void MAVLinkVehicle::requestAction(Kirogi::AbstractVehicle::VehicleAction action)
{
    if (!ready()) {
        qWarning() << name()
                   << "Requested an action while not ready, aborting. Current "
                      "connection state:"
                   << connectionState();

        return;
    }

    switch (action) {
    case TakeOff: {
        if (flying()) {
            return;
        }

        // Set the vehicle in stabilize mode and after that arm
        CommandQueueEntry_t cmdQueueEntry;

        cmdQueueEntry.command_int = false;
        cmdQueueEntry.target_system = 1;    // TODO: get from system heartbeat
        cmdQueueEntry.target_component = 1; // TODO: get from system heartbeat
        cmdQueueEntry.command = MAV_CMD_DO_SET_MODE;
        cmdQueueEntry.confirmation = 0;
        cmdQueueEntry.params[0] = MAV_MODE_FLAG_CUSTOM_MODE_ENABLED;
        cmdQueueEntry.params[1] = 0;
        cmdQueueEntry.params[2] = 0; // 0 for stabilize
        cmdQueueEntry.params[3] = 0;
        cmdQueueEntry.params[4] = 0;
        cmdQueueEntry.params[5] = 0;
        cmdQueueEntry.params[6] = 0;

        m_commandQueue.append(cmdQueueEntry);

        cmdQueueEntry.command_int = false;
        cmdQueueEntry.target_system = 1;    // TODO: get from system heartbeat
        cmdQueueEntry.target_component = 1; // TODO: get from system heartbeat
        cmdQueueEntry.command = MAV_CMD_COMPONENT_ARM_DISARM;
        cmdQueueEntry.confirmation = 0;
        cmdQueueEntry.params[0] = 1;
        cmdQueueEntry.params[1] = 0;
        cmdQueueEntry.params[2] = 0;
        cmdQueueEntry.params[3] = 0;
        cmdQueueEntry.params[4] = 0;
        cmdQueueEntry.params[5] = 0;
        cmdQueueEntry.params[6] = 0;

        m_commandQueue.append(cmdQueueEntry);

        setFlyingState(TakingOff); // FIXME: We don't /really/ know that without
                                   // looking at the response.

        if (m_commandQueue.count() == 2) {
            m_cmdResendCnt = 0;
            sendCommandInQueue();
        }

        break;
    }
    case Land: {
        if (!flying()) {
            return;
        }

        // Disarm vehicle
        CommandQueueEntry_t cmdQueueEntry;

        cmdQueueEntry.command_int = false;
        cmdQueueEntry.target_system = 1;    // TODO: get from system heartbeat
        cmdQueueEntry.target_component = 1; // TODO: get from system heartbeat
        cmdQueueEntry.command = MAV_CMD_COMPONENT_ARM_DISARM;
        cmdQueueEntry.confirmation = 0;
        cmdQueueEntry.params[0] = 0;
        cmdQueueEntry.params[1] = 0;
        cmdQueueEntry.params[2] = 0;
        cmdQueueEntry.params[3] = 0;
        cmdQueueEntry.params[4] = 0;
        cmdQueueEntry.params[5] = 0;
        cmdQueueEntry.params[6] = 0;

        m_commandQueue.append(cmdQueueEntry);

        setFlyingState(Landed); // FIXME: We don't /really/ know that without
                                // looking at the response.

        if (m_commandQueue.count() == 1) {
            m_cmdResendCnt = 0;
            sendCommandInQueue();
        }

        break;
    }
    default: {
    }
    }
}

void MAVLinkVehicle::sendCommandInQueue()
{
    if (m_commandQueue.count() == 0)
        return;

    // TODO: Need to set appropriate maximum resend count.
    if (m_cmdResendCnt++ > 10) {
        m_commandQueue.removeFirst();
        sendCommandInQueue();
        return;
    }

    CommandQueueEntry_t cmdQueueEntry = m_commandQueue[0];
    mavlink_command_long_t command_long;

    command_long.target_system = cmdQueueEntry.target_system;
    command_long.target_component = cmdQueueEntry.target_component;
    command_long.command = cmdQueueEntry.command;
    command_long.confirmation = cmdQueueEntry.confirmation;
    command_long.param1 = cmdQueueEntry.params[0];
    command_long.param2 = cmdQueueEntry.params[1];
    command_long.param3 = cmdQueueEntry.params[2];
    command_long.param4 = cmdQueueEntry.params[3];
    command_long.param5 = cmdQueueEntry.params[4];
    command_long.param6 = cmdQueueEntry.params[5];
    command_long.param7 = cmdQueueEntry.params[6];

    m_commandTimer.start();
    m_connection->sendMessage(command_long);
}

void MAVLinkVehicle::pilot(qint8 roll, qint8 pitch, qint8 yaw, qint8 gaz)
{
    mavlink_manual_control_t manual_control;
    manual_control.target = 1;
    manual_control.x = pitch * 10; // [-1000,1000] range
    manual_control.y = roll * 10;  // [-1000,1000] range
    manual_control.z = gaz * 10;   // [-1000,1000] range
    manual_control.r = yaw * 10;   // [-1000,1000] range
    manual_control.buttons = 0;
    m_connection->sendMessage(manual_control);
}

float MAVLinkVehicle::roll() const
{
    return m_roll;
}

float MAVLinkVehicle::pitch() const
{
    return m_pitch;
}

float MAVLinkVehicle::yaw() const
{
    return m_yaw;
}

int MAVLinkVehicle::signalStrength() const
{
    return m_signalStrength;
}

int MAVLinkVehicle::batteryLevel() const
{
    return m_batteryLevel;
}

QGeoCoordinate MAVLinkVehicle::gpsPosition() const
{
    return m_gpsPosition;
}

bool MAVLinkVehicle::gpsFix() const
{
    return m_gpsFix;
}

bool MAVLinkVehicle::gpsSupported() const
{
    return true;
}

float MAVLinkVehicle::altitude() const
{
    return m_altitudeSource.altitude;
}

void MAVLinkVehicle::connectToVehicle()
{
    if (connectionState() > Disconnected) {
        if (connectionState() > Connecting) {
            qCDebug(KIROGI_VEHICLESUPPORT_MAVLINK) << name() << "Asked to connect when not disconnected.";
        }

        QMetaObject::invokeMethod(m_connection, "reset", Qt::BlockingQueuedConnection);
    }

    QMetaObject::invokeMethod(m_connection, "handshake", Qt::QueuedConnection);

    // Keep re-trying every 3 seconds until we're successfully connected.
    QTimer::singleShot(3000, this, [this]() {
        if (connectionState() != Ready) {
            qCWarning(KIROGI_VEHICLESUPPORT_MAVLINK) << name()
                                                     << "Unable to establish connection within 3 seconds. Starting "
                                                        "over.";

            connectToVehicle();
        }
    });
}

void MAVLinkVehicle::fetchParameters() const
{
    // We need to handle this in a different way to ensure that we are going to receive all parameters.
    static bool requested = false;
    if (connectionState() < Kirogi::AbstractVehicle::ConnectionState::Connected || requested) {
        return;
    }
    requested = true;

    mavlink_param_request_list_t param_request_list;
    param_request_list.target_system = 1;
    param_request_list.target_component = 1;
    m_connection->sendMessage(param_request_list);
}

QString MAVLinkVehicle::videoSource() const
{
    return QLatin1String(
        "rtspsrc location=rtsp://192.168.99.1/media/stream2 latency=5 ! "
        "rtph264depay ! "
        "video/x-h264 ! "
        "queue ! "
        "h264parse ! "
        "decodebin ! ");
}
