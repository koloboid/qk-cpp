#if !defined(QKLOGLEVEL) || defined(ENUMIMPL)
#define QKLOGLEVEL

#include "qkenum.hpp"

QkEnumBegin(QkLogLevel, quint8)
{
    QkEnum(MaskNone,     0x00, "NONE", "")
    QkEnum(Trace,        0x01, "TRACE", "")
    QkEnum(Debug,        0x02, "DEBUG", "")
    QkEnum(Info,         0x04, "INFO", "")
    QkEnum(Warn,         0x08, "WARN", "")
    QkEnum(Assert,       0x10, "ASSERT", "")
    QkEnum(Error,        0x20, "ERROR", "")
    QkEnum(Fatality,     0x40, "FATAL", "")
    QkEnum(MaskErrors,   0x70, "ERRORS", "")
    QkEnum(MaskAll,      0xFF, "ALL", "")
}
QkEnumEnd(QkLogLevel, MaskNone)

#endif
