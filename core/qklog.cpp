#include "qklog.hpp"
#include "qklock.hpp"
#include <QtConcurrent/QtConcurrent>
#include "qkconsoledevice.hpp"
#include "qkloggertextio.hpp"

QkLog* qkLog()
{
    static QkLog* log = new QkLog(new QkLogBase(), qApp->applicationName());
    return log;
}

QkLogBase::QkLogBase()
{
}

QkLogBase::~QkLogBase()
{
    if (this->isRunning())
    {
        mRun = false;
        this->wait();
    }
    qDeleteAll(mLoggers);
}

void QkLogBase::enqueue(QkLogItem& pItem)
{
    if (!mRun)
    {
//        throwNow(QkError(ERRLOC, "Unable to write message, coz QkLogBase is not running. Please start it first."));
//        return;
    }
    mQueue.enqueue(pItem);
}

void QkLogBase::direct(QkLogItem& pItem)
{
    mQueue.enqueue(pItem);
    pItem.waitFlush();
}

void QkLogBase::run()
{
    mRun = true;
    do
    {
        QkLogItem item = mQueue.dequeue(200);
        if (item.isValid())
        {
            foreach (QkLogger* logger, mLoggers)
            {
                logger->writeItem(item);
            }
            item.flushed();
        }
    }
    while (mRun);
}

void QkLogBase::stop()
{
    mRun = false;
    QkLogItem itm;
    enqueue(itm);
    wait();
}

bool QkLogBase::addStdLoggers(bool pUseColor)
{
    return addLogger(new QkLoggerTextIO(QkConsoleDevice::stdOut(), QkLogLevel::inverse(QkLogLevel::MaskErrors), pUseColor)) &&
        addLogger(new QkLoggerTextIO(QkConsoleDevice::stdErr(), QkLogLevel::MaskErrors, pUseColor));
}

bool QkLogBase::addLogger(QkLogger *pLogger)
{
    if (mRun)
    {
        return this->throwNow(QkError(ERRLOC, "Unable to change logger collection on running QkLogBase. Stop log engine before change it."));
    }
    mLoggers.append(pLogger);
    return true;
}

bool QkLogBase::removeLogger(QkLogger *pLogger)
{
    if (mRun)
    {
        return this->throwNow(QkError(ERRLOC, "Unable to change logger collection on running QkLogBase. Stop log engine before change it."));
    }
    mLoggers.removeOne(pLogger);
    delete pLogger;
    return true;
}

bool QkLogBase::clearLoggers()
{
    if (mRun)
    {
        return this->throwNow(QkError(ERRLOC, "Unable to change logger collection on running QkLogBase. Stop log engine before change it."));
    }
    mLoggers.clear();
    return true;
}
