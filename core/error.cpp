#include "error.hpp"
#include "log.hpp"

namespace Qk
{
namespace Core
{

Error::Error(const QString& pLocation, const QString& pMessage, const QString& pDetails)
    : mMessage(pMessage), mLocation(pLocation), mDetails(pDetails), mBacktrace(LogBase::getBacktrace())
{
}


}
}
