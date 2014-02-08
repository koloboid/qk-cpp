#pragma once

#include <QList>
#include <QSharedPointer>
#include "condition.hpp"

namespace Qk
{
namespace Db
{

class ITable;
class IField;
class Driver;
class IRow;

class IQuery
{
public:
    IQuery(const ITable* pTable, QList<IField*> pFields, Driver* pDrv);
    virtual ~IQuery();

public:
    const ITable* table() const { return mTable; }
    QList<IField*> fields() const { return mFields; }
    const Condition& condition() const { return mCondition; }
    quint32 limitCount() const { return mLimitCount; }
    quint64 limitOffset() const { return mLimitOffset; }
    IQuery& limit(quint64 pOffset, quint32 pCount);
    IQuery& limit(quint32 pCount);
    IQuery& order(IField* pField, Qt::SortOrder pOrder = Qt::AscendingOrder);
    IRow one();

protected:
    const ITable* mTable = 0;
    QList<IField*> mFields;
    Condition mCondition;
    quint32 mLimitCount = 0;
    quint64 mLimitOffset = 0;
    QMap<IField*, Qt::SortOrder> mSort;
    Driver* mDriver;
};

template<class TTable>
class Query : public IQuery
{
public:
    Query(const TTable* pTable, QList<IField*> pFields, Driver* pDrv)
        : IQuery(pTable, pFields, pDrv)
    {
    }

public:
    Query<TTable>& where(const Condition& pCondition)
    {
        mCondition = pCondition;
        return *this;
    }
    typename TTable::Row one()
    {
        return typename TTable::Row(IQuery::one());
    }
};

}
}
