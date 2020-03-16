#pragma once

#include <QtGlobal>

#include "kirogicore_export.h"
#include "settings.h"

namespace Kirogi
{
class KIROGI_EXPORT JoystickConfiguration
{
public:
    static void pilot(qint8 &roll, qint8 &pitch, qint8 &yaw, qint8 &gaz);

private:
    explicit JoystickConfiguration();
    ~JoystickConfiguration();
};

}