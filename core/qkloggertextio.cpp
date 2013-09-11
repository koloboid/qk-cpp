#include "qkloggertextio.hpp"

QkLoggerTextIO::QkLoggerTextIO(QIODevice *pDevice, QkLogLevel pLevel, bool pUseColor)
    : mDevice(pDevice), mLevelMask(pLevel)
{
    mFile = qobject_cast<QFile*>(pDevice);
    mUseColor = pUseColor && QkConsoleDevice::isConsoleDevice(pDevice);
}

void QkLoggerTextIO::writeItem(const QkLogItem &pItem)
{
    if (mLevelMask & pItem.level())
    {
        QString message = pItem.message();
        if (!pItem.details().isEmpty()) message += "\n\tDetails: " + pItem.details();
        if (!pItem.location().isEmpty()) message += "\n\tLocation: " + pItem.location();
        if (!pItem.suggest().isEmpty()) message += "\n\tSuggestion: " + pItem.suggest();
        if (!pItem.stack().isEmpty()) message += "\n\tStack: " + pItem.stack();
        foreach (const QString& name, pItem.args().keys())
        {
            message += "\n\t" + name + " = " + pItem.args()[name].toString();
        }
        QString text = QString("%1  %3\t%2  \t%4\n")
                       .arg(pItem.time().toString("dd.MM.yyyy HH:mm:ss.zzz"))
                       .arg(pItem.source())
                       .arg(pItem.level().getTitle())
                       .arg(message);

        if (mUseColor)
        {
            setConsoleColor(pItem);
        }
        mDevice->write(text.toUtf8());
        if (mFile) mFile->flush();
    }
}

void QkLoggerTextIO::setConsoleColor(const QkLogItem &pItem)
{
    QkConsoleDevice::setForeColor(Qt::white);
}
