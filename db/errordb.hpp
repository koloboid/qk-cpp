#pragma once

#include <QSqlError>
#include <QSqlQuery>
#include <qk/core/error.hpp>

using namespace Qk::Core;

namespace Qk
{
namespace Db
{

class ErrorDb : public Error
{
public:
    ErrorDb(const QString& pLocation, const QString& pMessage)
        : Error(500, pLocation, pMessage)
    {
    }

    ErrorDb(const QString& pLocation, const QString& pMessage, const QSqlError& pErr)
        : Error(500, pLocation, pMessage, pErr.text())
    {
    }

    ErrorDb(const QString& pLocation, const QSqlQuery& pFailedQuery)
        : Error(500, pLocation, TR("Ошибка выполнения запроса '%1'").arg(pFailedQuery.lastQuery()), pFailedQuery.lastError().text())
    {
    }
};

}
}
