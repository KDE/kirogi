#include "joystickconfiguration.h"

#include <QEasingCurve>

namespace Kirogi
{
JoystickConfiguration::JoystickConfiguration()
{
}

JoystickConfiguration::~JoystickConfiguration()
{
}

void Kirogi::JoystickConfiguration::pilot(qint8 &roll, qint8 &pitch, qint8 &yaw, qint8 &gaz)
{
    if (Settings::self()->exponentialFitting()) {
        QEasingCurve expo(QEasingCurve::InExpo);

        roll = (roll > 0) ? expo.valueForProgress(static_cast<qreal>(roll) / static_cast<qreal>(100)) * 100 : -expo.valueForProgress(static_cast<qreal>(-roll) / static_cast<qreal>(100)) * 100;
        pitch = (pitch > 0) ? expo.valueForProgress(static_cast<qreal>(pitch) / static_cast<qreal>(100)) * 100 : -expo.valueForProgress(static_cast<qreal>(-pitch) / static_cast<qreal>(100)) * 100;
        yaw = (yaw > 0) ? expo.valueForProgress(static_cast<qreal>(yaw) / static_cast<qreal>(100)) * 100 : -expo.valueForProgress(static_cast<qreal>(yaw) / static_cast<qreal>(100)) * 100;
        gaz = (gaz > 0) ? expo.valueForProgress(static_cast<qreal>(gaz) / static_cast<qreal>(100)) * 100 : -expo.valueForProgress(static_cast<qreal>(-gaz) / static_cast<qreal>(100)) * 100;
    }
}

}