#include "loggertextio.hpp"

namespace Qk
{
namespace Core
{

LoggerTextIO::LoggerTextIO(QIODevice *pDevice, ELogLevel pLevel, bool pUseColor)
    : mDevice(pDevice), mLevelMask(pLevel)
{
    mFile = qobject_cast<QFile*>(pDevice);
    mUseColor = pUseColor && ConsoleDevice::isConsoleDevice(pDevice);
}

void LoggerTextIO::writeItem(const LogItem &pItem)
{
    if (mLevelMask & pItem.level())
    {
        QString message = pItem.message();
        if (!pItem.details().isEmpty()) message += "\n\tDetails: " + pItem.details();
        if (!pItem.location().isEmpty()) message += "\n\tLocation: " + pItem.location();
        if (!pItem.suggest().isEmpty()) message += "\n\tSuggestion: " + pItem.suggest();
        if (!pItem.stack().isEmpty()) message += "\n\tBacktrace: " + pItem.stack();
        foreach (const QString& name, pItem.args().keys())
        {
            message += "\n\t" + name + " = " + pItem.args()[name].toString();
        }
        QString text = QString("%1  %3\t%2  \t%4\n")
                       .arg(pItem.time().toLocalTime().toString("dd.MM.yyyy HH:mm:ss.zzz"))
                       .arg(pItem.source())
                       .arg(LogLevel::getTitle(pItem.level()).toUpper())
                       .arg(message);

        if (mUseColor)
        {
            setConsoleColor(pItem);
        }
        mDevice->write(text.toUtf8());
        if (mFile) mFile->flush();
    }
}

void LoggerTextIO::setConsoleColor(const LogItem&)
{
    ConsoleDevice::setForeColor(Qt::white);
}

}
}
