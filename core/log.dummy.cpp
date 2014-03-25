#include "log.hpp"

namespace Qk {
namespace Core {

QString LogBase::getBacktrace(quint8)
{
    return "Backtrace is not supported on this platform";
}

}
}
