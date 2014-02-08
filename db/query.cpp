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

IQuery& IQuery::limit(quint32 pCount)
{
    mLimitOffset = 0;
    mLimitCount = pCount;
    return *this;
}

IQuery& IQuery::order(IField* pField, Qt::SortOrder pOrder)
{
    if (mFields.contains(pField))
    {
        mSort[pField] = pOrder;
    }
    return *this;
}

IQuery& IQuery::limit(quint64 pOffset, quint32 pCount)
{
    mLimitOffset = pOffset;
    mLimitCount = pCount;
    return *this;
}

IRow IQuery::one()
{
    QList<IRow> rv = mDriver->select(*this);
    return rv.size() > 0 ? rv[0] : IRow(*mTable, RowState::Invalid);
}

}
}
