#if !defined(QKDBROWSTATE) || defined(ENUMIMPL)
#define QKDBROWSTATE

#include <qkenum.hpp>

QkEnumBegin(QkDbRowState, quint8, New)
    QkEnum(QkDbRowState, New,             0x00, "New", "")
    QkEnum(QkDbRowState, Original,        0x01, "Original", "")
    QkEnum(QkDbRowState, Modified,        0x02, "Modified", "")
QkEnumEnd

#endif
