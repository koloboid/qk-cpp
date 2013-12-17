#pragma once

#include <QSqlRecord>
#include <qk/core/error.hpp>
#include "table.hpp"
#include "field.hpp"
#include "value.hpp"
#include "query.hpp"
#include "db.hpp"

using namespace Qk::Core;

namespace Qk {
namespace Db {

class IModel;
typedef QSharedPointer<IModel> RModel;

class IModel
{
public:
    IModel(ITable* pTable);
    virtual ~IModel() { }

public:
    ITable* table() const { return mTable; }
    bool isLoaded() const { return mIsLoaded; }
    QVariant get(const IField* pField) const { return pField->get(this); }
    void set(const IField* pField, const QVariant& pValue) const { return pField->set(this, pValue); }

    virtual void load(const QSqlRecord& pRec);
    virtual void save(Driver* pDrv);
    virtual QList<Error> validate() const;

private:
    ITable* mTable = 0;
    bool mIsLoaded = false;
};

template<class TModel>
class Model : public IModel
{
public:
    static Table<TModel> TableSchema;
    static Query<TModel> select(Driver* pDriver = 0, QList<IField*> pFields = QList<IField*>())
    {
        Driver* drv = pDriver ? pDriver : TableSchema.db()->drv();
        return Query<TModel>(&TableSchema, pFields, drv);
    }
    static inline void initTable()
    {
    }

public:
    Model()
        : IModel(&TableSchema)
    {
    }
};

}
}

Q_DECLARE_METATYPE(Qk::Db::RModel)
