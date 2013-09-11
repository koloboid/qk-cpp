#pragma once

#include "qkcore.export.hpp"
#include "qklogitem.hpp"
#include "qklogger.hpp"
#include "qkerror.hpp"
#include "qkloglevel.hpp"
#include "qkblockqueue.hpp"
#include "qkthrowable.hpp"

#include <QThread>

class QKCORE_EXPORT QkLogBase : public QThread, public QkThrowable
{
    Q_DISABLE_COPY(QkLogBase)
    friend class QkLogItem::Data;

public:
    static QString getBacktrace(quint8 pMaxSize = 64);

public:
    QkLogBase();
    virtual ~QkLogBase();

public:
    bool addStdLoggers(bool pUseColor = true);
    bool addLogger(QkLogger* pLogger);
    bool removeLogger(QkLogger* pLogger);
    bool clearLoggers();
    void stop();

    void installQtHandler();
    void levelMask(QkLogLevel pLevel);
    QkLogLevel levelMask() const;

protected:
    virtual void run() override;

private:
    void enqueue(QkLogItem& pItem);
    void direct(QkLogItem& pItem);

private:
    QList<QkLogger*> mLoggers;
    QkBlockQueue<QkLogItem> mQueue;
    QkLogLevel mLevelMask = QkLogLevel::MaskAll;
    bool mRun = false;
};

class QKCORE_EXPORT QkLog
{
public:
    QkLog(QkLogBase* pLogBase, QString pSource, QkLogLevel pLevel = QkLogLevel::MaskAll)
        : mBase(pLogBase), mSource(pSource), mLevelMask(pLevel)
    {
    }

    QkLog(QkLog* pLog, QString pSource, QkLogLevel pLevel = QkLogLevel::MaskAll)
        : mBase(pLog->mBase), mSource(pSource), mLevelMask(pLevel)
    {
    }

public:
    void addStdLoggers(bool pUseColor = true) { mBase->addStdLoggers(pUseColor); }
    void addLogger(QkLogger* pLogger) { mBase->addLogger(pLogger); }
    void removeLogger(QkLogger* pLogger) { mBase->removeLogger(pLogger); }
    void clearLoggers() { mBase->clearLoggers(); }
    void start() { mBase->start(); }

    void installQtHandler() { mBase->installQtHandler(); }
    void stopwatchStart();
    void stopwatchEnd(const QString& pMessage, QkLogLevel pLevel = QkLogLevel::Trace);
    void levelMask(QkLogLevel pLevel);
    QkLogLevel levelMask() const;

    QkLogItem trace(const QString& pMessage = "", const QString& pLocation = "")
        { return append(QkLogLevel::Trace, pMessage, pLocation); }
    QkLogItem debug(const QString& pMessage, const QString& pLocation = "")
        { return append(QkLogLevel::Debug, pMessage, pLocation); }
    QkLogItem info(const QString& pMessage, const QString& pLocation = "")
        { return append(QkLogLevel::Info, pMessage, pLocation); }
    QkLogItem warn(const QString& pMessage, const QString& pLocation = "")
        { return append(QkLogLevel::Warn, pMessage, pLocation); }
    QkLogItem error(const QString& pMessage, const QString& pLocation = "")
        { return append(QkLogLevel::Error, pMessage, pLocation); }
    QkLogItem fatal(const QString& pMessage, const QString& pLocation = "")
        { return append(QkLogLevel::Fatality, pMessage, pLocation); }

    QkLogItem append(QkLogLevel pLevel, const QString pMessage, const QString pLocation = "")
    {
        if (mLevelMask.hasOneOf(pLevel)) return QkLogItem(mBase, pLevel, pMessage, pLocation).source(mSource);
        else return QkLogItem();
    }

private:
    QkLogBase* mBase;
    QString mSource;
    QkLogLevel mLevelMask;
};

QkLog* qkLog();

#define QkLogScope(pComment) QkLogScopeClass scope__LINE__(qkLog(), __PRETTY_FUNCTION__, pComment);
#define QkLogScopeOwn(pLog, pComment) QkLogScopeClass scope__LINE__(pLog, __PRETTY_FUNCTION__, pComment);

class QkLogScopeClass
{
private:
    QkLog* mLog;
    QString mExitText;

public:
    QkLogScopeClass(QkLog* pLog, const char* pFunction, const char* pComment = 0)
        : mLog(pLog), mExitText(qApp->translate("", "Exit from %1. %2").arg(pFunction).arg(pComment))
    {
        mLog->trace(qApp->translate("", "Enter to %1. %2").arg(pFunction).arg(pComment));
    }

    ~QkLogScopeClass()
    {
        mLog->trace(mExitText);
    }
};
