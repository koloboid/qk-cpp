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
    IField* groupField() const { return mGroupField; }
    IField* displayField() const { return mDisplayField; }
    virtual IRow rowFromVariant(const QVariant& pVariant) const = 0;
    virtual QVariant rowToVariant(const IRow& pRow) const = 0;
    virtual IRow newRow() const = 0;
    virtual QString engine() const { return QString(); }
    IQuery select(Driver* pDriver = 0, QList<IField*> pFields = QList<IField*>()) const { return selectInternal(pDriver, pFields); }

protected:
    void addField(IField* pField);
    virtual IQuery selectInternal(Driver* pDriver, QList<IField*> pFields) const;

private:
    QMap<QString, IField*> mFields;
    QString mName;
    Db* mDb = 0;
    IField* mPrimaryField = 0;
    IField* mGroupField = 0;
    IField* mDisplayField = 0;
};

template<class TTable, class TRow>
class Table : public ITable
{
public:
    typedef TRow RowType;

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

    Query<TTable> select(Driver* pDriver = 0) const
    {
        Driver* drv = pDriver ? pDriver : this->db()->drv();
        return Query<TTable>(static_cast<const TTable*>(this), QList<IField*>(), drv);
    }

    virtual IRow rowFromVariant(const QVariant& pVariant) const override
    {
        return pVariant.value<TRow>();
    }

    virtual QVariant rowToVariant(const IRow& pRow) const override
    {
        return QVariant::fromValue<TRow>(TRow(pRow));
    }

    virtual IRow newRow() const override
    {
        return TRow(this);
    }

protected:
    virtual IQuery selectInternal(Driver* pDriver = 0, QList<IField*> pFields = QList<IField*>()) const override
    {
        Driver* drv = pDriver ? pDriver : this->db()->drv();
        return Query<TTable>(static_cast<const TTable*>(this), pFields, drv);
    }
};

}
}
