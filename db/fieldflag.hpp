#pragma once

#include <qk/core/enum.hpp>

namespace Qk {
namespace Db {

class FieldFlag : public Qk::Core::Enum
{
    Q_OBJECT

public:
    enum Value
    {
        Simple =               0x00000000,
        Primary =              0x00000001,
        IDGenerator =          0x00000002,
        Counter =              0x00000008,
        Filterable =           0x00000010,
        IterableJoin =         0x00000020,
        Reserved =             0x00000040,
        Virtual =              0x00000080,
        RefWeak =              0x00000100,
        RefStrong =            0x00000200,
        Password =             0x00000400,
        AutoIncrement =        0x00000801,
        Invisible =            0x00001000,
        InvisibleJoin =        0x00002000,
        CascadeDelete =        0x00004000,
        HistoryDisabled =      0x00008000,
        DontSelectDefault =    0x00010000,
        ReadOnly =             0x00020000,
        AllowNull =            0x00040000,
        NonSerializable =      0x00080000,
        CaseInsensitive =      0x00100000,
    };

    QKFLAG(Value)
};
QKENUMDECL(FieldFlag::Value, EFieldFlag)

}
}
