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
#include "parametermodel.h"

#include <QDataStream>
#include <QDateTime>
#include <QStandardPaths>

#include "mavlink_plugin_debug.h"

MAVLinkVehicle::MAVLinkVehicle(Configuration configuration, MAVLinkConnection *connection, QObject *parent)
    : Kirogi::AbstractVehicle(parent)
    , m_connection(connection)
    , m_configuration(configuration)
    , m_name(configuration.name) // TODO: name field is duplicated.
    , m_roll(0.0)
    , m_pitch(0.0)
    , m_yaw(0.0)
    , m_signalStrength(-1)
    , m_batteryLevel(-1)
    , m_gpsFix(false)
{
    QObject::connect(m_connection, &MAVLinkUdpConnection::stateChanged, this, [this]() {
        switch (m_connection->state()) {
        case Kirogi::AbstractConnection::State::Disconnected:
            setConnectionState(Kirogi::AbstractVehicle::Disconnected);
            break;
        case Kirogi::AbstractConnection::State::Connected:
            setConnectionState(Kirogi::AbstractVehicle::Connected);
            break;
        }
    });
    QObject::connect(this, &MAVLinkVehicle::connectionStateChanged, this, &MAVLinkVehicle::fetchParameters);
    QObject::connect(m_connection, &MAVLinkConnection::messageReceived, this, &MAVLinkVehicle::processMessage);

    m_commandResendTimer.setSingleShot(true);
    m_commandResendTimer.setInterval(3000);
    QObject::connect(&m_commandResendTimer, &QTimer::timeout, this, &MAVLinkVehicle::sendCommandInQueue);
}

MAVLinkVehicle::~MAVLinkVehicle()
{
}

void MAVLinkVehicle::processMessage(MAVLinkConnection *connection, const mavlink_message_t &message)
{
    Q_UNUSED(connection);

    if (connectionState() < Kirogi::AbstractVehicle::ConnectionState::Ready) {
        setConnectionState(Kirogi::AbstractVehicle::ConnectionState::Ready);
    }

    switch (message.msgid) {
    case MAVLINK_MSG_ID_HEARTBEAT:
        handleHeartbeat(message);
        break;
    case MAVLINK_MSG_ID_COMMAND_ACK:
        handleCommandAck(message);
        break;
    case MAVLINK_MSG_ID_RC_CHANNELS_SCALED:
        handleRcChannelsScaled(message);
        break;
    case MAVLINK_MSG_ID_RC_CHANNELS_RAW:
        handleRcChannelsRaw(message);
        break;
    case MAVLINK_MSG_ID_RC_CHANNELS:
        handleRcChannels(message);
        break;
    case MAVLINK_MSG_ID_ALTITUDE:
        handleAltitude(message);
        break;
    case MAVLINK_MSG_ID_ATTITUDE:
        handleAttitude(message);
        break;
    case MAVLINK_MSG_ID_BATTERY_STATUS:
        handleBatteryStatus(message);
        break;
    case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
        handleGlobalPositionInt(message);
        break;
    case MAVLINK_MSG_ID_PARAM_VALUE:
        handleParamValue(message);
        break;
    }
}

QString MAVLinkVehicle::name() const
{
    return m_name;
}

QString MAVLinkVehicle::iconName() const
{
    switch (m_configuration.type) {
    case MAV_TYPE_QUADROTOR:
        return QStringLiteral("uav-quadcopter");
        break;
    default:
        return QStringLiteral("uav-quadcopter");
    }
}

Kirogi::AbstractVehicle::VehicleType MAVLinkVehicle::vehicleType() const
{
    switch (m_configuration.type) {
    case MAV_TYPE_QUADROTOR:
        return Kirogi::AbstractVehicle::QuadCopter;
        break;
    default:
        return Kirogi::AbstractVehicle::UnknownVehicleType;
    }
}

QList<Kirogi::AbstractVehicle::VehicleAction> MAVLinkVehicle::supportedActions() const
{
    // TODO: Improve this to work with mavlink infrastructure
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
        CommandQueueItem commandQueueItem;

        commandQueueItem.command_int = false;
        commandQueueItem.target_system = m_configuration.sysid;
        commandQueueItem.target_component = m_configuration.compid;
        commandQueueItem.command = MAV_CMD_DO_SET_MODE;
        commandQueueItem.confirmation = 0;
        commandQueueItem.params[0] = MAV_MODE_FLAG_CUSTOM_MODE_ENABLED;
        commandQueueItem.params[1] = 0;
        commandQueueItem.params[2] = 0; // 0 for stabilize
        commandQueueItem.params[3] = 0;
        commandQueueItem.params[4] = 0;
        commandQueueItem.params[5] = 0;
        commandQueueItem.params[6] = 0;

        m_commandQueue.append(commandQueueItem);

        commandQueueItem.command_int = false;
        commandQueueItem.target_system = m_configuration.sysid;
        commandQueueItem.target_component = m_configuration.compid;
        commandQueueItem.command = MAV_CMD_COMPONENT_ARM_DISARM;
        commandQueueItem.confirmation = 0;
        commandQueueItem.params[0] = 1;
        commandQueueItem.params[1] = 0;
        commandQueueItem.params[2] = 0;
        commandQueueItem.params[3] = 0;
        commandQueueItem.params[4] = 0;
        commandQueueItem.params[5] = 0;
        commandQueueItem.params[6] = 0;

        m_commandQueue.append(commandQueueItem);

        if (m_commandQueue.count() == 2) {
            m_commandResendCount = 0;
            sendCommandInQueue();
        }

        break;
    }
    case Land: {
        if (!flying()) {
            return;
        }

        // Disarm vehicle
        CommandQueueItem commandQueueItem;

        commandQueueItem.command_int = false;
        commandQueueItem.target_system = m_configuration.sysid;
        commandQueueItem.target_component = m_configuration.compid;
        commandQueueItem.command = MAV_CMD_COMPONENT_ARM_DISARM;
        commandQueueItem.confirmation = 0;
        commandQueueItem.params[0] = 0;
        commandQueueItem.params[1] = 0;
        commandQueueItem.params[2] = 0;
        commandQueueItem.params[3] = 0;
        commandQueueItem.params[4] = 0;
        commandQueueItem.params[5] = 0;
        commandQueueItem.params[6] = 0;

        m_commandQueue.append(commandQueueItem);

        if (m_commandQueue.count() == 1) {
            m_commandResendCount = 0;
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
    if (m_commandQueue.count() == 0) {
        return;
    }

    if (m_commandResendCount++ > 10) {
        m_commandQueue.removeFirst();
        m_commandResendCount = 0;
        sendCommandInQueue();
        return;
    }

    CommandQueueItem commandQueueItem = m_commandQueue[0];
    mavlink_command_long_t command_long;

    command_long.target_system = commandQueueItem.target_system;
    command_long.target_component = commandQueueItem.target_component;
    command_long.command = commandQueueItem.command;
    command_long.confirmation = commandQueueItem.confirmation;
    command_long.param1 = commandQueueItem.params[0];
    command_long.param2 = commandQueueItem.params[1];
    command_long.param3 = commandQueueItem.params[2];
    command_long.param4 = commandQueueItem.params[3];
    command_long.param5 = commandQueueItem.params[4];
    command_long.param6 = commandQueueItem.params[5];
    command_long.param7 = commandQueueItem.params[6];

    m_commandResendTimer.start();
    m_connection->sendMessage(command_long);
}

void MAVLinkVehicle::pilot(qint8 roll, qint8 pitch, qint8 yaw, qint8 gaz)
{
    mavlink_manual_control_t manual_control;

    manual_control.target = 1;
    manual_control.x = pitch * 10; // [-1000,1000] range
    manual_control.y = roll * 10; // [-1000,1000] range
    manual_control.z = gaz * 10; // [-1000,1000] range
    manual_control.r = yaw * 10; // [-1000,1000] range
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

bool MAVLinkVehicle::gpsSupported() const
{
    return true;
}

bool MAVLinkVehicle::gpsFix() const
{
    return m_gpsFix;
}

QGeoCoordinate MAVLinkVehicle::gpsPosition() const
{
    return m_gpsPosition;
}

float MAVLinkVehicle::altitude() const
{
    return m_altitudeSource.altitude;
}

QString MAVLinkVehicle::videoSource() const
{
    return QStringLiteral(
        "rtspsrc location=rtsp://192.168.99.1/media/stream2 latency=5 ! "
        "rtph264depay ! "
        "video/x-h264 ! "
        "queue ! "
        "h264parse ! "
        "decodebin ! ");
}

void MAVLinkVehicle::fetchParameters()
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

void MAVLinkVehicle::handleHeartbeat(const mavlink_message_t &message)
{
    mavlink_heartbeat_t heartbeat;
    mavlink_msg_heartbeat_decode(&message, &heartbeat);

    // TODO: Support mavlink state more precisely.
    switch (heartbeat.system_status) {
    case MAV_STATE_UNINIT:
        setFlyingState(Kirogi::AbstractVehicle::FlyingState::Unknown);
        break;
    case MAV_STATE_STANDBY:
        setFlyingState(Kirogi::AbstractVehicle::FlyingState::Landed);
        break;
    case MAV_STATE_ACTIVE:
        setFlyingState(Kirogi::AbstractVehicle::FlyingState::Flying);
        break;
    case MAV_STATE_BOOT:
    case MAV_STATE_CALIBRATING:
    case MAV_STATE_CRITICAL:
    case MAV_STATE_EMERGENCY:
    case MAV_STATE_POWEROFF:
    case MAV_STATE_FLIGHT_TERMINATION:
        break;
    default:
        qCDebug(KIROGI_VEHICLESUPPORT_MAVLINK_PLUGIN) << QString("Received heartbeat with state %1 which is not handled.").arg(heartbeat.system_status);
    }
}

void MAVLinkVehicle::handleCommandAck(const mavlink_message_t &message)
{
    if (m_commandQueue.size() == 0) {
        qCDebug(KIROGI_VEHICLESUPPORT_MAVLINK_PLUGIN) << "Received command ack when there's no pending command in queue.";
        return;
    }

    mavlink_command_ack_t commandAck;
    mavlink_msg_command_ack_decode(&message, &commandAck);

    if (commandAck.command == m_commandQueue[0].command) {
        m_commandResendCount = 0;
        m_commandResendTimer.stop();
        m_commandQueue.removeFirst();
        // This recursive usage of function makes stack too deep. It would be better to use event loop.
        sendCommandInQueue();
    } else {
        qCDebug(KIROGI_VEHICLESUPPORT_MAVLINK_PLUGIN) << "Received unintended ack message.";
    }
}

void MAVLinkVehicle::handleRcChannelsScaled(const mavlink_message_t &message)
{
    mavlink_rc_channels_scaled_t channels;
    mavlink_msg_rc_channels_scaled_decode(&message, &channels);

    m_signalStrength = channels.rssi >= 255 ? -1 : static_cast<float>(channels.rssi) / 2.54f;

    emit signalStrengthChanged();
}

void MAVLinkVehicle::handleRcChannelsRaw(const mavlink_message_t &message)
{
    mavlink_rc_channels_raw_t channels;
    mavlink_msg_rc_channels_raw_decode(&message, &channels);

    m_signalStrength = channels.rssi >= 255 ? -1 : static_cast<float>(channels.rssi) / 2.54f;

    emit signalStrengthChanged();
}

void MAVLinkVehicle::handleRcChannels(const mavlink_message_t &message)
{
    mavlink_rc_channels_t channels;
    mavlink_msg_rc_channels_decode(&message, &channels);

    m_signalStrength = channels.rssi >= 255 ? -1 : static_cast<float>(channels.rssi) / 2.54f;

    emit signalStrengthChanged();
}

void MAVLinkVehicle::handleAltitude(const mavlink_message_t &message)
{
    mavlink_altitude_t altitude;
    mavlink_msg_altitude_decode(&message, &altitude);

    m_altitudeSource.altitudeMessage = true;
    m_altitudeSource.altitude = altitude.altitude_terrain;

    emit altitudeChanged();
}

void MAVLinkVehicle::handleAttitude(const mavlink_message_t &message)
{
    mavlink_attitude_t attitude;
    mavlink_msg_attitude_decode(&message, &attitude);

    m_yaw = attitude.yaw;
    m_roll = attitude.roll;
    m_pitch = attitude.pitch;

    emit rollChanged();
    emit pitchChanged();
    emit yawChanged();
    emit attitudeChanged();
}

void MAVLinkVehicle::handleBatteryStatus(const mavlink_message_t &message)
{
    mavlink_battery_status_t status;
    mavlink_msg_battery_status_decode(&message, &status);

    m_batteryLevel = status.battery_remaining;

    emit batteryLevelChanged();
}

void MAVLinkVehicle::handleGlobalPositionInt(const mavlink_message_t &message)
{
    mavlink_global_position_int_t position;
    mavlink_msg_global_position_int_decode(&message, &position);

    m_gpsFix = !(qFuzzyCompare(position.lat, 0.0) && qFuzzyCompare(position.lon, 0.0));
    emit gpsFixChanged();

    if (m_gpsFix) {
        m_gpsPosition.setLatitude(position.lat / 1e7);
        m_gpsPosition.setLongitude(position.lon / 1e7);
    } else {
        // Invalidate position if there is no fix
        m_gpsPosition.setLatitude(qQNaN());
        m_gpsPosition.setLongitude(qQNaN());
    }

    m_gpsPosition.setAltitude(position.alt / 1e3);
    emit gpsPositionChanged();

    // Check if altitude message was received
    if (!m_altitudeSource.altitudeMessage) {
        m_altitudeSource.altitude = m_gpsPosition.altitude();
        emit altitudeChanged();
    }
}

void MAVLinkVehicle::handleParamValue(const mavlink_message_t &message)
{
    mavlink_param_value_t paramValue;
    mavlink_msg_param_value_decode(&message, &paramValue);

    parameters()->update({
        {ParameterModel::Roles::Name, QString(paramValue.param_id)},
        {ParameterModel::Roles::Value, static_cast<float>(paramValue.param_value)},
        {ParameterModel::Roles::Index, static_cast<int>(paramValue.param_index)},
    });
}
