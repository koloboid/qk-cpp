#include <qk/core/error.hpp>
#include "model.hpp"
#include "db.hpp"

using namespace Qk::Core;

namespace Qk {
namespace Db {

IModel::IModel(ITable *pTable)
    : mTable(pTable)
{
}

QList<Error> IModel::validate() const
{
    QList<Error> rv;
    return rv;
}

void IModel::load(const QSqlRecord& pRecord)
{
    for (int i = 0; i < pRecord.count(); i++)
    {
        IField* fld = table()->field(pRecord.fieldName(i));
        fld->set(this, pRecord.value(i));
    }
    mIsLoaded = true;
}

void IModel::save(Driver* pDrv)
{
    auto err = validate();
    if (err.size() > 0) throw err[0];
    if (!pDrv)
    {
        pDrv = table()->db()->drv();
    }
    if (mIsLoaded)
    {
        pDrv->updateRow(this);
    }
    else
    {
        QVariant id = pDrv->insertRow(this);
        if (table()->primaryField()->flags().isAutoIncrement())
        {
            table()->primaryField()->set(this, id);
        }
    }
    mIsLoaded = true;
}

}
}
