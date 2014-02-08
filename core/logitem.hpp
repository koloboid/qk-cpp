#pragma once

#include "core.export.hpp"
#include "loglevel.hpp"

#include <QString>
#include <QVariant>
#include <QDateTime>
#include <QWaitCondition>

namespace Qk
{
namespace Core
{

class LogBase;

class QKCORE_EXPORT LogItem
{
    friend class LogBase;

public:
    class Data;

public:
    LogItem()
        : mData(new Data())
    {
    }

    LogItem(LogBase* pLog, ELogLevel pLevel, const QString& pMessage, const QString& pLocation)
        : mData(new Data(pLog, pLevel, pMessage, pLocation))
    {
    }

    LogItem(Data* pData)
        : mData(pData)
    {
    }

public:
    LogItem& message(const QString& pMessage) { mData->mMessage = pMessage; return *this; }
    LogItem& suggest(const QString& pSuggestion) { mData->mSuggest = pSuggestion; return *this; }
    LogItem& details(const QString& pDetails) { mData->mDetails = pDetails; return *this; }
    LogItem& location(const QString& pLocation) { mData->mLocation = pLocation; return *this; }
    LogItem& code(quint32 pCode) { mData->mCode = pCode; return *this; }
    LogItem& source(const QString& pSource) { mData->mSource = pSource; return *this; }
    template<class T>
    LogItem& arg(const QString& pName, const T& pValue) { mData->mArgs[pName] = QVariant::fromValue<T>(pValue); return *this; }
    LogItem& arg(const QVariant& pValue) { mData->mMessage = mData->mMessage.arg(pValue.toString()); return *this; }
    LogItem& backtrace(const QString& pBacktrace = QString());
    LogItem& level(ELogLevel pLevel) { mData->mLevel = pLevel; return *this; }
    LogItem& time(QDateTime pTime) { mData->mTime = pTime; return *this; }
    void direct();

    QString message() const { return mData->mMessage; }
    QString suggest() const { return mData->mSuggest; }
    QString details() const { return mData->mDetails; }
    QString location() const { return mData->mLocation; }
    quint32 code() const { return mData->mCode; }
    QString source() const { return mData->mSource; }
    QString stack() const { return mData->mStack; }
    QVariantMap args() const { return mData->mArgs; }
    ELogLevel level() const { return mData->mLevel; }
    QDateTime time() const { return mData->mTime; }
    bool isValid() const { return mData->mLevel != LogLevel::MaskNone; }

public:
    class Data : public QSharedData
    {
    public:
        LogBase* mLog = nullptr;
        ELogLevel mLevel = LogLevel::MaskNone;
        QString mMessage;
        QString mSuggest;
        QString mDetails;
        QString mSource;
        QString mLocation;
        QVariantMap mArgs;
        quint32 mCode = 0;
        QString mStack;
        QDateTime mTime;
        bool mDirect = false;
        QMutex* mFlushMutex = nullptr;
        QWaitCondition* mFlushCond = nullptr;
        bool mFlushed = false;

    public:
        Data()
        {
        }

        Data(LogBase* pLog, ELogLevel pLevel, const QString& pMessage, const QString& pLocation)
            : mLog(pLog), mLevel(pLevel), mMessage(pMessage), mLocation(pLocation),
              mTime(QDateTime::currentDateTimeUtc())
        {
        }

        ~Data();
    };

private:
    QExplicitlySharedDataPointer<Data> mData;
    void waitFlush();
    void flushed();
};

}
}
