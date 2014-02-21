#include "error.hpp"
#include "log.hpp"

namespace Qk
{
namespace Core
{

Error::Error()
{
}

Error::Error(const QString& pLocation, const QString& pMessage, const QString& pDetails)
    : mMessage(pMessage), mLocation(pLocation), mDetails(pDetails), mBacktrace(LogBase::getBacktrace())
{
}

Error::Error(const std::exception& pEx)
    : mMessage(pEx.what()), mLocation("std::exception"), mDetails(), mBacktrace(LogBase::getBacktrace())
{

}

}
}
