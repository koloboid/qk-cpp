#pragma once

#include <QObject>
#include <QStringList>
#include <QBuffer>
#include <QElapsedTimer>
#include <qk/core/log.hpp>
#include <qk/core/formatter.hpp>
#include "session.hpp"

using namespace Qk::Core;

namespace Qk {
namespace Rpc {

extern Log* rpclog(Log* pParent = 0);

class Server;
class Handler;

class Context : public QObject
{
    Q_OBJECT

    friend class RpcRunnable;
    friend class RpcFuncRunnable;
    friend class Server;
    friend class Handler;

public:
    Context(Server* pServer);
    virtual ~Context();

public:
    virtual QUuid sessionId() = 0;
    virtual QString path() = 0;
    bool isServerThread();

    void doAsync(const std::function<void(Context*)>& pFunc);
    void respondError(const Error& pError, quint32 pStatusCode = 500) { respondError(pError.message(), pStatusCode); }
    void respondError(const QString& pError, quint32 pStatusCode = 500);

    Server* server() const { return mServer; }
    Session* session() const { return mSession; }
    QString statusText() const;
    quint32 statusCode() const { return mStatusCode; }
    Log* log() { return &mLog; }
    Formatter* out() { ASSERTPTR(mOut); return mOut; }
    Handler* handler() const { return mHandler; }
    qint64 elapsed() const { return mTimer.elapsed(); }
    quint32 requestID() const { return mRequestID; }
    template<class T>
    T in(const char* pKey, const char* pAlias = 0) const;
    template<class T>
    T inRequire(const char* pKey, const char* pAlias = 0) const;

protected:
    virtual void start(quint32 pRequestID);
    virtual void finish();
    virtual void handlerStart(Handler* pHandler);
    virtual QString requestContentType() const = 0;
    virtual QByteArray requestRawData() const = 0;
    void setSession(Session* pSession);
    bool hasAsyncCall() const { return mHasAsyncCall; }
    void hasAsyncCall(bool pHas) { mHasAsyncCall = pHas; }
    void updateLogHeader();

protected:
    QStringList mRespondErrors;
    quint32 mStatusCode = 200;
    QBuffer mResponseBuffer;
    QMap<QString, QVariant> mRequestData;
    LogItem mFinishLogItem;

private:
    Session* mSession = nullptr;
    Formatter* mOut = nullptr;
    Server* mServer = nullptr;
    Handler* mHandler = nullptr;
    bool mHasAsyncCall = false;
    quint32 mRequestID;
    Log mLog;
    QElapsedTimer mTimer;
};

template<class T>
T Context::in(const char* pKey, const char* pAlias) const
{
    QVariant v = mRequestData.value(pKey);
    if (!v.isValid() && pAlias)
    {
        return mRequestData.value(pAlias).value<T>();
    }
    else
    {
        return v.value<T>();
    }
}

template<class T>
T Context::inRequire(const char* pKey, const char* pAlias) const
{
    QVariant v = mRequestData.value(pKey);
    if (v.isValid())
    {
        return v.value<T>();
    }
    else
    {
        if (pAlias)
        {
            v = mRequestData.value(pAlias);
            if (v.isValid()) return v.value<T>();
        }
        throw Error(ERRLOC, tr("Required argument '%1' is missing").arg(pKey));
    }
}

}
}
