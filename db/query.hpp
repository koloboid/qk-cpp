#pragma once

#include <QList>
#include <QSharedPointer>
#include "condition.hpp"
#include "driver.hpp"
#include "row.hpp"

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
protected:
    IQuery(const ITable* pTable, QList<IField*> pFields, Driver* pDrv);
public:
    virtual ~IQuery();

public:
    const ITable* table() const { return mTable; }
    QList<IField*> fields() const { return mFields; }
    const Condition& condition() const { return mCondition; }
    quint32 limitCount() const { return mLimitCount; }
    quint64 limitOffset() const { return mLimitOffset; }
    IRow one();
    QList<IRow> list();
    IQuery& where(const Condition& pCondition)
    {
        mCondition = pCondition;
        return *this;
    }

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
    const TTable* table() const { return static_cast<const TTable*>(mTable); }
    Query<TTable>& limit(quint64 pOffset, quint32 pCount) { mLimitOffset = pOffset; mLimitCount = pCount; return *this; }
    Query<TTable>& limit(quint32 pCount) { mLimitOffset = 0; mLimitCount = pCount; return *this; }
    template<class TField>
    Query<TTable>& order(TField& pField, Qt::SortOrder pOrder = Qt::AscendingOrder) {
        static_assert(std::is_same<typename TField::Table, TTable>::value, "Type of model for this field must be the same as row's model");
        mSort[&pField] = pOrder;
        return *this;
    }

    Query<TTable>& where(const Condition& pCondition)
    {
        mCondition = pCondition;
        return *this;
    }

    typename TTable::RowType one()
    {
        mLimitCount = 1;
        int sz = 0;
        typename TTable::RowType row(table());
        mDriver->query(*this, &sz, [&row](const QSqlRecord& pRecord) {
            row.load(pRecord);
        });
        return sz == 0 ? typename TTable::RowType(nullptr) : row;
    }

    QList<typename TTable::RowType> list()
    {
        QList<typename TTable::RowType> list;
        int sz = 0;
        mDriver->query(*this, &sz, [&](const QSqlRecord& pRecord) {
            list.reserve(sz);
            typename TTable::RowType row(table());
            row.load(pRecord);
            list.append(row);
        });
        return list;
    }

    void iterate(const std::function<void(typename TTable::RowType)>& pFunc)
    {
        mDriver->query(*this, 0, [&](const QSqlRecord& pRecord) {
            typename TTable::RowType row(table());
            row.load(pRecord);
            pFunc(row);
        });
    }
};

}
}
