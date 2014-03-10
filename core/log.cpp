#include "log.hpp"
#include "lock.hpp"
#include "logitem.hpp"
#include <QtConcurrent/QtConcurrent>
#include "consoledevice.hpp"
#include "loggertextio.hpp"
#include <QDir>
#include <QHostInfo>

Qk::Core::Log* qtLog = nullptr;

void qkMessageHandler(QtMsgType pType, const QMessageLogContext& pContext, const QString& pMsg)
{
    Qk::Core::Log* l = qtLog ? qtLog : Qk::Core::log();
    QString location = QString("%1:%2").arg(pContext.file).arg(pContext.line);
    switch (pType)
    {
        case QtDebugMsg:
            l->debug(pMsg).location(location).direct();
            break;
        case QtWarningMsg:
            l->warn(pMsg).location(location).direct();
            break;
        case QtCriticalMsg:
            l->error(pMsg).location(location).direct();
            break;
        case QtFatalMsg:
            l->fatal(pMsg).location(location).direct();
            break;
    }
}

namespace Qk
{
namespace Core
{

Log* log()
{
    static Log* log = new Log(new LogBase(), qApp->applicationName());
    return log;
}

LogBase::LogBase()
{
}

LogBase::~LogBase()
{
    if (this->isRunning())
    {
        mRun = false;
        this->wait();
    }
    qDeleteAll(mLoggers);
}

void LogBase::enqueue(LogItem& pItem)
{
    if (!isRunning())
    {
        throw Error(500, ERRLOC, TR("Невозможно записать сообщение в журнал, т.к. он не запущен. Запустите его вызовом LogBase::start()"));
    }
    mQueue.enqueue(pItem);
}

void LogBase::direct(LogItem& pItem)
{
    mQueue.enqueue(pItem);
    pItem.waitFlush();
}

void LogBase::run()
{
    mRun = true;
    do
    {
        LogItem item = mQueue.dequeue(200);
        if (item.isValid())
        {
            foreach (Logger* logger, mLoggers)
            {
                logger->writeItem(item);
            }
            item.flushed();
        }
    }
    while (mRun);
}

void LogBase::stop()
{
    mRun = false;
    LogItem itm;
    enqueue(itm);
    wait();
}

void LogBase::addStdLoggers(bool pUseColor)
{
    addLogger(new LoggerTextIO(ConsoleDevice::stdOut(), (ELogLevel)~LogLevel::MaskErrors, pUseColor));
    addLogger(new LoggerTextIO(ConsoleDevice::stdErr(), LogLevel::MaskErrors, pUseColor));
}

void LogBase::addLogger(Logger *pLogger)
{
    if (mRun)
    {
        throw Error(500, ERRLOC, TR("Невозможно изменить набор логгеров при работающем движке журнала. Остановите его, вызовом LogBase::stop()"));
    }
    mLoggers.append(pLogger);
}

void LogBase::removeLogger(Logger *pLogger)
{
    if (mRun)
    {
        throw Error(500, ERRLOC, TR("Невозможно изменить набор логгеров при работающем движке журнала. Остановите его, вызовом LogBase::stop()"));
    }
    mLoggers.removeOne(pLogger);
    delete pLogger;
}

void LogBase::clearLoggers()
{
    if (mRun)
    {
        throw Error(500, ERRLOC, TR("Невозможно изменить набор логгеров при работающем движке журнала. Остановите его, вызовом LogBase::stop()"));
    }
    mLoggers.clear();
}

Core::LogItem Core::Log::appendSystemInfo()
{
    return info("System information:")
        .arg("Application", qApp->applicationName())
        .arg("Version", qApp->applicationVersion())
        .arg("PID", qApp->applicationPid())
        .arg("Path", qApp->applicationFilePath())
        .arg("Current dir", QDir::currentPath())
        .arg("Hostname", QHostInfo::localHostName())
        .arg("Domain", QHostInfo::localDomainName());
}

void Log::source(QString pSource)
{
    mSource = pSource.left(sourceMaxSize());
    int sz = sourceMaxSize() - mSource.size();
    for (int i = 0; i < sz; i++) mSource += " ";
}

void LogBase::installQtHandler()
{
    if (qtLog) delete qtLog;
    qtLog = new Log(this, "QT");
    qInstallMessageHandler(qkMessageHandler);
}

}
}
