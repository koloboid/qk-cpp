#pragma once

#include "qkcore.export.hpp"
#include "qkconsoledevice.hpp"
#include "qkthrowable.hpp"
#include <QObject>
#include <QAbstractItemModel>
#include <QtWidgets/QDialog>
#include <QSettings>

class QKCORE_EXPORT QkConfig : public QObject, public QkThrowable
{
    Q_OBJECT

public:
    QkConfig(QObject* pParent = 0);
    virtual ~QkConfig();

public:
    virtual bool init(int pArgc, char** pArgv);
    virtual bool handleHelp(QIODevice* pOutput = QkConsoleDevice::stdOut());
    QAbstractItemModel* model();
    virtual void sync();

protected:
    QMap<QString, QString> mDescriptions;
    QMap<QString, QString> mShortCut;
    QSettings mSettings;
};

#define QK_CONFIGPROP(pType, pName, pDefault, pShortCut, pDescr) Q_PROPERTY(pType pName READ pName WRITE pName RESET pName##Reset) \
    private: pType m_##pName; \
    public: pType pName() const { return m_##pName; } \
    public: void pName(const pType& pValue) { m_##pName = pValue; } \
    public: void pName##Reset() { mShortCut[#pName] = pShortCut; \
        mDescriptions[#pName] = QString(pDescr).isEmpty() ? #pName : pDescr; m_##pName = pDefault; }
