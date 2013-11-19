#include "query.hpp"
#include "table.hpp"
#include "driver.hpp"
#include "model.hpp"

namespace Qk
{
namespace Db
{

IQuery::IQuery(ITable *pTable, QList<IField *> pFields, Driver* pDriver)
    : mTable(pTable), mFields(pFields), mDriver(pDriver)
{
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

IQuery& IQuery::limit(quint64 pOffset, quint32 pCount)
{
    mLimitOffset = pOffset;
    mLimitCount = pCount;
    return *this;
}

RModel IQuery::one()
{
    QList<RModel> rv = mDriver->select(this);
    return rv.size() > 0 ? rv[0] : RModel();
}

}
}
