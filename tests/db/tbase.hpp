#pragma once

#include <qkdbtable.hpp>

template<class TRowType = QkDbRow>
class TBase : public QkDbTable<TRowType>
{
public:
    TBase(QkDb* pDb, QString pName, QString pTitle, QString pDescription, QkDbField* pJoinParentField)
        : QkDbTable<TRowType>(pDb, pName, pTitle, pDescription, pJoinParentField),
          ID(this, "ID", "", "")
    {
    }

public:
    const QkDbFieldUInt64 ID;
};
