#pragma once

#include <QVariant>
#include <QExplicitlySharedDataPointer>
#include "db.export.hpp"

namespace Qk
{
namespace Db
{

class IField;
class ConditionData;

class QKDB_EXPORT Condition
{
public:
    enum EOperator
    {
        OpEqual,
        OpGreater,
        OpLess,
        OpGreaterOrEqual,
        OpLessOrEqual,
        OpLike,
        OpAnd,
        OpOr,
    };

    enum EType
    {
        TypeValueValue,
        TypeValueCondition,
        TypeValueField,
        TypeFieldValue,
        TypeFieldCondition,
        TypeFieldField,
        TypeConditionValue,
        TypeConditionCondition,
        TypeConditionField,
        TypeInvalid
    };

public:
    Condition();
    Condition(const Condition& pCondition);
    Condition(const IField* pFieldLeft, EOperator pOperator, QVariant pValueRight);
    Condition(const Condition& pCondLeft, EOperator pOperator, const Condition& pCondRight);
    ~Condition();

public:
    EType type() const;
    EOperator op() const;
    Condition conditionLeft() const;
    Condition conditionRight() const;
    QVariant valueLeft() const;
    QVariant valueRight() const;
    const IField* fieldLeft() const;
    const IField* fieldRight() const;
    QString toString() { return toString(*this); }

    Condition& operator=(const Condition& pCond);
    Condition operator&&(const Condition& pOther) const;
    Condition operator||(const Condition& pOther) const;

private:
    static QString toString(const Condition& pCond, QString pText = "");

    QExplicitlySharedDataPointer<ConditionData> mData;
};

}
}
