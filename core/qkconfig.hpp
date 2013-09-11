#pragma once

#include "qkcore.export.hpp"
#include "qkconsoledevice.hpp"
#include <QObject>
#include <QAbstractItemModel>
#include <QtWidgets/QDialog>
#include <QSettings>

class QKCORE_EXPORT QkConfig : public QObject
{
    Q_OBJECT

public:
    QkConfig();
    virtual ~QkConfig();

public:
    virtual bool init();
    virtual bool handleHelp(QIODevice* pOutput = QkConsoleDevice::stdOut());
    QAbstractItemModel* model();
    virtual void sync();

protected:
    QMap<QString, QString> mTitles;
    QMap<QString, QString> mDescriptions;
    QSettings mSettings;
};

#define QK_CONFIGPROP(pType, pName, pDefault, pTitle, pDescr) Q_PROPERTY(pType pName READ pName WRITE pName RESET pName##Reset) \
    private: pType m_##pName; \
    public: pType pName() const { return m_##pName; } \
    public: void pName(const pType& pValue) { m_##pName = pValue; } \
    public: void pName##Reset() { QString title = QString(pTitle).isEmpty() ? "pName" : pTitle; mTitles["pName"] = title; \
        mDescriptions["pName"] = QString(pDescr).isEmpty() ? title : pDescr; m_##pName = pDefault; }
