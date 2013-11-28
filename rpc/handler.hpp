#pragma once

#include <QObject>

namespace Qk {
namespace Rpc {

class Context;

class Handler : public QObject
{
    Q_OBJECT

public:
    Handler();

public:
    virtual QString path() = 0;
    virtual void processRequest(Context* pCtx);
    virtual void init();
    virtual void newSession(Context* pCtx);
};

}
}

#ifndef Q_MOC_RUN

#define QKRPC

#endif
