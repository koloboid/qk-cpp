#include "qkdbtable.hpp"

QkDbTableBase::QkDbTableBase(QkDb *pDb, const QString &pName, const QString &pTitle, const QString &pDescr, QkDbField *pJoinParentField)
    : mDb(pDb), mSchema(pName, pTitle, pDescr, pJoinParentField)
{
}

QkDbTableBase::~QkDbTableBase()
{
}

bool QkDbTableBase::init()
{
    return true;
}

bool QkDbTableBase::migrate()
{
    return this->throwNotImplemented(ERRLOC);
}
