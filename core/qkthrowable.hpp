#pragma once

#include "qkcore.export.hpp"
#include "qkerror.hpp"
#include <QVariant>

class QKCORE_EXPORT QkThrowable
{
public:
#ifdef QK_EXCEPTION
    QkThrowable(bool pThrowException = true)
#else
    QkThrowable(bool pThrowException = false)
#endif
        : mError(), mThrowException(pThrowException)
    {
    }

    virtual ~QkThrowable()
    {
    }

public:
    const QkError& lastError() const { return mError; }

protected:
    bool throwNow(const QString& pErrLoc, const QString& pMessage) const
    {
        return throwNow(QkError(pErrLoc, pMessage));
    }

    bool throwNow(const QkError& pError) const
    {
        mError = pError;
#ifdef QK_EXCEPTION
        if (pError.hasError() && mThrowException) throw mError;
#endif
        return false;
    }

    bool throwNotImplemented(const QString& pErrLoc)
    {
        return throwNow(QkErrorNotImplemented(pErrLoc));
    }

    bool assertPtr(const QString& pErrLoc, const void* pPtr, const QString& pVarName) const
    {
        if (pPtr == nullptr)
            return throwNow(pErrLoc, tr("Переменная '%1' не может быть NULL").arg(pVarName));
        return true;
    }

protected:
    template<class TKey, class TVal>
    bool assertMapKey(const QString& pErrLoc, QMap<TKey, TVal> pMap, TKey pKey) const
    {
        if (pMap.contains(pKey))
            return throwNow(pErrLoc, tr("Словарь уже имеет значение с ключом '%1'").arg(pKey));
        return true;
    }

    template<class TKey, class TVal>
    bool assertMapValue(const QString& pErrLoc, QMap<TKey, TVal> pMap, TVal pValue) const
    {
        if (pMap.values().contains(pValue))
            return throwNow(pErrLoc, tr("Словарь уже имеет такое значение"));
        return true;
    }

    bool assertStr(const QString& pErrLoc, const QVariant& pVal, int pMinSize, int pMaxSize, const QString& pVarName) const
    {
        if (!pVal.canConvert<QString>())
            return throwNow(pErrLoc, tr("Значение '%1' переменной '%2' невозможно преобразовать в строку")
                            .arg(pVal.toString())
                            .arg(pVarName));
        return assertStr(pErrLoc, pVal.toString(), pMinSize, pMaxSize, pVarName);
    }

    bool assertStr(const QString& pErrLoc, const QString& pStr, int pMinSize, int pMaxSize, const QString& pVarName) const
    {
        if (pStr.size() < pMinSize)
            return throwNow(pErrLoc, tr("Переменная '%1' ('%2') должна иметь более %3 символов в длину")
                            .arg(pVarName)
                            .arg(pStr)
                            .arg(pMinSize));
        if (pStr.size() > pMaxSize)
            return throwNow(pErrLoc, tr("Переменная '%1' ('%2') должна иметь иметь менее %3 символов в длину")
                            .arg(pVarName)
                            .arg(pStr)
                            .arg(pMaxSize));
        return true;
    }

    template<class TVal>
    bool assertVal(const QString& pErrLoc, const QVariant& pVal, TVal pMinValue, TVal pMaxValue, const QString& pVarName) const
    {
        if (!pVal.canConvert<TVal>())
            return throwNow(pErrLoc, tr("Значение '%1' переменной '%2' невозможно преобразовать")
                            .arg(pVal.toString())
                            .arg(pVarName));
        TVal val = pVal.value<TVal>();
        if (val < pMinValue)
            return throwNow(pErrLoc, tr("Переменная '%1' ('%2') должна быть больше %3")
                            .arg(pVarName)
                            .arg(pVal.toString())
                            .arg(QVariant::fromValue(pMinValue).toString()));
        if (val > pMaxValue)
            return throwNow(pErrLoc, tr("Переменная '%1' ('%2') должна быть меньше %3")
                            .arg(pVarName)
                            .arg(pVal.toString())
                            .arg(QVariant::fromValue(pMinValue).toString()));
        return true;
    }

    QString tr(const char* pText) const
    {
        return QCoreApplication::translate("common", pText);
    }

    void resetError()
    {
        mError = QkError();
    }

private:
    mutable QkError mError;
    bool mThrowException;
};

#define ASSERTPTR(pPtrVarName) if (!assertPtr(ERRLOC, pPtrVarName, #pPtrVarName)) return false;
