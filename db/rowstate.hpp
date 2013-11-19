#if !defined(QKDBROWSTATE) || defined(ENUMIMPL)
#define QKDBROWSTATE

#include <qk/core/enum.hpp>

EnumBegin(RowState, quint8, New)
    Enum(RowState, New,             0x00, "New", "")
    Enum(RowState, Original,        0x01, "Original", "")
    Enum(RowState, Modified,        0x02, "Modified", "")
EnumEnd

#endif
