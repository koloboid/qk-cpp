#if !defined(QKDBFIELDFLAG) || defined(ENUMIMPL)

#define QKDBFIELDFLAG \
    QKENUM(Simple,               0x00000000, "", "") \

#include "qkenum.hpp"


QkEnumBegin(QkDbFieldFlag, quint32)
{
    QkEnum(Simple,               0x00000000, "", "")
    QkEnum(Primary,              0x00000001, "", "")
    QkEnum(IDGenerator,          0x00000002, "", "")
    QkEnum(Counter,              0x00000008, "", "")
    QkEnum(Filterable,           0x00000010, "", "")
    QkEnum(IterableJoin,         0x00000020, "", "")
    QkEnum(Reserved,             0x00000040, "", "")
    QkEnum(Virtual,              0x00000080, "", "")
    QkEnum(RefWeak,              0x00000100, "", "")
    QkEnum(RefStrong,            0x00000200, "", "")
    QkEnum(Password,             0x00000400, "", "")
    QkEnum(AutoIncrement,        0x00000801, "", "")
    QkEnum(Invisible,            0x00001000, "", "")
    QkEnum(InvisibleJoin,        0x00002000, "", "")
    QkEnum(CascadeDelete,        0x00004000, "", "")
    QkEnum(HistoryDisabled,      0x00008000, "", "")
    QkEnum(DontSelectDefault,    0x00010000, "", "")
}
QkEnumEnd(QkDbFieldFlag, Simple)

#endif
