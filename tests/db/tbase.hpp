#pragma once

#include <qkdb.hpp>
#include <memory>

template<class TRowType = QkDbRow>
class TBase : public QkDbTable<TRowType>
{
public:
    TBase(QkDb* pDb, QString pName, QString pTitle, QString pDescription, QkDbField* pJoinParentField)
        : QkDbTable<TRowType>(pDb, pName, pTitle, pDescription, pJoinParentField)
    {
    }

public:
    const QkDbField ID;
};

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

class TUser : public TBase<RUser>
{
public:
    TUser()
        : TBase(0, "", "", "", 0)
    {
    }

public:
    const QkDbField Name;
};
