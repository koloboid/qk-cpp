#pragma once

#include "core.export.hpp"

#include <QFile>

namespace Qk
{
namespace Core
{

class QKCORE_EXPORT ConsoleDevice
{
protected:
    ConsoleDevice();

public:
    static QFile* stdOut();
    static QFile* stdErr();
    static QFile* stdIn();
    static bool isConsoleDevice(QIODevice* pDevice);
    static void setForeColor(const Qt::GlobalColor pColor);
    static void setBackColor(const Qt::GlobalColor pColor);
    static void beep();
};

}
}
