#include <qk/core/error.hpp>
#include "table.hpp"
#include "field.hpp"

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

void ITable::init()
{
    foreach (IField* fld, mFields.values())
    {
        if (fld->flags() & FieldFlag::Primary)
        {
            if (mPrimaryField)
            {
                throw Error(ERRLOC, TR("Таблица '%1' имеет два первичных ключа (поля '%1' и '%2'), что невозможно.")
                    .arg(mPrimaryField->name()).arg(fld->name()));
            }
            else mPrimaryField = fld;
        }
        fld->init();
    }
    if (!primaryField())
    {
        throw Error(ERRLOC, TR("Таблица '%1' не имеет первичного ключа. Определите первичный ключ в схеме данных").arg(name()));
    }
}

}
}
