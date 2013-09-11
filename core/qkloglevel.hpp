#if !defined(QKLOGLEVEL) || defined(ENUMIMPL)
#define QKLOGLEVEL

#include "qkenum.hpp"

QkEnumBegin(QkLogLevel, quint8, MaskNone)
    QkEnum(QkLogLevel, MaskNone,     0x00, "NONE", "")
    QkEnum(QkLogLevel, Trace,        0x01, "TRACE", "")
    QkEnum(QkLogLevel, Debug,        0x02, "DEBUG", "")
    QkEnum(QkLogLevel, Info,         0x04, "INFO", "")
    QkEnum(QkLogLevel, Warn,         0x08, "WARN", "")
    QkEnum(QkLogLevel, Assert,       0x10, "ASSERT", "")
    QkEnum(QkLogLevel, Error,        0x20, "ERROR", "")
    QkEnum(QkLogLevel, Fatality,     0x40, "FATAL", "")
    QkEnum(QkLogLevel, MaskErrors,   0x70, "ERRORS", "")
    QkEnum(QkLogLevel, MaskAll,      0xFF, "ALL", "")
QkEnumEnd

#endif
