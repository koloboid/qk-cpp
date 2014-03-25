#include <QUuid>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QDateTime>
#include <QDebug>
#include "drivermysql.hpp"
#include "errordb.hpp"
#include "table.hpp"
#include "field.hpp"
#include "query.hpp"
#include "table.hpp"
#include "db.hpp"

namespace Qk
{
namespace Db
{

DriverMySql::DriverMySql(const QString& pHost, quint16 pPort, const QString& pDbName, const QString& pUserName, const QString& pPassword, const QMap<QString, QString>& pParams)
    : mDb(QSqlDatabase::addDatabase("QMYSQL", connectionName()))
{
    ASSERTSTR(pHost, 1, 255);
    ASSERTSTR(pDbName, 1, 255);
    ASSERTSTR(pUserName, 1, 255);

    mDb.setHostName(pHost);
    mDb.setPort(pPort);
    mDb.setDatabaseName(pDbName);
    mDb.setUserName(pUserName);
    mDb.setPassword(pPassword);
    QString opts;
    foreach (QString key, pParams)
    {
        opts += key + "=" + pParams[key] + ";";
    }

    mDb.setConnectOptions(opts);
}

DriverMySql::~DriverMySql()
{
    disconnect();
    QSqlDatabase::removeDatabase(mDb.connectionName());
}

void DriverMySql::connect()
{
    disconnect();
    if (!mDb.open())
    {
        throw ErrorDb(ERRLOC, TR("Не удалось подключиться к БД"), mDb.lastError());
    }
}

void DriverMySql::disconnect()
{
    mPrepared.clear();
    if (mCurrentQuery.isActive()) mCurrentQuery.clear();
    if (mDb.isOpen())
    {
        mDb.close();
    }
}

bool DriverMySql::checkConnection()
{
    QSqlQuery q("select 1", mDb);
    return q.next();
}

void DriverMySql::query(const IQuery& pQuery, int* pTotalCount, const std::function<void(const QSqlRecord& pRecord)>& pFunc)
{
    QString cols = "";
    QString order = "";
    QString limit = "";
    if (pQuery.limitOffset() != 0 && pQuery.limitCount() != 0)
    {
        limit = QString("LIMIT %1,%2").arg(pQuery.limitOffset()).arg(pQuery.limitCount());
    }
    else if (pQuery.limitCount() > 0)
    {
        limit = QString("LIMIT %1").arg(pQuery.limitCount());
    }

    foreach (IField* fld, pQuery.fields())
    {
        cols.append(fieldName(fld, true) + ",");
    }
    cols = cols.left(cols.size() - 1);
    QString joins = "";//JoinsToSql(pQuery, cols);
    QString sql = QString("select %1 from `%2` %3 %4 %5 %6")
        .arg(cols)
        .arg(pQuery.table()->name())
        .arg(joins)
        .arg(conditionToSql(pQuery.condition()))
        .arg(order)
        .arg(limit);

    doQuery(sql);
    if (pTotalCount) *pTotalCount = mCurrentQuery.size();
    if (pFunc)
    {
        while (mCurrentQuery.next())
        {
            pFunc(mCurrentQuery.record());
        }
    }
}

QString DriverMySql::conditionToSql(const Condition& pCond)
{
    if (pCond.type() == Condition::TypeInvalid) return "";
    return conditionToSql(pCond, "WHERE ");
}

QString DriverMySql::conditionToSql(const Condition& pCond, QString pText)
{
    if (pCond.type() == Condition::TypeInvalid) return pText;
    switch (pCond.type())
    {
        case Condition::TypeConditionCondition:
            pText += "(" + conditionToSql(pCond.conditionLeft(), "") + operatorToSql(pCond.op()) + conditionToSql(pCond.conditionRight(), "") + ")";
            break;
        case Condition::TypeFieldValue:
            pText += "(" + fieldName(pCond.fieldLeft()) + operatorToSql(pCond.op()) + sqlValue(pCond.valueRight(), pCond.fieldLeft()) + ")";
            break;
        default:
            pText += QString(" INVALID:%1 ").arg(pCond.type());
            break;
    }
    return pText;
}

QString DriverMySql::sqlValue(QVariant pValue, const IField* pField)
{
    QString val = "";
    if (pField->linkedTo())
    {
        IRow mod = pField->linkedTo()->rowFromVariant(pValue);
        if (!mod) return "NULL";
        return sqlValue(mod.primaryValue(), mod.table()->primaryField());
    }
    else
    {
        switch (pValue.type())
        {
            case QVariant::Invalid:
                val = "NULL";
                break;
            case QVariant::String:
                val = "'" + pValue.toString() + "'";
                break;
            case QVariant::DateTime:
            {
                QDateTime dt = pValue.toDateTime();
                if (dt.timeSpec() == Qt::LocalTime) dt = dt.toUTC();
                val = "'" + dt.toString("yyyy-MM-dd HH:mm:ss") + "'";
                break;
            }
            default:
                val = pValue.toString();
                break;
        }
    }
    return val;
}

QString DriverMySql::operatorToSql(Condition::EOperator pOperator)
{
    switch (pOperator)
    {
        case Condition::OpAnd:
            return " AND ";
        case Condition::OpOr:
            return " OR ";
        case Condition::OpEqual:
            return " = ";
        case Condition::OpGreater:
            return " > ";
        case Condition::OpGreaterOrEqual:
            return " >= ";
        case Condition::OpLess:
            return " < ";
        case Condition::OpLessOrEqual:
            return " <= ";
        case Condition::OpLike:
            return  " % ";
        case Condition::OpEqualCaseInsensitive:
            return  " % ";
        default:
            return " UNKNOWN OPERATOR ";
    }
}

void DriverMySql::deleteRow(const IRow&)
{
    throw ErrorNotImplemented(ERRLOC);
}

QVariant DriverMySql::insertRow(const IRow& pRow)
{
    QString sql = "insert into `%1` (%2) values (%3)";
    QString cols = "";
    QString vals = "";
    foreach (IField* fld, pRow.table()->fields())
    {
        if (!(fld->flags() & EFieldFlag::AutoIncrement))
        {
            cols += fieldName(fld) + ",";
            vals += sqlValue(pRow.get(fld), fld) + ",";
        }
    }
    cols = cols.left(cols.size() - 1);
    vals = vals.left(vals.size() - 1);
    sql = sql.arg(pRow.table()->name()).arg(cols).arg(vals);
    execSql(sql);
    if (pRow.table()->primaryField()->flags() & EFieldFlag::AutoIncrement)
    {
        QSqlQuery q(mDb);
        if (!q.prepare("select LAST_INSERT_ID()"))
        {
            throw ErrorDb(ERRLOC, q);
        }
        if (!q.exec())
        {
            throw ErrorDb(ERRLOC, q);
        }
        if (q.size() <= 0)
        {
            throw ErrorDb(ERRLOC, TR("Невозможно определить идентификатор новой строки"));
        }
        q.next();
        return q.value(0);
    }
    else return QVariant();
}

void DriverMySql::updateRow(const IRow& pRow)
{
    QString sql = "update `%1` set %2 where %3";
    QString names = "";
    foreach (const IField* fld, pRow.changedFields())
    {
        names += fieldName(fld) + "=?,";
    }
    QString where = fieldName(pRow.table()->primaryField()) + "=?";
    sql = sql.arg(pRow.table()->name()).arg(names.left(names.size() - 1)).arg(where);
    QSqlQuery q = getPreparedQuery(sql);
    int i = 0;
    foreach (const IField* fld, pRow.changedFields())
    {
        q.bindValue(i++, pRow.get(fld));
    }
    q.bindValue(i++, pRow.primaryValue());
    execSqlQuery(q);
}

void DriverMySql::migrateField(const IField* pField, QSqlRecord pDbRecord)
{
    ASSERTPTR(pField);

    QString dbtype = pDbRecord.value("Type").toString();
    if (pDbRecord.value("Collation").toString().toLower().endsWith("_bin")) dbtype += " binary";
    if (pDbRecord.value("Null").toString().toLower() == "yes") dbtype += " null";
    else dbtype += " not null";
    QString extra = pDbRecord.value("Extra").toString();
    if (!extra.isEmpty())
    {
        dbtype += " " + extra;
    }
    if (pDbRecord.value("Key").toString().toLower() == "pri") dbtype += " primary key";
    dbtype = dbtype.toLower();
    QString mytype = fieldDefinition(pField).replace("`" + pField->name() + "` ", "").toLower();
    if (pDbRecord.isEmpty())
    {
        QString code = QString("alter table `%1` add column %2").arg(pField->table()->name()).arg(fieldDefinition(pField));
        execSql(code);
    }
    else if (dbtype != mytype)
    {
        dblog()->debug("Alter field %3, my field: '%1', db field: '%2'").arg(mytype).arg(dbtype).arg(pField->name());
        QString code = QString("alter table `%1` change column %2 %3")
            .arg(pField->table()->name())
            .arg(fieldName(pField))
            .arg(fieldDefinition(pField));
        execSql(code);
    }
    if (pField->flags() & FieldFlag::CascadeDelete)
    {
        ASSERTPTR(pField->linkedTo());
        QString constrname = pField->name() + pField->linkedTo()->name() + pField->linkedTo()->primaryField()->name();
        doQuery(QString("select CONSTRAINT_NAME FROM INFORMATION_SCHEMA.REFERENTIAL_CONSTRAINTS WHERE CONSTRAINT_NAME = '%1'")
            .arg(constrname));
        if (!hasRecord())
        {
            QString sql = "ALTER TABLE `%1` ADD CONSTRAINT `%2` FOREIGN KEY (%3) "
                            "REFERENCES `%4` (`%5`) ON DELETE CASCADE ON UPDATE NO ACTION, "
                            "ADD INDEX `%2` (`%3` ASC)";
            sql = sql.arg(pField->table()->name())
                .arg(constrname)
                .arg(fieldName(pField))
                .arg(pField->linkedTo()->name())
                .arg(fieldName(pField->linkedTo()->primaryField()));
            execSql(sql);
        }
    }
}

bool DriverMySql::hasRecord()
{
    return mCurrentQuery.next();
}

QSqlQuery DriverMySql::getPreparedQuery(const QString& pSql)
{
    QSqlQuery q = mPrepared.value(pSql, QSqlQuery(mDb));
    if (q.isValid()) return q;
    if (!q.prepare(pSql))
    {
        throw ErrorDb(ERRLOC, q);
    }
    mPrepared[pSql] = q;
    return q;
}

QMap<QString, QSqlRecord> DriverMySql::getDbFields(const QString& pTableName)
{
    QMap<QString, QSqlRecord> rv;
    doQuery("show full columns from `" + pTableName + "`");
    while (mCurrentQuery.next())
    {
        QSqlRecord rec = mCurrentQuery.record();
        rv[rec.value("Field").toString()] = rec;
    }
    return rv;
}

void DriverMySql::doQuery(const QString& pQuery)
{
    QSqlQuery q(mDb);
    if (!q.prepare(pQuery))
    {
        throw ErrorDb(ERRLOC, q);
    }
    doQuery(q);
}

void DriverMySql::doQuery(QSqlQuery pQuery)
{
    if (!pQuery.exec())
    {
        throw ErrorDb(ERRLOC, pQuery);
    }
    dblog()->debug(QString("SQL на %1: %2: %3").arg(mDb.connectionName()).arg(pQuery.lastQuery()).arg(pQuery.size()));
    mCurrentQuery = pQuery;
}

void DriverMySql::migrateIndex(const Index* pIndex)
{
    ASSERTPTR(pIndex);

    throw ErrorNotImplemented(ERRLOC);
}

void DriverMySql::migrateTable(const ITable& pTable)
{
    dblog()->debug(TR("Миграция таблицы '%1'").arg(pTable.name()));
    if (mDb.record(pTable.name()).isEmpty())
    {
        IField* id = pTable.primaryField();
        execSql(QString("create table `%1` (%2) ENGINE=%3")
            .arg(pTable.name())
            .arg(fieldDefinition(id))
            .arg(pTable.engine().isEmpty() ? QString("InnoDB") : pTable.engine()));
    }
    QMap<QString, QSqlRecord> dbfields = getDbFields(pTable.name());
    foreach (IField* fld, pTable.fields().values())
    {
        migrateField(fld, dbfields.value(fld->name()));
    }
}

void DriverMySql::commit()
{
    if (!mInTransaction)
    {
        throw ErrorDb(ERRLOC, TR("Сбой завершения транзакции. Драйвер не находится в состоянии транзакции"));
    }
    if (!mDb.commit())
    {
        throw ErrorDb(ERRLOC, TR("Сбой завершения транзакции"), mDb.lastError());
    }
    dblog()->debug(TR("SQL на %1: COMMIT").arg(mDb.connectionName()));
    mInTransaction = false;
}

void DriverMySql::rollback()
{
    if (!mInTransaction)
    {
        dblog()->warn(TR("Сбой завершения транзакции. Драйвер не находится в состоянии транзакции"));
    }
    if (!mDb.rollback())
    {
        throw ErrorDb(ERRLOC, TR("Сбой отката транзакции"), mDb.lastError());
    }
    dblog()->debug(TR("SQL на %1: ROLLBACK").arg(mDb.connectionName()));
    mInTransaction = false;
}

void DriverMySql::transaction()
{
    if (mInTransaction)
    {
        dblog()->warn(TR("Сбой создания транзакции. Драйвер уже находится в состоянии транзакции. Будет выполнен автоматический откат"));
        rollback();
    }
    if (!mDb.transaction())
    {
        throw ErrorDb(ERRLOC, TR("Не удалось запустить выполнение транзакции"), mDb.lastError());
    }
    dblog()->debug(TR("SQL на %1: TRANSACT").arg(mDb.connectionName()));
    mInTransaction = true;
}

qint64 DriverMySql::execSql(const QString& pSql)
{
    QSqlQuery q(mDb);
    if (!q.prepare(pSql))
    {
        throw ErrorDb(ERRLOC, q);
    }
    return execSqlQuery(q);
}

qint64 DriverMySql::execSqlQuery(QSqlQuery& pQuery)
{
    dblog()->debug(TR("SQL на %1: %2").arg(mDb.connectionName()).arg(pQuery.lastQuery()));
    if (!pQuery.exec())
    {
        throw ErrorDb(ERRLOC, pQuery);
    }
    return pQuery.numRowsAffected();
}

QString DriverMySql::fieldDefinition(const IField* pField)
{
    ASSERTPTR(pField);
    QString def = fieldName(pField) + " " + typeName(pField);
    if (pField->flags() & EFieldFlag::AllowNull) def += " null";
    else def += " not null";
    if (pField->flags() & EFieldFlag::AutoIncrement) def += " AUTO_INCREMENT PRIMARY KEY";
    return def;
}

QString DriverMySql::fieldName(const IField* pField, bool pWithTableName)
{
    ASSERTPTR(pField);
    if (pWithTableName)
    {
        return QString("`%1`.`%2`").arg(pField->table()->name()).arg(pField->name());
    }
    else
    {
        return escape(pField->name());
    }
}

QString DriverMySql::typeName(const IField* pField)
{
    ASSERTPTR(pField);
    QMetaType::Type t = pField->underylingType();
    switch (t)
    {
        case QMetaType::QDateTime:
            return "datetime";
        case QMetaType::Short:
            return "smallint(5)";
        case QMetaType::Int:
            return "int(11)";
        case QMetaType::LongLong:
            return "bigint(20)";
        case QMetaType::SChar:
            return "tinyint(3)";
        case QMetaType::QString:
            if (pField->max() > 32767)
            {
                return "text";
            }
            else
            {
                return "varchar(" + pField->max().toString() + (pField->flags() & EFieldFlag::CaseInsensitive ? ")" : ") BINARY");
            }
        case QMetaType::UShort:
            return "smallint(5) unsigned";
        case QMetaType::UInt:
            return "int(11) unsigned";
        case QMetaType::ULongLong:
            return "bigint(20) unsigned";
        case QMetaType::UChar:
            return "tinyint(3) unsigned";
        case QMetaType::Float:
            return "single";
        case QMetaType::Double:
            return "double";
        case QMetaType::Bool:
            return "bool";
        default:
            throw ErrorDb(ERRLOC, TR("Тип '%1' не поддерживается базой данных").arg(QMetaType::typeName(t)));
    }
}

QString DriverMySql::escape(const QString& pName)
{
    return "`" + pName + "`";
}

}
}
