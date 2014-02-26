#pragma once

#include "core.export.hpp"
#include "logitem.hpp"
#include "logger.hpp"
#include "error.hpp"
#include "loglevel.hpp"
#include "blockqueue.hpp"

#include <QThread>

namespace Qk
{
namespace Core
{

class QKCORE_EXPORT LogBase : public QThread
{
    Q_DISABLE_COPY(LogBase)
    friend class LogItem::Data;

public:
    static QString getBacktrace(quint8 pMaxSize = 64);

public:
    LogBase();
    virtual ~LogBase();

public:
    void addStdLoggers(bool pUseColor = true);
    void addLogger(Logger* pLogger);
    void removeLogger(Logger* pLogger);
    void clearLoggers();
    void stop();

    void installQtHandler();
    void installSysSignalHandler();
    void levelMask(LogLevel pLevel);
    LogLevel levelMask() const;

protected:
    virtual void run() override;

private:
    void enqueue(LogItem& pItem);
    void direct(LogItem& pItem);

private:
    QList<Logger*> mLoggers;
    BlockQueue<LogItem> mQueue;
    ELogLevel mLevelMask = LogLevel::MaskAll;
    bool mRun = false;
};

class QKCORE_EXPORT Log
{
public:
    Log(LogBase* pLogBase, QString pSource, ELogLevel pLevel = LogLevel::MaskAll)
        : mBase(pLogBase), mLevelMask(pLevel)
    {
        source(pSource);
    }

    Log(Log* pLog, QString pSource, ELogLevel pLevel = LogLevel::MaskAll)
        : mBase(pLog->mBase), mLevelMask(pLevel)
    {
        source(pSource);
    }

    virtual ~Log()
    {
    }

public:
    void addStdLoggers(bool pUseColor = true) { mBase->addStdLoggers(pUseColor); }
    void addLogger(Logger* pLogger) { mBase->addLogger(pLogger); }
    void removeLogger(Logger* pLogger) { mBase->removeLogger(pLogger); }
    void clearLoggers() { mBase->clearLoggers(); }
    void start() { mBase->start(); }

    void installQtHandler() { mBase->installQtHandler(); }
    void installSysSignalHandler() { mBase->installSysSignalHandler(); }
    void stopwatchStart();
    void stopwatchEnd(const QString& pMessage, ELogLevel pLevel = LogLevel::Trace);
    void levelMask(LogLevel pLevel);
    LogLevel levelMask() const;
    QString source() const { return mSource; }
    void source(QString pSource);
    QString prepend() const { return mPrepend; }
    void prepend(const QString& pPrepend) { mPrepend = pPrepend; }

    LogItem trace(const QString& pMessage = "", const QString& pLocation = "")
        { return append(LogLevel::Trace, pMessage, pLocation); }
    LogItem debug(const QString& pMessage, const QString& pLocation = "")
        { return append(LogLevel::Debug, pMessage, pLocation); }
    LogItem info(const QString& pMessage, const QString& pLocation = "")
        { return append(LogLevel::Info, pMessage, pLocation); }
    LogItem warn(const QString& pMessage, const QString& pLocation = "")
        { return append(LogLevel::Warn, pMessage, pLocation); }
    LogItem error(const QString& pMessage, const QString& pLocation = "")
        { return append(LogLevel::Error, pMessage, pLocation); }
    LogItem error(const Error& pError)
        { return append(LogLevel::Error, pError.message(), pError.location()).details(pError.details()).backtrace(pError.backtrace()); }
    LogItem error(const std::exception& pError)
        { return append(LogLevel::Error, pError.what(), "std::exception").backtrace(LogBase::getBacktrace()); }
    LogItem fatal(const QString& pMessage, const QString& pLocation = "")
        { return append(LogLevel::Fatality, pMessage, pLocation); }
    LogItem fatal(const Error& pError)
        { return append(LogLevel::Fatality, pError.message(), pError.location()).details(pError.details()).backtrace(pError.backtrace()); }

    LogItem append(ELogLevel pLevel, const QString& pMessage, const QString& pLocation = "")
    {
        if (mLevelMask & pLevel) return LogItem(mBase, pLevel, mPrepend + pMessage, pLocation).source(mSource);
        else return LogItem();
    }
    virtual LogItem appendSystemInfo();

protected:
    virtual int sourceMaxSize() { return 8; }

private:
    LogBase* mBase;
    QString mSource;
    QString mPrepend;
    ELogLevel mLevelMask;
};

Log* log();

#define LogScope(pComment) LogScopeClass scope__LINE__(log(), __PRETTY_FUNCTION__, pComment);
#define LogScopeOwn(pLog, pComment) LogScopeClass scope__LINE__(pLog, __PRETTY_FUNCTION__, pComment);

class LogScopeClass
{
private:
    Log* mLog;
    QString mExitText;

public:
    LogScopeClass(Log* pLog, const char* pFunction, const char* pComment = 0)
        : mLog(pLog), mExitText(qApp->translate("", "Exit from %1. %2").arg(pFunction).arg(pComment))
    {
        mLog->trace(qApp->translate("", "Enter to %1. %2").arg(pFunction).arg(pComment)).direct();
    }

    ~LogScopeClass()
    {
        mLog->trace(mExitText).direct();
    }
};

}
}
