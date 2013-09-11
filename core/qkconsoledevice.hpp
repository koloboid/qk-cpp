#pragma once

#include "qkcore.export.hpp"

#include <QFile>

class QKCORE_EXPORT QkConsoleDevice
{
protected:
    QkConsoleDevice();

public:
    static QFile* stdOut();
    static QFile* stdErr();
    static QFile* stdIn();
    static bool isConsoleDevice(QIODevice* pDevice);
    static void setForeColor(const Qt::GlobalColor pColor);
    static void setBackColor(const Qt::GlobalColor pColor);
    static void beep();
};
