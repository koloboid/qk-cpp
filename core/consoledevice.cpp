#include "consoledevice.hpp"
#include <unistd.h>

QFile* mStdOut = 0;
QFile* mStdErr = 0;
QFile* mStdIn = 0;

namespace Qk
{
namespace Core
{

ConsoleDevice::ConsoleDevice()
{
}

QFile* ConsoleDevice::stdOut()
{
    if (!mStdOut)
    {
        mStdOut = new QFile();
        mStdOut->open(STDOUT_FILENO, QIODevice::WriteOnly);
    }
    return mStdOut;
}

QFile* ConsoleDevice::stdErr()
{
    if (!mStdErr)
    {
        mStdErr = new QFile();
        mStdErr->open(STDERR_FILENO, QIODevice::WriteOnly);
    }
    return mStdErr;
}

bool ConsoleDevice::isConsoleDevice(QIODevice *pDevice)
{
    QFile* file = qobject_cast<QFile*>(pDevice);
    return file && (file->handle() == STDOUT_FILENO || file->handle() == STDERR_FILENO);
}

void ConsoleDevice::setForeColor(const Qt::GlobalColor pColor)
{
    switch (pColor)
    {
        case Qt::black:
        case Qt::white:
        case Qt::darkGray:
        case Qt::gray:
        case Qt::lightGray:
        case Qt::green:
        case Qt::blue:
        case Qt::cyan:
        case Qt::magenta:
        case Qt::yellow:
        case Qt::darkGreen:
        case Qt::darkBlue:
        case Qt::darkCyan:
        case Qt::darkMagenta:
        case Qt::darkYellow:
        case Qt::transparent:
        case Qt::darkRed:
        case Qt::red:
            stdOut()->write(QString("\033[37m").toUtf8());
            break;
        default:
            break;
    }
}

}
}
