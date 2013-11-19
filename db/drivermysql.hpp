#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include "driver.hpp"
#include "condition.hpp"

namespace Qk
{
namespace Db
{

class QKDB_EXPORT DriverMySql : public Driver
{
public:
    DriverMySql(const QString& pHost, quint16 pPort, const QString& pDbName, const QString& pUserName,
                    const QString& pPassword, const QMap<QString, QString>& pParams);
    virtual ~DriverMySql();

public:
    virtual void connect() override;
    virtual void disconnect() override;
    virtual bool checkConnection() override;

    virtual void transaction() override;
    virtual void commit() override;
    virtual void rollback() override;
    virtual bool inTransaction() const { return mInTransaction; }

    virtual QList<RModel> select(const IQuery* pQuery);

    virtual QVariant insertRow(const IModel* pRow) override;
    virtual void updateRow(const IModel* pRow) override;
    virtual void deleteRow(const IModel* pRow) override;

protected:
    virtual void migrateTable(const ITable* pTable) override;
    virtual void migrateField(const IField* pField, QSqlRecord pDbRecord);
    virtual void migrateIndex(const Index* pIndex);
    qint64 execSql(const QString& pSql);
    virtual void doQuery(const QString& pSql);
    virtual void doQuery(QSqlQuery pQuery);
    bool hasRecord();
    QList<RModel> readQueryResult(const IQuery* pQuery);

    static QString fieldDefinition(const IField* pField);
    static QString typeName(const IField* pField);
    static QString fieldName(const IField* pField, bool pWithTableName = false);
    static QString escape(const QString& pName);
    static QString conditionToSql(const Condition& pCond);
    static QString conditionToSql(const Condition& pCond, QString pText);
    static QString operatorToSql(Condition::EOperator pOperator);
    static QString sqlValue(QVariant pValue, const IField* pField);

private:
    QSqlDatabase mDb;
    QSqlQuery mCurrentQuery;
    bool mInTransaction = false;
};

}
}
