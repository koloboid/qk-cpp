#include "rpcserver.hpp"
#include <qk/core/error.hpp>

namespace Qk
{
namespace RPC
{

RpcServer::RpcServer()
{

}

RpcServer::~RpcServer()
{

}

void RpcServer::run()
{
    throw Qk::Core::ErrorNotImplemented(ERRLOC);
}

}
}
