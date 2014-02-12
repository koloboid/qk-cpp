#include <qk/core/error.hpp>
#include "row.hpp"
#include "db.hpp"
#include "table.hpp"

using namespace Qk::Core;

namespace Qk {
namespace Db {

IRow::IRow()
    : mData(new Data())
{
}

IRow::IRow(const ITable* pTable, RowState::Value pState)
    : mData(new Data(pTable, pState))
{
    reset();
}

IRow::~IRow()
{
}

void IRow::reset()
{
    ASSERTPTR(mData->mTable)
    foreach (IField* fld, table()->fields())
    {
        this->setPrivate(fld, fld->defaultValue());
    }
}

QList<Error> IRow::validate() const
{
    ASSERTPTR(mData->mTable)
    QList<Error> rv;
    return rv;
}

void IRow::reload()
{

}

void IRow::load(const QSqlRecord& pRecord)
{
    ASSERTPTR(mData->mTable);
    for (int i = 0; i < pRecord.count(); i++)
    {
        IField* fld = table()->field(pRecord.fieldName(i));
        setPrivate(fld, pRecord.value(i));
    }
    mData->mState = RowState::Original;
}

IRow&IRow::set(const IField* pField, const QVariant& pValue)
{
    if (pField->readOnly()) throw new Error(ERRLOC, TR("Поле '%1' таблицы '%2' только для чтения, модификация возможна только на приватном уровне класса")
                                            .arg(pField->name()).arg(pField->table()->name()), TR("Идентификатор строки - '%1'")
                                            .arg(primaryValue().toString()));
    return setPrivate(pField, pValue);
}

IRow&IRow::setPrivate(const IField* pField, const QVariant& pValue)
{
    mData->mValues[pField] = pValue;
    if (mData->mState == RowState::Original) mData->mState = RowState::Modified;
    return *this;
}

QVariant IRow::primaryValue() const
{
    ASSERTPTR(mData->mTable);
    return table() ? get(*table()->primaryField()) : QVariant();
}

void IRow::serialize(Formatter &pFmt) const
{
    ASSERTPTR(mData->mTable);
    foreach (IField* fld, table()->fields())
    {
        pFmt.write(fld->name(), get(fld));
    }
}

QVariant IRow::lazyFetch(const IField *pField) const
{
    ASSERTPTR(mData->mTable);
    QVariant v = mData->mFK.value(pField);
    if (v.isValid()) return v;
    v = mData->mValues.value(pField);
    if (!v.isValid() || v.isNull() || v.type() != pField->linkedTo()->primaryField()->type()) return v;
    IRow r = pField->linkedTo()->select().where(*pField->linkedTo()->primaryField() == v).one();
    if (r.state() == RowState::Original)
    {
        v = pField->linkedTo()->rowToVariant(r);
        mData->mFK[pField] = v;
    }
    else
    {
        v = QVariant();
    }
    return v;
}

void IRow::save(Driver* pDrv)
{
    ASSERTPTR(mData->mTable)
    auto err = validate();
    if (err.size() > 0) throw err[0];
    if (!pDrv)
    {
        pDrv = table()->db()->drv();
    }
    switch (state())
    {
        case RowState::Invalid:
            dblog()->warn(TR("Попытка сохранить нулевую строку %1 таблицы '%2'").arg(primaryValue().toString()).arg(table()->name()));
            return;
        case RowState::Original:
            dblog()->warn(TR("Попытка сохранить немодифицированную строку %1 таблицы '%2'").arg(primaryValue().toString()).arg(table()->name()));
            return;
        case RowState::Modified:
            pDrv->updateRow(*this);
            mData->mState = RowState::Original;
            break;
        case RowState::New:
            QVariant id = pDrv->insertRow(*this);
            if (table()->primaryField()->flags() & FieldFlag::AutoIncrement)
            {
                setPrivate(table()->primaryField(), id);
            }
            mData->mState = RowState::Original;
            break;
    }
}

void IRow::drop(Driver* pDrv)
{
    ASSERTPTR(mData->mTable)
    if (!pDrv)
    {
        pDrv = table()->db()->drv();
    }
    pDrv->deleteRow(*this);
    mData->mState = RowState::Invalid;
}

}
}
