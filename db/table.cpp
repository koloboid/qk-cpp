#include "errordb.hpp"
#include "table.hpp"
#include "field.hpp"
#include "db.hpp"

using namespace Qk::Core;

namespace Qk {
namespace Db {

ITable::ITable(const QString& pName, Db* pDb)
    : mName(pName), mDb(pDb)
{
    ASSERTSTR(pName, 1, 64);
    ASSERTPTR(pDb);
}

ITable::~ITable()
{
}

void ITable::addField(IField* pField)
{
    ASSERTPTR(pField);
    ASSERTMAPUNIQKEY(mFields, pField->name());
    ASSERTMAPUNIQVAL(mFields, pField);

    mFields[pField->name()] = pField;
}

IQuery ITable::selectInternal(Driver*, QList<IField *>) const
{
    throw ErrorNotImplemented(ERRLOC);
}

void ITable::init()
{
    foreach (IField* fld, mFields.values())
    {
        if (fld->flags() & FieldFlag::Primary)
        {
            if (mPrimaryField)
            {
                throw ErrorDb(ERRLOC, TR("Таблица '%1' имеет два первичных ключа (поля '%2' и '%3'), что невозможно.")
                    .arg(name()).arg(mPrimaryField->name()).arg(fld->name()));
            }
            else mPrimaryField = fld;
        }
        if (fld->flags() & FieldFlag::Grouping)
        {
            if (mGroupField)
            {
                throw ErrorDb(ERRLOC, TR("Таблица '%1' имеет два групповых ключа (поля '%2' и '%3'), что невозможно.")
                    .arg(name()).arg(mGroupField->name()).arg(fld->name()));
            }
            else mGroupField = fld;
        }
        if (fld->flags() & FieldFlag::DisplayField)
        {
            if (mDisplayField)
            {
                dblog()->warn(TR("Таблица '%1' имеет олее одного отображаемого поля ('%2' и '%3')")
                    .arg(name()).arg(mDisplayField->name()).arg(fld->name()));
            }
            else mDisplayField = fld;
        }
        fld->init();
    }
    if (!primaryField())
    {
        throw ErrorDb(ERRLOC, TR("Таблица '%1' не имеет первичного ключа. Определите первичный ключ в схеме данных").arg(name()));
    }
}

}
}
