#if !defined(QKDBFIELDFLAG) || defined(ENUMIMPL)

#include "qkenum.hpp"

QkEnumBegin(QkDbFieldFlag, quint32, Simple)
    QkEnum(QkDbFieldFlag, Simple,               0x00000000, "", "")
    QkEnum(QkDbFieldFlag, Primary,              0x00000001, "", "")
    QkEnum(QkDbFieldFlag, IDGenerator,          0x00000002, "", "")
    QkEnum(QkDbFieldFlag, Counter,              0x00000008, "", "")
    QkEnum(QkDbFieldFlag, Filterable,           0x00000010, "", "")
    QkEnum(QkDbFieldFlag, IterableJoin,         0x00000020, "", "")
    QkEnum(QkDbFieldFlag, Reserved,             0x00000040, "", "")
    QkEnum(QkDbFieldFlag, Virtual,              0x00000080, "", "")
    QkEnum(QkDbFieldFlag, RefWeak,              0x00000100, "", "")
    QkEnum(QkDbFieldFlag, RefStrong,            0x00000200, "", "")
    QkEnum(QkDbFieldFlag, Password,             0x00000400, "", "")
    QkEnum(QkDbFieldFlag, AutoIncrement,        0x00000801, "", "")
    QkEnum(QkDbFieldFlag, Invisible,            0x00001000, "", "")
    QkEnum(QkDbFieldFlag, InvisibleJoin,        0x00002000, "", "")
    QkEnum(QkDbFieldFlag, CascadeDelete,        0x00004000, "", "")
    QkEnum(QkDbFieldFlag, HistoryDisabled,      0x00008000, "", "")
    QkEnum(QkDbFieldFlag, DontSelectDefault,    0x00010000, "", "")
QkEnumEnd

#endif
