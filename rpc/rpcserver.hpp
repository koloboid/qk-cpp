#pragma once

#include <QObject>
#include <QDir>
#include "irpchandler.hpp"

namespace Qk
{
namespace RPC
{

class RpcServer : public QObject
{
    Q_OBJECT

public:
    RpcServer();
    virtual ~RpcServer();

public:
    void run();
    bool registerHandler(const QString& pUrl, IRpcHandler* pHandler);
    bool registerStatic(const QString& pUrl, const QString& pDir);
    bool unregisterHandler(IRpcHandler* pHandler);
    bool unregisterHandler(const QString& pUrl);
};

}
}
