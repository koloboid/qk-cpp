#pragma once

#include "tentity.hpp"

class TUserGroup : public TEntity<QkDbRow>
{
public:
    TUserGroup(QkDb* pDb)
        : TEntity(pDb, "TUserGroup", "", "", 0),
          Name(this, "Name", "", "")
    {
    }

public:
    const QkDbFieldString Name;
};
