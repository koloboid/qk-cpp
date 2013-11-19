#if !defined(QKDBFIELDFLAG) || defined(ENUMIMPL)

#include <qk/core/enum.hpp>

EnumBegin(FieldFlag, quint32, Simple)
    Enum(FieldFlag, Simple,               0x00000000, "", "")
    Enum(FieldFlag, Primary,              0x00000001, "", "")
    Enum(FieldFlag, IDGenerator,          0x00000002, "", "")
    Enum(FieldFlag, Counter,              0x00000008, "", "")
    Enum(FieldFlag, Filterable,           0x00000010, "", "")
    Enum(FieldFlag, IterableJoin,         0x00000020, "", "")
    Enum(FieldFlag, Reserved,             0x00000040, "", "")
    Enum(FieldFlag, Virtual,              0x00000080, "", "")
    Enum(FieldFlag, RefWeak,              0x00000100, "", "")
    Enum(FieldFlag, RefStrong,            0x00000200, "", "")
    Enum(FieldFlag, Password,             0x00000400, "", "")
    Enum(FieldFlag, AutoIncrement,        0x00000801, "", "")
    Enum(FieldFlag, Invisible,            0x00001000, "", "")
    Enum(FieldFlag, InvisibleJoin,        0x00002000, "", "")
    Enum(FieldFlag, CascadeDelete,        0x00004000, "", "")
    Enum(FieldFlag, HistoryDisabled,      0x00008000, "", "")
    Enum(FieldFlag, DontSelectDefault,    0x00010000, "", "")
    Enum(FieldFlag, ReadOnly,             0x00020000, "", "")
EnumEnd

#endif
