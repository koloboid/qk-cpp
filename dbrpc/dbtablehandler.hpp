#pragma once

#include <qk/rpc/irpchandler.hpp>

using namespace Qk::RPC;

namespace Qk
{
namespace DbRpc
{

class DbTableHandler : public IRpcHandler
{
public:
    DbTableHandler();
};

}
}
