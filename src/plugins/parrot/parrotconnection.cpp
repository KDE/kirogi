/*
 * Copyright 2019 Eike Hein <hein@kde.org>
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

#include "parrotconnection.h"
#include "debug.h"

#include <QDataStream>
#include <QHostInfo>
#include <QJsonDocument>
#include <QNetworkDatagram>

ParrotConnection::ParrotConnection(ParrotVehicle::Type type, const QString &vehicleName, const QString &hostName, int port, QObject *parent)
    : QObject(parent)
    , m_type(type)
    , m_vehicleName(vehicleName)
    , m_port(port)
    , m_c2dport(-1)
    , m_d2cPort(54321)
    , m_roll(0)
    , m_pitch(0)
    , m_yaw(0)
    , m_gaz(0)
{
#ifndef Q_OS_ANDROID
    const QHostInfo &hostInfo = QHostInfo::fromName(hostName);

    if (!hostInfo.addresses().isEmpty()) {
        m_hostAddress = hostInfo.addresses().at(0);
        qCDebug(KIROGI_VEHICLESUPPORT_PARROT) << m_vehicleName << "Resolved host name to:" << m_hostAddress;
    } else {
        qCWarning(KIROGI_VEHICLESUPPORT_PARROT) << "Fatal: Unable to resolve host name to address.";
    }
#else
    Q_UNUSED(hostName)

    // FIXME TODO: QHostInfo::fromName() doesn't seem to work on Android.
    m_hostAddress = QHostAddress("192.168.42.1");
#endif
}

ParrotConnection::~ParrotConnection() = default;

void ParrotConnection::handshake(const QString &productSerial)
{
    // No point shaking hands if we don't have what we need to communicate later.
    if (m_hostAddress.isNull()) {
        qCWarning(KIROGI_VEHICLESUPPORT_PARROT) << m_vehicleName << "Fatal: Cannot handshake without host address.";

        return;
    }

    emit stateChanged(Kirogi::AbstractVehicle::Connecting);

    m_handshakeSocket = new QTcpSocket(this);

    QObject::connect(m_handshakeSocket, &QTcpSocket::connected, this, [this, productSerial]() {
        QJsonObject obj;
        obj.insert(QStringLiteral("d2c_port"), m_d2cPort);                        // FIXME TODO: Don't hardcode the receive port. Current choice taken from Parrot example docs.
        obj.insert(QStringLiteral("controller_type"), QStringLiteral("desktop")); // FIXME TODO: Send different type based on host platform.
        obj.insert(QStringLiteral("controller_name"), QStringLiteral("KirogiParrotPlugin"));
        obj.insert(QStringLiteral("arstream2_client_stream_port"), 55004);
        obj.insert(QStringLiteral("arstream2_client_control_port"), 55005);
        obj.insert(QStringLiteral("arstream2_supported_metadata_version"), 1);

        if (!productSerial.isEmpty()) {
            obj.insert(QStringLiteral("device_id"), productSerial);
        }

        qCDebug(KIROGI_VEHICLESUPPORT_PARROT) << m_vehicleName << "Sending handshake data:" << obj;

        QJsonDocument handshakeDoc;
        handshakeDoc.setObject(obj);

        m_handshakeSocket->write(handshakeDoc.toJson(QJsonDocument::Compact));
    });

    QObject::connect(m_handshakeSocket, &QTcpSocket::disconnected, this, [this]() { m_handshakeSocket->deleteLater(); });

    QObject::connect(m_handshakeSocket, &QTcpSocket::readyRead, this, [this]() {
        const QByteArray &responseData = m_handshakeSocket->readAll();

        QJsonParseError error;

        // For Bebop 2 chopping by 1 byte, response data is \x00-terminated and the parser considers
        // this a fatal "garbage at end" error.
        const QJsonDocument &handshakeResponse = QJsonDocument::fromJson(m_type == ParrotVehicle::Bebop2 ? responseData.chopped(1) : responseData, &error);

        qDebug() << handshakeResponse;

        if (!handshakeResponse.isNull() && handshakeResponse.isObject()) {
            m_handshakeResponse = handshakeResponse.object();
        } else {
            qCWarning(KIROGI_VEHICLESUPPORT_PARROT) << m_vehicleName << "Unable to decode handshake response.";
        }

        const int vehicleStatus = m_handshakeResponse.value(QStringLiteral("status")).toInt();

        if (vehicleStatus != 0) {
            qCWarning(KIROGI_VEHICLESUPPORT_PARROT) << m_vehicleName << "Handshake unsuccessful. Vehicle status:";
            emit stateChanged(Kirogi::AbstractVehicle::Disconnected);
        } else {
            qCDebug(KIROGI_VEHICLESUPPORT_PARROT) << m_vehicleName << "Handshake response:" << m_handshakeResponse;

            m_c2dport = m_handshakeResponse.value(QStringLiteral("c2d_port")).toInt();
            qCDebug(KIROGI_VEHICLESUPPORT_PARROT) << m_vehicleName << "Got Controller-to-Device (c2d) port:" << m_c2dport;

            qCDebug(KIROGI_VEHICLESUPPORT_PARROT) << m_vehicleName << "Handshake successful.";

            initSockets();
        }

        m_handshakeSocket->disconnectFromHost();
    });

    QObject::connect(m_handshakeSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, [this]() {
        qCWarning(KIROGI_VEHICLESUPPORT_PARROT) << m_vehicleName << "Handshake failed with socket error:" << m_handshakeSocket->errorString();
        emit stateChanged(Kirogi::AbstractVehicle::Disconnected);
        m_handshakeSocket->abort();
    });

    qCDebug(KIROGI_VEHICLESUPPORT_PARROT) << m_vehicleName << "Performing handshake ...";

    m_handshakeSocket->connectToHost(m_hostAddress, m_port);
}

void ParrotConnection::reset()
{
    qCDebug(KIROGI_VEHICLESUPPORT_PARROT) << m_vehicleName << "Resetting connection.";

    if (m_handshakeSocket) {
        m_handshakeSocket->abort();
        m_handshakeSocket->deleteLater();
    }

    m_c2dAckTimer.reset();

    m_c2dAckQueue.clear();

    m_d2cSocket->abort();
    delete m_d2cSocket;
    m_c2dSocket->abort();
    delete m_c2dSocket;

    emit stateChanged(Kirogi::AbstractVehicle::Disconnected);
}

void ParrotConnection::sendCommand(Parrot::Command _command, const QVariantList &arguments, bool retryForever)
{
    ParrotCommand command(_command, arguments);

    // FIXME Generi-fy.
    int bufferId = Parrot::BUFFER_C2D_NON_ACK_DATA;

    if (_command != Parrot::Ardrone3PilotingPCMD) {
        bufferId = Parrot::BUFFER_C2D_ACK_DATA;
    }

    ParrotFrame frame(Parrot::DATA_WITH_ACK, bufferId, makeSeq(bufferId), command.pack());

    if (retryForever) {
        frame.retry = -1;
    }

    sendFrame(frame);
}

void ParrotConnection::pilot(qint8 roll, qint8 pitch, qint8 yaw, qint8 gaz)
{
    m_roll = roll;
    m_pitch = pitch;
    m_yaw = yaw;
    m_gaz = gaz;

    if (roll != 0 || pitch != 0 || yaw != 0 || gaz != 0) {
        if (!m_pilotingTimer) {
            m_pilotingTimer = std::make_unique<QTimer>(this);
            m_pilotingTimer->setInterval(40);
            QObject::connect(m_pilotingTimer.get(), &QTimer::timeout, this, &ParrotConnection::sendPilotingCommand);
        }

        if (!m_pilotingTimer->isActive()) {
            sendPilotingCommand();
        }

        m_pilotingTimer->start();
    } else if (m_pilotingTimer) {
        m_pilotingTimer->stop();
        sendPilotingCommand();
    }
}

void ParrotConnection::receiveData()
{
    while (m_d2cSocket->hasPendingDatagrams()) {
        const QNetworkDatagram &datagram = m_d2cSocket->receiveDatagram();

        if (datagram.isValid()) {
            const QByteArray &data = datagram.data();

            int bytesRead = 0;

            // There may be more than one frame in a datagram.
            while ((data.size() - bytesRead) >= Parrot::HEADER_FRAME) {
                ParrotFrame frame(data, bytesRead);
                bytesRead += frame.size;
                processIncomingFrame(frame);
            }
        }
    }
}

void ParrotConnection::pumpC2dAckQueue()
{
    // We do this here instead of in the constructor so the timer is created on the
    // current thread. It's not allowed to start/stop timers across thread boundaries.
    if (!m_c2dAckTimer) {
        m_c2dAckTimer = std::make_unique<QTimer>(this);
        QObject::connect(m_c2dAckTimer.get(), &QTimer::timeout, this, &ParrotConnection::pumpC2dAckQueue, Qt::QueuedConnection);
    }

    m_c2dAckTimer->stop();

    if (m_c2dAckQueue.isEmpty()) {
        return;
    }

    ParrotFrame &frame = m_c2dAckQueue.head();

    // Skip over frames that have been re-sent n times and went unacknowledged. They
    // are considered lost.
    // NOTE: According to Parrot docs, the number of retries is per-buffer. There's
    // one buffer for controller-to-device ACK data however, and it's documented with
    // a retry attempt value of 5.
    if (frame.retry != -1) {
        if (frame.retry >= 5) {
            qCWarning(KIROGI_VEHICLESUPPORT_PARROT) << m_vehicleName << "Frame lost!";

            m_c2dAckQueue.dequeue();

            if (m_c2dAckQueue.isEmpty()) {
                return;
            }

            frame = m_c2dAckQueue.head();
        } else {
            ++frame.retry;
        }
    }

    sendData(frame.pack(), frame.size);

    // NOTE: According to Parrot docs, the ACK timeout is theoretically per-buffer;
    // however, they currently use 150ms for all buffers in practice.
    m_c2dAckTimer->start(150);
}

void ParrotConnection::sendPilotingCommand()
{
    const quint8 flag = 1;
    const quint32 timestampAndSeqNum = 0;
    sendCommand(Parrot::Ardrone3PilotingPCMD, {flag, m_roll, m_pitch, m_yaw, m_gaz, timestampAndSeqNum});
}

void ParrotConnection::initSockets()
{
    qCDebug(KIROGI_VEHICLESUPPORT_PARROT) << m_vehicleName << "Initializing UDP socket ...";

    m_d2cSocket = new QUdpSocket(this);
    QObject::connect(m_d2cSocket, &QUdpSocket::readyRead, this, &ParrotConnection::receiveData);

    m_c2dSocket = new QUdpSocket(this);

    m_d2cSocket->bind(QHostAddress::AnyIPv4, m_d2cPort);

    emit stateChanged(Kirogi::AbstractVehicle::Connected);
}

void ParrotConnection::processIncomingFrame(const ParrotFrame &frame)
{
    // Respond to PING with PONG.
    if (frame.bufferId == Parrot::BUFFER_PING) {
        sendFrame(ParrotFrame(Parrot::DATA, Parrot::BUFFER_PONG, makeSeq(Parrot::BUFFER_PONG), frame.data));

        return;
    }

    // Check if this is an ACK for a frame in our outbound queue;
    // if it is, pump the queue.
    if (frame.bufferId == Parrot::BUFFER_D2C_ACK_FOR_C2D_ACK_DATA) {
        if (m_c2dAckQueue.isEmpty()) {
            return;
        }

        QDataStream s(frame.data);
        s.setByteOrder(QDataStream::LittleEndian);

        quint8 seq;

        s >> seq;

        if (seq == m_c2dAckQueue.head().seq) {
            m_c2dAckQueue.dequeue();
            pumpC2dAckQueue();
        }

        return;
    }

    // Respond with ACK if requested.
    if (frame.dataType == Parrot::DATA_WITH_ACK) {
        sendAck(frame);
    }

    if (frame.size - Parrot::HEADER_FRAME >= Parrot::HEADER_COMMAND) {
        emit commandReceived(frame.command());
    }
}

void ParrotConnection::sendAck(const ParrotFrame &frame)
{
    quint8 targetBuffer = (frame.bufferId + 128) % 256;

    QByteArray data;
    data.reserve(1);

    QDataStream s(&data, QIODevice::WriteOnly);
    s.setByteOrder(QDataStream::LittleEndian);

    s << frame.seq;

    sendFrame(ParrotFrame(Parrot::ACK, targetBuffer, makeSeq(targetBuffer), data));
}

void ParrotConnection::sendFrame(const ParrotFrame &frame)
{
    switch (frame.bufferId) {
    case Parrot::BUFFER_C2D_ACK_DATA: {
        m_c2dAckQueue.enqueue(frame);

        if (!m_c2dAckTimer || !m_c2dAckTimer->isActive()) {
            pumpC2dAckQueue();
        }

        break;
    }
    default: {
        sendData(frame.pack(), frame.size);
    }
    }
}

void ParrotConnection::sendData(const QByteArray &data, quint32 size)
{
    if (!m_c2dSocket) {
        qCWarning(KIROGI_VEHICLESUPPORT_PARROT) << m_vehicleName << "Trying to send data without socket.";

        return;
    }

    m_c2dSocket->writeDatagram(data.constData(), size, m_hostAddress, m_c2dport);
}

quint8 ParrotConnection::makeSeq(quint8 bufferId)
{
    if (!m_seq.contains(bufferId)) {
        m_seq[bufferId] = 0;
    } else {
        m_seq[bufferId] += 1;
        m_seq[bufferId] %= 256;
    }

    return m_seq[bufferId];
}
