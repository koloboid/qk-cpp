#include "qkdbdrivermysql.hpp"

QkDbDriverMySql::QkDbDriverMySql(const QString& pHost, quint16 pPort, const QString& pDbName, const QString& pUserName,
                                 const QString& pPassword, const QMap<QString, QString>& pParams)
{
}

QkDbDriverMySql::~QkDbDriverMySql()
{
    mDb.close();
}

bool QkDbDriverMySql::connect()
{
}

bool QkDbDriverMySql::disconnect()
{
    mDb.close();
}
