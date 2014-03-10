#include "field.hpp"
#include "table.hpp"
#include "db.hpp"

namespace Qk {
namespace Db {

IField::IField(ITable* pTable, const QString& pName, QMetaType::Type pType, EFieldFlag pFlags, bool pReadOnly)
    : mName(pName), mFlag(pFlags), mTable(pTable), mType(pType), mReadOnly(pReadOnly)
{
    ASSERTPTR(pTable);
    ASSERTSTR(pName, 1, 64);
    if (pType == QMetaType::QString)
    {
        mMinVariant = 0;
        mMaxVariant = 255;
        if ((mFlag & FieldFlag::AllowNull) == 0)
        {
            mDefaultVariant = "";
        }
    }
}

IField::~IField()
{
}

QString IField::tableName() const
{
    return table()->name();
}

QMetaType::Type IField::underylingType() const
{
    if (linkedTo())
    {
        return linkedTo()->primaryField()->underylingType();
    }
    else if (isEnum())
    {
        switch (sizeOf())
        {
            case 1:
                return QMetaType::UChar;
            case 2:
                return QMetaType::UShort;
            case 4:
                return QMetaType::UInt;
            case 8:
                return QMetaType::ULongLong;
            default:
                return QMetaType::Void;
        }
    }
    else return type();
}

void IField::initLinkedTo(const char* pTabName)
{
    mLinkedTo = table()->db()->getTable(pTabName);
    if (mLinkedTo)
    {
        if (!(mFlag & FieldFlag::RefStrong || mFlag & FieldFlag::RefWeak))
        {
            mFlag = mFlag | FieldFlag::RefStrong;
        }
    }
}

}
}
