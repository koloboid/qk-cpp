#pragma once

#include "qkcore.export.hpp"
#include "qkloglevel.hpp"

#include <QString>
#include <QVariant>
#include <QDateTime>
#include <QWaitCondition>

class QkLogBase;

class QKCORE_EXPORT QkLogItem
{
    friend class QkLogBase;

public:
    class Data;

public:
    QkLogItem()
        : mData(new Data())
    {
    }

    QkLogItem(QkLogBase* pLog, QkLogLevel pLevel, const QString& pMessage, const QString& pLocation)
        : mData(new Data(pLog, pLevel, pMessage, pLocation))
    {
    }

    QkLogItem(Data* pData)
        : mData(pData)
    {
    }

public:
    QkLogItem& message(const QString& pMessage) { mData->mMessage = pMessage; return *this; }
    QkLogItem& suggest(const QString& pSuggestion) { mData->mSuggest = pSuggestion; return *this; }
    QkLogItem& details(const QString& pDetails) { mData->mDetails = pDetails; return *this; }
    QkLogItem& location(const QString& pLocation) { mData->mLocation = pLocation; return *this; }
    QkLogItem& code(quint32 pCode) { mData->mCode = pCode; return *this; }
    QkLogItem& source(const QString& pSource) { mData->mSource = pSource; return *this; }
    QkLogItem& arg(const QString& pName, const QVariant pValue) { mData->mArgs[pName] = pValue; return *this; }
    QkLogItem& appendStack();
    QkLogItem& level(QkLogLevel pLevel) { mData->mLevel = pLevel; return *this; }
    QkLogItem& time(QDateTime pTime) { mData->mTime = pTime; return *this; }
    void direct();

    QString message() const { return mData->mMessage; }
    QString suggest() const { return mData->mSuggest; }
    QString details() const { return mData->mDetails; }
    QString location() const { return mData->mLocation; }
    quint32 code() const { return mData->mCode; }
    QString source() const { return mData->mSource; }
    QString stack() const { return mData->mStack; }
    QVariantMap args() const { return mData->mArgs; }
    QkLogLevel level() const { return mData->mLevel; }
    QDateTime time() const { return mData->mTime; }
    bool isValid() const { return mData->mLevel != QkLogLevel::MaskNone; }

public:
    class Data : public QSharedData
    {
    public:
        QkLogBase* mLog = nullptr;
        QkLogLevel mLevel = QkLogLevel::MaskNone;
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

        Data(QkLogBase* pLog, QkLogLevel pLevel, const QString& pMessage, const QString& pLocation)
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
