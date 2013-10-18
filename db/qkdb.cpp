#include "qkdb.hpp"
#include "qkdbtable.hpp"
#include "qkdbdrivermysql.hpp"
#include <QThreadStorage>
#include <QStringList>

QkDb::QkDb()
{
}

QkDb::~QkDb()
{
    qDeleteAll(mTables);
}

bool QkDb::init()
{
    bool rv = true;
    foreach (QkDbTableBase* tab, mTables)
    {
        rv &= tab->init();
    }
    return rv;
}

QkFuture<bool> QkDb::migrate(const QkFuture<bool>::StdCallback& pCallback)
{
    bool rv = true;
    QkFutureBase f;
    foreach (QkDbTableBase* tab, mTables)
    {
//        f += tab->migrate();
    }
    if (pCallback)
    {
        pCallback(QkError(), true);
    }
//    return QkFuture<bool>(f);
//    return rv;
}

QkDbTableBase* QkDb::addTable(QkDbTableBase *pTable)
{
    if (!assertPtr(ERRLOC, pTable, "pTable")) return nullptr;
    QString name = pTable->schema()->name();
    if (!assertStr(ERRLOC, name, 3, 32, "name")) return nullptr;
    if (!assertMapKey(ERRLOC, mTables, name)) return nullptr;
    if (!assertMapValue(ERRLOC, mTables, pTable)) return nullptr;

    mTables[name] = pTable;
    return pTable;
}

bool QkDb::setupConnection(const QString& pConnectionString)
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
                mDriverName = val;
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
    return true;
}

QkFuture<bool> QkDb::exec(std::function<void(QkDbDriver*)> pCode)
{
    // if currentthread is db thread, pCode(drv)
    // get free driver and their thread
    // if no free driver - queue request
    // send message to thread
}
