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
class IModel;
typedef QSharedPointer<IModel> RModel;
template<class TModel>
class Table;

class IQuery
{
public:
    IQuery(ITable* pTable, QList<IField*> pFields, Driver* pDrv);
    virtual ~IQuery();

public:
    ITable* table() const { return mTable; }
    QList<IField*> fields() const { return mFields; }
    const Condition& condition() const { return mCondition; }
    quint32 limitCount() const { return mLimitCount; }
    quint64 limitOffset() const { return mLimitOffset; }
    IQuery& limit(quint64 pOffset, quint32 pCount);
    IQuery& limit(quint32 pCount);
    RModel one();

protected:
    ITable* mTable = 0;
    QList<IField*> mFields;
    Condition mCondition;
    quint32 mLimitCount = 0;
    quint64 mLimitOffset = 0;
    Driver* mDriver;
};

template<class TModel>
class Query : public IQuery
{
public:
    Query(Table<TModel>* pTable, QList<IField*> pFields, Driver* pDrv)
        : IQuery(pTable, pFields, pDrv)
    {
    }

public:
    Query<TModel>& where(const Condition& pCondition)
    {
        mCondition = pCondition;
        return *this;
    }
    QSharedPointer<TModel> one()
    {
        return qSharedPointerDynamicCast<TModel>(IQuery::one());
    }
};

}
}
