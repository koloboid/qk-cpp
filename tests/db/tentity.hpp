#pragma once

#include "tbase.hpp"

class TUser;

template<class TRowType = QkDbRow>
class TEntity : public TBase<TRowType>
{
public:
    TEntity(QkDb* pDb, QString pName, QString pTitle, QString pDescription, QkDbField* pJoinParentField)
        : TBase<TRowType>(pDb, pName, pTitle, pDescription, pJoinParentField),
          CreateDate(this, "CreateDate", "", ""),
          ModifyDate(this, "ModifyDate", "", ""),
          Creator(this, pDb, "Creator", "", ""),
          Modifier(this, pDb, "Modifier", "", "")
    {
    }

public:
    const QkDbFieldDateTime CreateDate;
    const QkDbFieldDateTime ModifyDate;
    const QkDbFieldLink<TUser> Creator;
    const QkDbFieldLink<TUser> Modifier;
};
