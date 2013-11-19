#pragma once

#include "db.export.hpp"
#include <QThread>

namespace Qk
{
namespace Db
{

class Db;
class ITable;
class IField;
class Index;
class IModel;
class Driver;
class IQuery;
class Condition;
typedef QSharedPointer<IModel> RModel;

class QKDB_EXPORT Driver
{
public:
    Driver();
    virtual ~Driver();

public:
    virtual void connect() = 0;
    virtual void disconnect() = 0;
    virtual bool checkConnection() = 0;

    virtual void transaction() = 0;
    virtual void commit() = 0;
    virtual void rollback() = 0;
    virtual bool inTransaction() const = 0;

    virtual QList<RModel> select(const IQuery* pQuery) = 0;

    virtual QVariant insertRow(const IModel* pRow) = 0;
    virtual void updateRow(const IModel* pRow) = 0;
    virtual void deleteRow(const IModel* pRow) = 0;

    virtual void migrateDb(const Db* pSchema);

protected:
    virtual void migrateTable(const ITable* pTable) = 0;
    QString connectionName();
};

}
}
