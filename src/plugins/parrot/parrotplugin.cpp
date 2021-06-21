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

#include "parrotplugin.h"
#include "debug.h"
#include "parrotprotocol.h"
#include "parrotvehicle.h"

#include <KLocalizedString>
#include <KPluginFactory>

#include <QJsonDocument>

#ifndef Q_OS_ANDROID
#include <kdnssd_version.h>
#if KDNSSD_VERSION >= QT_VERSION_CHECK(5, 84, 0)
#include <KDNSSD/RemoteService>
#include <KDNSSD/ServiceBrowser>
#else
#include <DNSSD/RemoteService>
#include <DNSSD/ServiceBrowser>
#endif
#else
#include "QtZeroConf/qzeroconf.h"
#include "QtZeroConf/qzeroconfservice.h"
#endif

ParrotPlugin::ParrotPlugin(QObject *parent, const QVariantList & /* args */)
    : Kirogi::VehicleSupportPlugin(parent)
#ifndef Q_OS_ANDROID
    , m_bebop2Browser(nullptr)
    , m_anafiBrowser(nullptr)
#endif
{
    qCDebug(KIROGI_VEHICLESUPPORT_PARROT) << "Parrot Vehicle Support Plugin initializing ...";

    qRegisterMetaType<Parrot::Command>("Parrot::Command");
    qRegisterMetaType<ParrotCommand>();

    class AbstractMDNSService
    {
    public:
#ifndef Q_OS_ANDROID
        AbstractMDNSService(KDNSSD::RemoteService::Ptr service)
        {
            name = service->serviceName();
            host = service->hostName();
            port = service->port();

            if (!service->textData().isEmpty()) {
                data = QJsonDocument::fromJson(service->textData().firstKey().toUtf8());
            }
        }
#else
        AbstractMDNSService(QZeroConfService service)
        {
            name = service->name();
            host = service->host();
            port = service->port();

            if (!service->txt().isEmpty()) {
                data = QJsonDocument::fromJson(service->txt().firstKey());
            }
        }
#endif

        QString name;
        QString host;
        uint port;
        QJsonDocument data;
    };

    auto serviceAdded = [this](AbstractMDNSService *service, ParrotVehicle::Type type) {
        if (!m_vehicles.contains(service->name)) {
            QString productSerial;

            if (!service->data.isNull() || !service->data.isObject()) {
                productSerial = service->data.object().value(QStringLiteral("device_id")).toString();
            } else {
                qCWarning(KIROGI_VEHICLESUPPORT_PARROT) << "mDNS service has no text data. We will be unable to decode the product serial.";
            }

            if (productSerial.isEmpty()) {
                qCWarning(KIROGI_VEHICLESUPPORT_PARROT) << "Unable to decode product serial from mDNS service text data.";
            }

            qCDebug(KIROGI_VEHICLESUPPORT_PARROT) << "Discovered new vehicle at:" << QString("%1:%2.").arg(service->name, QString::number(service->port));

            ParrotVehicle *vehicle = new ParrotVehicle(type, service->host, service->port, productSerial, this);
            m_vehicles.insert(service->name, vehicle);
            emit vehicleAdded(vehicle);

            vehicle->connectToVehicle();
        } else {
            qCWarning(KIROGI_VEHICLESUPPORT_PARROT) << "Already have a vehicle for:" << service->name;
        }
    };

    auto serviceRemoved = [this](AbstractMDNSService *service) {
        qCDebug(KIROGI_VEHICLESUPPORT_PARROT) << "Removing vehicle at" << QString("%1:%2.").arg(service->host, QString::number(service->port));

        Kirogi::AbstractVehicle *vehicle = m_vehicles.value(service->name);

        m_vehicles.remove(service->name);
        emit vehicleRemoved(vehicle);

        delete vehicle;
    };

#ifndef Q_OS_ANDROID
    m_bebop2Browser = new KDNSSD::ServiceBrowser(QStringLiteral("_arsdk-090c._udp"), true);
    m_anafiBrowser = new KDNSSD::ServiceBrowser(QStringLiteral("_arsdk-0914._udp"), true);

    if (m_bebop2Browser->isAvailable() != KDNSSD::ServiceBrowser::Working) {
        qCWarning(KIROGI_VEHICLESUPPORT_PARROT) << "KDNSSD mDNS browsing (Bebop 2) unavailable:" << m_bebop2Browser->isAvailable();
    }

    if (m_anafiBrowser->isAvailable() != KDNSSD::ServiceBrowser::Working) {
        qCWarning(KIROGI_VEHICLESUPPORT_PARROT) << "KDNSSD mDNS browsing (Anafi) unavailable:" << m_anafiBrowser->isAvailable();
    }

    QObject::connect(m_bebop2Browser, &KDNSSD::ServiceBrowser::serviceAdded, this, [serviceAdded](KDNSSD::RemoteService::Ptr service) {
        serviceAdded(new AbstractMDNSService(service), ParrotVehicle::Bebop2);
    });
    QObject::connect(m_bebop2Browser, &KDNSSD::ServiceBrowser::serviceRemoved, this, [serviceRemoved](KDNSSD::RemoteService::Ptr service) {
        serviceRemoved(new AbstractMDNSService(service));
    });
    QObject::connect(m_anafiBrowser, &KDNSSD::ServiceBrowser::serviceAdded, this, [serviceAdded](KDNSSD::RemoteService::Ptr service) {
        serviceAdded(new AbstractMDNSService(service), ParrotVehicle::Anafi);
    });
    QObject::connect(m_anafiBrowser, &KDNSSD::ServiceBrowser::serviceRemoved, this, [serviceRemoved](KDNSSD::RemoteService::Ptr service) {
        serviceRemoved(new AbstractMDNSService(service));
    });

    m_bebop2Browser->startBrowse();
    m_anafiBrowser->startBrowse();
#else
    QZeroConf *bebop2Browser = new QZeroConf(this);
    QZeroConf *anafiBrowser = new QZeroConf(this);

    QObject::connect(bebop2Browser, &QZeroConf::serviceAdded, this, [serviceAdded](QZeroConfService service) {
        serviceAdded(new AbstractMDNSService(service), ParrotVehicle::Bebop2);
    });
    QObject::connect(bebop2Browser, &QZeroConf::serviceRemoved, this, [serviceRemoved](QZeroConfService service) {
        serviceRemoved(new AbstractMDNSService(service));
    });
    QObject::connect(bebop2Browser, &QZeroConf::error, this, [](QZeroConf::error_t error) {
        qCWarning(KIROGI_VEHICLESUPPORT_PARROT) << "QZeroConf mDNS browsing (Bebop 2) error:" << error;
    });
    QObject::connect(anafiBrowser, &QZeroConf::serviceAdded, this, [serviceAdded](QZeroConfService service) {
        serviceAdded(new AbstractMDNSService(service), ParrotVehicle::Anafi);
    });
    QObject::connect(anafiBrowser, &QZeroConf::serviceRemoved, this, [serviceRemoved](QZeroConfService service) {
        serviceRemoved(new AbstractMDNSService(service));
    });
    QObject::connect(anafiBrowser, &QZeroConf::error, this, [](QZeroConf::error_t error) {
        qCWarning(KIROGI_VEHICLESUPPORT_PARROT) << "QZeroConf mDNS browsing (Anafi) error:" << error;
    });

    bebop2Browser->startBrowser(QStringLiteral("_arsdk-090c._udp"));
    anafiBrowser->startBrowser(QStringLiteral("_arsdk-0914._udp"));
#endif
}

ParrotPlugin::~ParrotPlugin()
{
#ifndef Q_OS_ANDROID
    delete m_bebop2Browser;
    delete m_anafiBrowser;
#endif

    qCDebug(KIROGI_VEHICLESUPPORT_PARROT) << "Parrot Vehicle Support Plugin unloaded.";
}

QList<Kirogi::AbstractVehicle *> ParrotPlugin::vehicles() const
{
    return m_vehicles.values();
}

K_PLUGIN_CLASS_WITH_JSON(ParrotPlugin, "kirogiparrotplugin.json")

#include "parrotplugin.moc"
