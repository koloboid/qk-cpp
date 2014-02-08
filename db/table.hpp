#pragma once

#include "row.hpp"
#include "field.hpp"
#include "query.hpp"

namespace Qk {
namespace Db {

class Db;

class ITable
{
protected:
    ITable(const QString& pName, Db* pDb);

public:
    virtual ~ITable();

public:
    virtual void init();

    QString name() const { return mName; }
    Db* db() const { return mDb; }
    QMap<QString, IField*> fields() const { return mFields; }
    IField* field(const QString& pName) const { return mFields.value(pName); }
    IField* primaryField() const { return mPrimaryField; }
    virtual IRow rowFromVariant(const QVariant& pVariant) const = 0;
    virtual IRow newRow() const = 0;
    virtual QString engine() const { return QString(); }

protected:
    void addField(IField* pField);

private:
    QMap<QString, IField*> mFields;
    QString mName;
    Db* mDb = 0;
    IField* mPrimaryField = 0;
};

template<class TTable, class TRow>
class Table : public ITable
{
public:
    typedef TRow Row;

public:
    Table(Db* pDb)
        : ITable(TTable::tableName, pDb)
    {
    }

public:
    template<class TType, bool TReadOnly>
    void addField(Field<TTable, TType, TReadOnly>* pFld)
    {
        ITable::addField(pFld);
    }

    Query<TTable> select(Driver* pDriver = 0, QList<IField*> pFields = QList<IField*>()) const __attribute__((warn_unused_result))
    {
        Driver* drv = pDriver ? pDriver : this->db()->drv();
        return Query<TTable>(static_cast<const TTable*>(this), pFields, drv);
    }

    virtual IRow rowFromVariant(const QVariant& pVariant) const override
    {
        return pVariant.value<TRow>();
    }

    virtual IRow newRow() const override
    {
        return TRow(*this);
    }
};

}
}
