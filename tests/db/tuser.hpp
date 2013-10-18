#pragma once

#include "tentity.hpp"

class TUser;

class RUser : public QkDbRow
{
public:
    RUser(const QkDbTableBase* pParent)
        : QkDbRow(pParent)
    {
    }

    void foo();
};

class TUser : public TEntity<RUser>
{
public:
    static constexpr const char* tableName = "TUser";

public:
    TUser(QkDb* pDb)
        : TEntity(pDb, tableName, "", "", 0),
          Name(this, "Name", "", "")
    {
    }

public:
    const QkDbFieldString Name;
};

extern TUser* User;
