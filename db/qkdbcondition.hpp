#pragma once

#include <QVariant>
#include <QExplicitlySharedDataPointer>
#include "qkdb.export.hpp"

class QkDbField;
class QkDbConditionData;

class QKDB_EXPORT QkDbCondition
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
    QkDbCondition();
    QkDbCondition(const QkDbCondition& pCondition);
    QkDbCondition(const QkDbField& pFieldLeft, EOperator pOperator, QVariant pValueRight);
    QkDbCondition(const QkDbCondition& pCondLeft, EOperator pOperator, const QkDbCondition& pCondRight);
    ~QkDbCondition();

public:
    EType type() const;
    EOperator op() const;
    QkDbCondition conditionLeft() const;
    QkDbCondition conditionRight() const;
    QVariant valueLeft() const;
    QVariant valueRight() const;
    const QkDbField* fieldLeft() const;
    const QkDbField* fieldRight() const;
    QString toString() { return toString(*this); }

private:
    static QString toString(const QkDbCondition& pCond, QString pText = "");

    QExplicitlySharedDataPointer<QkDbConditionData> mData;
};
