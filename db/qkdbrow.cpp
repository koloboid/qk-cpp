#include "qkdbrow.hpp"
#include "qkdbtable.hpp"
#include "qkdb.hpp"

QMetaType::Type QkDbRow::metaTypeID()
{
    static QMetaType::Type t = (QMetaType::Type)qRegisterMetaType<QkDbRow>("QkDbRow");
    return t;
}

QkDbRow::QkDbRow()
    : mData(nullptr)
{
}

QkDbRow::QkDbRow(const QkDbTableBase *pTable)
    : mData(new RowData(pTable, QkDbRowState::New))
{
}

QkDbRow::~QkDbRow()
{
}

QkFuture<bool> QkDbRow::save(const QkFuture<bool>::StdCallback& pCallback)
{
    // pre-save stuff
    if (mData->mLocked)
    {
        if (pCallback) pCallback(QkError(ERRLOC, "Row is locked"), false);
        return QkFuture<bool>(false);
    }
    auto errs = check();
    if (errs.count() > 0)
    {
        if (pCallback) pCallback(errs[0], false);
        return QkFuture<bool>(false);
    }
    mData->mLocked = true;
    return table()->db()->exec([this, pCallback](QkDbDriver* pDrv) {
        if (state().isNew())
        {
            pDrv->insertRow(*this);
        }
        else
        {
            pDrv->updateRow(*this);
        }
        if (pCallback) pCallback(QkError(), true);
    });
}
