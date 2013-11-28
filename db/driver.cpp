#include <QUuid>
#include "driver.hpp"
#include "table.hpp"
#include "db.hpp"

namespace Qk
{
namespace Db
{

Driver::Driver()
{
}

Driver::~Driver()
{
}

QString Driver::connectionName()
{
    return QString("%1").arg((intptr_t)this);
}

void Driver::migrateDb(const Db* pSchema)
{
    foreach (const ITable* tab, pSchema->tables())
    {
        migrateTable(tab);
    }
}

}
}
