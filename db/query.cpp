#include "query.hpp"
#include "table.hpp"
#include "driver.hpp"
#include "table.hpp"
#include <qk/core/error.hpp>

namespace Qk
{
namespace Db
{

IQuery::IQuery(const ITable* pTable, QList<IField*> pFields, Driver* pDriver)
    : mTable(pTable), mFields(pFields), mDriver(pDriver)
{
    ASSERTPTR(pTable);
    ASSERTPTR(pDriver);

    if (mFields.size() == 0)
    {
        mFields = pTable->fields().values();
    }
}

IQuery::~IQuery()
{
}

IRow IQuery::one()
{
    mLimitCount = 1;
    int sz = 0;
    IRow row(table());
    mDriver->query(*this, &sz, [&row](const QSqlRecord& pRecord) {
        row.load(pRecord);
    });
    return sz == 0 ? IRow(nullptr) : row;
}

QList<IRow> IQuery::list()
{
    int sz = 0;
    QList<IRow> list;
    mDriver->query(*this, &sz, [&](const QSqlRecord& pRecord) {
        IRow row(table());
        row.load(pRecord);
        list.append(row);
    });
    return list;
}

}
}
