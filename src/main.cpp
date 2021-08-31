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

#include "autosettingshandler.h"
#include "gstreamer/gstreamerintegration.h"
#include "permissions.h"
#include "settings.h"

// Remove warnings related to gstreamer library
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#else
#pragma warning(push, 0)
#endif
#include <gst/gst.h>
#ifdef __GNUC__
#pragma GCC diagnostic pop
#else
#pragma warning(pop, 0)
#endif

#include <KAboutData>
#ifndef Q_OS_ANDROID
#include <KCrash>
#endif
#include <KLocalizedContext>
#include <KLocalizedString>

#ifdef Q_OS_ANDROID
#include <QGuiApplication>
#include <QtAndroid>
#else
#include <QApplication>
#endif

#include <QCommandLineParser>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickView>

#ifdef Q_OS_ANDROID
extern "C" gboolean gst_qt_android_init(GError **error);

Q_DECL_EXPORT
#endif
int main(int argc, char *argv[])
{
    int ret;

#ifndef Q_OS_ANDROID
    gst_init(&argc, &argv);
#else
    if (!gst_qt_android_init(nullptr)) {
        return -1;
    }
#endif
    GStreamerIntegration::init();

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

#ifdef Q_OS_ANDROID
    QGuiApplication app(argc, argv);
#else
    QApplication app(argc, argv);
#endif

    KLocalizedString::setApplicationDomain("kirogi");

    KAboutData aboutData(QStringLiteral("kirogi"),
                         xi18nc("@title", "<application>Kirogi</application>"),
                         QStringLiteral("0.1-dev"),
                         xi18nc("@title", "A ground control application for drones."),
                         KAboutLicense::GPL,
                         xi18nc("@info:credit", "(c) 2019 The Kirogi Team"),
                         QString(),
                         QStringLiteral("https://www.kirogi.org/"));

    aboutData.setOrganizationDomain(QByteArray("kde.org"));
    aboutData.setProductName(QByteArray("kirogi"));

    aboutData.addAuthor(xi18nc("@info:credit", "Eike Hein"), xi18nc("@info:credit", "Founder, Lead Developer"), QStringLiteral("hein@kde.org"));

    aboutData.addAuthor(xi18nc("@info:credit", "Patrick José Pereira"), xi18nc("@info:credit", "Developer"), QStringLiteral("patrickjp@kde.org"));

    aboutData.addAuthor(xi18nc("@info:credit", "Rafael Brandmaier"),
                        xi18nc("@info:credit", "Application icon"),
                        QStringLiteral("rafael.brandmaier@kdemail.net"));

    aboutData.addAuthor(xi18nc("@info:credit", "L. 'AsmoArael' C."), xi18nc("@info:credit", "Mascot artwork"), QStringLiteral("lc.jarryh99@outlook.fr"));

    KAboutData::setApplicationData(aboutData);
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    app.setApplicationName(aboutData.componentName());
    app.setApplicationDisplayName(aboutData.displayName());
    app.setOrganizationDomain(aboutData.organizationDomain());
    app.setApplicationVersion(aboutData.version());
    app.setWindowIcon(QIcon::fromTheme(QStringLiteral("kirogi")));

#ifndef Q_OS_ANDROID
    KCrash::initialize();
#endif

    QQmlApplicationEngine engine(&app);
    AutoSettingsHandler settingsHandler;
    settingsHandler.addSettings(Settings::self());

    /* Add relative path to import paths to cover the case when the application
     * is not installed in the system. */
    engine.addImportPath(QCoreApplication::applicationDirPath() + "/../lib/qml");

    // For i18n.
    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));

    engine.rootContext()->setContextProperty(QStringLiteral("kirogiAboutData"), QVariant::fromValue(aboutData));

    engine.rootContext()->setContextProperty(QStringLiteral("kirogiSettings"), Settings::self());

    engine.rootContext()->setContextProperty(
        QStringLiteral("locationPermissions"),
        QVariant::fromValue(
            new Permissions({QStringLiteral("android.permission.ACCESS_COARSE_LOCATION"), QStringLiteral("android.permission.ACCESS_FINE_LOCATION")}, &app)));

    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    if (engine.rootObjects().isEmpty()) {
        qCritical() << "Application failed to load.";
        return -1;
    }

#ifdef Q_OS_ANDROID
    QtAndroid::hideSplashScreen();
#endif

    ret = app.exec();
    return ret;
}
