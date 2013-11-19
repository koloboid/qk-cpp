#pragma once

#include <QMap>
#include <qk/core/log.hpp>
#include "db.export.hpp"
#include "driver.hpp"

using namespace Qk::Core;

namespace Qk
{
namespace Db
{

class QKDB_EXPORT Db
{
    Q_DISABLE_COPY(Db)

public:
    Db();
    virtual ~Db();

public:
    template<class TTable>
    void registerTable() { addTable(&TTable::TableSchema); }
    ITable* getTable(const QString& pName) const { return mTables[pName]; }
    QMap<QString, ITable*> tables() const { return mTables; }

    virtual Driver* drv();
    virtual void init();
    virtual void setupConnection(const QString& pConnectionString);
    virtual void migrate();

private:
    void addTable(ITable* pTable);

private:
    QMap<QString, ITable*> mTables;
    QString mConnectionString;
    QMap<QString, QString> mConnectionOptions;
    QString mDriverName;
    QString mHostName;
    QString mDbName;
    QString mUserName;
    QString mPassword;
    quint32 mPoolSize = 0;
    quint16 mPort = 0;
};

Log* dblog(Log* pParent = 0);

}
}
