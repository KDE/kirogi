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

#include "ryzetelloconnection.h"
#include "debug.h"

#include <QNetworkDatagram>

RyzeTelloConnection::RyzeTelloConnection(const QString &vehicleName, QObject *parent)
    : QObject(parent)
    , m_vehicleName(vehicleName)
    , m_address(QHostAddress("192.168.10.1"))
    , m_roll(0)
    , m_pitch(0)
    , m_yaw(0)
    , m_gaz(0)
{
}

RyzeTelloConnection::~RyzeTelloConnection()
{
}

void RyzeTelloConnection::handshake()
{
    initSockets();
}

void RyzeTelloConnection::reset()
{
    m_commandQueueTimer.reset();

    m_commandQueue.clear();

    m_controlSocket->abort();
    delete m_controlSocket;

    m_stateSocket->abort();
    delete m_stateSocket;

    emit stateChanged(Kirogi::AbstractVehicle::Disconnected);
}

void RyzeTelloConnection::sendCommand(const QString &_command, bool retryForever)
{
    RyzeTelloCommand command;
    command.text = _command;

    if (retryForever) {
        command.retry = -1;
    }

    m_commandQueue.enqueue(command);

    if (!m_commandQueueTimer || !m_commandQueueTimer->isActive()) {
        pumpCommandQueue();
    }
}

void RyzeTelloConnection::pilot(qint8 roll, qint8 pitch, qint8 yaw, qint8 gaz)
{
    m_roll = roll;
    m_pitch = pitch;
    m_yaw = yaw;
    m_gaz = gaz;

    if (roll != 0 || pitch != 0 || yaw != 0 || gaz != 0) {
        if (!m_pilotingTimer) {
            m_pilotingTimer = std::make_unique<QTimer>(this);
            m_pilotingTimer->setInterval(40);
            QObject::connect(m_pilotingTimer.get(), &QTimer::timeout, this, &RyzeTelloConnection::sendPilotingCommand);
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

void RyzeTelloConnection::receiveData()
{
    while (m_controlSocket->hasPendingDatagrams()) {
        const QNetworkDatagram &datagram = m_controlSocket->receiveDatagram();

        if (datagram.isValid()) {
            const QByteArray &data = datagram.data();
            const QString &encoded = QString::fromUtf8(data);

            if (encoded != QStringLiteral("error")) {
                emit responseReceived(encoded);

                if (m_commandQueue.size() > 1) {
                    m_commandQueue.dequeue();
                    pumpCommandQueue();
                } else {
                    m_commandQueue.clear();
                }
            }
        }
    }
}

void RyzeTelloConnection::receiveState()
{
    while (m_stateSocket->hasPendingDatagrams()) {
        const QNetworkDatagram &datagram = m_stateSocket->receiveDatagram();

        if (datagram.isValid()) {
            emit stateReceived(datagram.data());
        }
    }
}

void RyzeTelloConnection::pumpCommandQueue()
{
    // We do this here instead of in the constructor so the timer is created on the
    // current thread. It's not allowed to start/stop timers across thread boundaries.
    if (!m_commandQueueTimer) {
        m_commandQueueTimer = std::make_unique<QTimer>(this);
        QObject::connect(m_commandQueueTimer.get(), &QTimer::timeout, this, &RyzeTelloConnection::pumpCommandQueue, Qt::QueuedConnection);
    }

    m_commandQueueTimer->stop();

    if (m_commandQueue.isEmpty()) {
        return;
    }

    RyzeTelloCommand &command = m_commandQueue.head();

    if (command.retry > -1) {
        if (command.retry >= 5) {
            m_commandQueue.dequeue();

            if (m_commandQueue.isEmpty()) {
                return;
            }
        } else {
            ++command.retry;
        }
    }

    m_controlSocket->writeDatagram(m_commandQueue.head().text.toUtf8(), m_address, 8889);

    // 300ms inspired by defaults in https://github.com/dji-sdk/Tello-Python
    m_commandQueueTimer->start(300);
}

void RyzeTelloConnection::sendPilotingCommand()
{
    const QByteArray &decoded =
        QString("rc %1 %2 %3 %4").arg(QString::number(m_roll), QString::number(m_pitch), QString::number(m_gaz), QString::number(m_yaw)).toUtf8();
    m_controlSocket->writeDatagram(decoded, m_address, 8889);
}

void RyzeTelloConnection::initSockets()
{
    m_controlSocket = new QUdpSocket(this);
    QObject::connect(m_controlSocket, &QUdpSocket::readyRead, this, &RyzeTelloConnection::receiveData);
    m_controlSocket->bind(QHostAddress::AnyIPv4, 8889);

    m_stateSocket = new QUdpSocket(this);
    QObject::connect(m_stateSocket, &QUdpSocket::readyRead, this, &RyzeTelloConnection::receiveState);
    m_stateSocket->bind(QHostAddress::AnyIPv4, 8890);

    emit stateChanged(Kirogi::AbstractVehicle::Connecting);
}
