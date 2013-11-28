#pragma once

#include <QObject>
#include <QStringList>
#include <qk/core/log.hpp>
#include <qk/core/formatter.hpp>
#include "session.hpp"

using namespace Qk::Core;

namespace Qk {
namespace Rpc {

class Context : public QObject
{
    Q_OBJECT

    friend class RpcRunnable;
    friend class Server;

public:
    Context();

public:
    virtual QUuid sessionId() = 0;
    virtual QString path() = 0;

    void respondError(const Error& pError, quint32 pStatusCode = 500) { respondError(pError.toString(), pStatusCode); }
    void respondError(const QString& pError, quint32 pStatusCode = 500);

    Session* session() const { return mSession; }
    QString statusText() const;
    quint32 statusCode() const { return mStatusCode; }
    Log* log() const { return session() ? session()->log() : log(); }
    virtual Formatter* out() = 0;

protected:
    virtual void finish() = 0;
    void setSession(Session* pSession);

protected:
    QStringList mRepondErrors;
    quint32 mStatusCode = 200;

private:
    Session* mSession = nullptr;
};

}
}
