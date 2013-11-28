#pragma once

#include <QObject>

namespace Qk {
namespace Rpc {

class Context;

class Transport : public QObject
{
    Q_OBJECT

public:
    Transport();
    virtual ~Transport();

public:
    virtual void init();
    virtual void run() = 0;
    virtual void stop() = 0;
    virtual void pause() = 0;

signals:
    void onRequest(Transport* pFrom, Context* pCtx);
};

}
}
