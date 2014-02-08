#pragma once

#include "enum.hpp"

namespace Qk
{
namespace Core
{

class LogLevel : public Enum
{
    Q_OBJECT

public:
    enum Value
    {
        MaskNone        = 0x00,
        Trace           = 0x01,
        Debug           = 0x02,
        Info            = 0x04,
        Warn            = 0x08,
        Assert          = 0x10,
        Error           = 0x20,
        Fatality        = 0x40,
        MaskErrors      = 0x70,
        MaskAll         = 0xFF,
    };

    QKENUM(Value)
};
QKENUMDECL(LogLevel::Value, ELogLevel)

}
}
