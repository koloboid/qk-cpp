#include <QThreadStorage>
#include <QUuid>
#include <QStringList>
#include "db.hpp"
#include "table.hpp"
#include "drivermysql.hpp"

namespace Qk
{
namespace Db
{

Log* dblog(Log* pParent)
{
    static Log* l = new Log(pParent ? pParent : Qk::Core::log(), "DB");
    return l;
}

Db::Db()
{
}

Db::~Db()
{
    qDeleteAll(mTables);
}

void Db::init()
{
    foreach (ITable* tab, mTables)
    {
        tab->init();
    }
}

void Db::migrate()
{
    Driver* d = drv();
    d->migrateDb(*this);
}

void Db::addTable(ITable *pTable)
{
    ASSERTPTR(pTable);
    QString tableName = pTable->name();
    ASSERTSTR(tableName, 3, 32);
    ASSERTMAPUNIQKEY(mTables, tableName);
    ASSERTMAPUNIQVAL(mTables, pTable);

    dblog()->debug(TR("Добавление таблицы '%1'").arg(tableName));
    mTables[tableName] = pTable;
}

void Db::setupConnection(const QString& pConnectionString)
{
    mConnectionString = pConnectionString;
    mConnectionOptions.clear();
    foreach (QString opt, mConnectionString.split(';'))
    {
        opt = opt.trimmed();
        QStringList expr = opt.split('=');
        if (expr.size() == 2)
        {
            QString name = expr[0].trimmed();
            QString val = expr[1].trimmed();
            if (name == "driver")
            {
                mDriverName = val.toLower();
            }
            else if (name == "host")
            {
                mHostName = val;
            }
            else if (name == "dbname")
            {
                mDbName = val;
            }
            else if (name == "user")
            {
                mUserName = val;
            }
            else if (name == "password")
            {
                mPassword = val;
            }
            else if (name == "poolsize")
            {
                mPoolSize = val.toInt();
            }
            else if (name == "port")
            {
                mPort = val.toInt();
            }
            else mConnectionOptions[name] = val;
        }
        else
        {
            mConnectionOptions[opt] = "";
        }
    }
    if (mPoolSize > 100 || mPoolSize == 0) mPoolSize = 10;
}

thread_local Driver* mThreadDriver = 0;

Driver* Db::drv()
{
    if (!mThreadDriver)
    {
        if (mDriverName == "mysql")
        {
            mThreadDriver = new DriverMySql(mHostName, mPort, mDbName, mUserName, mPassword, mConnectionOptions);
            mThreadDriver->connect();
        }
        else
        {
            throw ErrorObjectNotFound(ERRLOC, TR("Драйвер '%1' не поддерживается системой. Поддерживаемые драйвера: mysql").arg(mDriverName));
        }
    }
    if (!mThreadDriver->checkConnection())
    {
        delete mThreadDriver;
        mThreadDriver = new DriverMySql(mHostName, mPort, mDbName, mUserName, mPassword, mConnectionOptions);
        mThreadDriver->connect();
    }
//    if (mThreadDriver->inTransaction()) mThreadDriver->rollback();
    return mThreadDriver;
}

}
}
