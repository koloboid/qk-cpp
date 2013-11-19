#if !defined(QKLOGLEVEL) || defined(ENUMIMPL)
#define QKLOGLEVEL

#include "enum.hpp"

namespace Qk
{
namespace Core
{

EnumBegin(LogLevel, quint8, MaskNone)
    Enum(LogLevel, MaskNone,     0x00, "NONE", "")
    Enum(LogLevel, Trace,        0x01, "TRACE", "")
    Enum(LogLevel, Debug,        0x02, "DEBUG", "")
    Enum(LogLevel, Info,         0x04, "INFO", "")
    Enum(LogLevel, Warn,         0x08, "WARN", "")
    Enum(LogLevel, Assert,       0x10, "ASSERT", "")
    Enum(LogLevel, Error,        0x20, "ERROR", "")
    Enum(LogLevel, Fatality,     0x40, "FATAL", "")
    Enum(LogLevel, MaskErrors,   0x70, "ERRORS", "")
    Enum(LogLevel, MaskAll,      0xFF, "ALL", "")
EnumEnd

}
}

#endif
