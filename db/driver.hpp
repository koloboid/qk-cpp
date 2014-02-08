#pragma once

#include "db.export.hpp"
#include <QThread>
#include <qk/core/formatter.hpp>

namespace Qk
{
namespace Db
{

class Db;
class ITable;
class IRow;
class IField;
class Index;
class Driver;
class IQuery;
class Condition;

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

    virtual QList<IRow> select(const IQuery& pQuery) = 0;
    virtual void select(const IQuery& pQuery, Qk::Core::Formatter* pFormatter) = 0;

    virtual QVariant insertRow(const IRow& pRow) = 0;
    virtual void updateRow(const IRow& pRow) = 0;
    virtual void deleteRow(const IRow& pRow) = 0;

    virtual void migrateDb(const Db& pSchema);

protected:
    virtual void migrateTable(const ITable& pTable) = 0;
    QString connectionName();
};

}
}
