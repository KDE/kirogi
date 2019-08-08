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

#ifndef PARROTCONNECTION_H
#define PARROTCONNECTION_H

#include <QObject>

#include "parrotprotocol.h"
#include "parrotvehicle.h"

#include <QHostAddress>
#include <QJsonObject>
#include <QPointer>
#include <QQueue>
#include <QTcpSocket>
#include <QTimer>
#include <QUdpSocket>

class ParrotConnection : public QObject
{
    Q_OBJECT

    public:
        explicit ParrotConnection(ParrotVehicle::Type type, const QString &vehicleName,
            const QString &hostName, int port, QObject *parent = nullptr);
        ~ParrotConnection();

    public Q_SLOTS:
        void handshake(const QString &productSerial = QString());
        void reset();
        void sendCommand(Parrot::Command command, const QVariantList &arguments,
            bool retryForever = false);
        void pilot(qint8 roll, qint8 pitch, qint8 yaw, qint8 gaz);

    Q_SIGNALS:
        void stateChanged(Kirogi::AbstractVehicle::ConnectionState state) const;
        void commandReceived(const ParrotCommand &command) const;

    private Q_SLOTS:
        void receiveData();
        void pumpC2dAckQueue();
        void sendPilotingCommand();

    private:
        void initSockets();

        void processIncomingFrame(const ParrotFrame &frame);

        void sendAck(const ParrotFrame &frame);
        void sendFrame(const ParrotFrame &frame);
        void sendData(const QByteArray &data, quint32 size);

        quint8 makeSeq(quint8 bufferId);

        ParrotVehicle::Type m_type;
        QString m_vehicleName;

        QHostAddress m_hostAddress;
        int m_port;

        QPointer<QTcpSocket> m_handshakeSocket;
        QJsonObject m_handshakeResponse;

        // c2d = Controller-to-device, i.e. we send here.
        int m_c2dport;
        // d2c = Device-to-Controller, i.e. we listen here.
        int m_d2cPort;

        QPointer<QUdpSocket> m_d2cSocket;
        QPointer<QUdpSocket> m_c2dSocket;

        QHash<quint8, quint8> m_seq;

        QQueue<ParrotFrame> m_c2dAckQueue;
        QTimer *m_c2dAckTimer;

        QTimer *m_pilotingTimer;
        qint8 m_roll;
        qint8 m_pitch;
        qint8 m_yaw;
        qint8 m_gaz;
};

#endif
