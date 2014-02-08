#include "transport.hpp"

namespace Qk {
namespace Rpc {

Transport::Transport(Server* pServer)
    : mRpcServer(pServer)
{
}

Transport::~Transport()
{
}

void Transport::init()
{
}

}
}
