#pragma once

#include "core.export.hpp"
#include "logitem.hpp"

namespace Qk
{
namespace Core
{

class QKCORE_EXPORT Logger
{
public:
    Logger()
    {
    }

    virtual ~Logger()
    {
    }

public:
    virtual void writeItem(const LogItem& pItem) = 0;
};

}
}
