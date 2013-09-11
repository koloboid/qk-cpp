#if !defined(QKDBROWSTATE) || defined(ENUMIMPL)
#define QKDBROWSTATE

#include <qkenum.hpp>

QkEnumBegin(QkDbRowState, quint8)
{
    QkEnum(New,             0x00, "New", "")
    QkEnum(Original,        0x01, "Original", "")
    QkEnum(Modified,        0x02, "Modified", "")
}
QkEnumEnd(QkDbRowState, New)

#endif
