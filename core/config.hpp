#pragma once

#include "core.export.hpp"
#include "consoledevice.hpp"
#include "log.hpp"
#include <QObject>
#include <QAbstractItemModel>
#include <QStringList>
#include <QString>

#define QK_CONFIGPROP(pType, pName, pDefault, pShortCut, pDescr) Q_PROPERTY(pType pName READ pName WRITE pName RESET pName##Reset STORED true) \
    private: pType m_##pName; \
    public: pType pName() const { return m_##pName; } \
    public: void pName(const pType& pValue) { m_##pName = pValue; } \
    public: void pName##Reset() { mShortCut[#pName] = pShortCut; \
        mDescriptions[#pName] = QString(pDescr).isEmpty() ? #pName : pDescr; mDefaults[#pName] = #pDefault; m_##pName = pDefault; }

#define QK_CONFIGPROPCMD(pType, pName, pDefault, pShortCut, pDescr) Q_PROPERTY(pType pName READ pName WRITE pName RESET pName##Reset STORED false) \
    private: pType m_##pName; \
    public: pType pName() const { return m_##pName; } \
    public: void pName(const pType& pValue) { m_##pName = pValue; } \
    public: void pName##Reset() { mShortCut[#pName] = pShortCut; \
        mDescriptions[#pName] = QString(pDescr).isEmpty() ? #pName : pDescr; mDefaults[#pName] = #pDefault; m_##pName = pDefault; }

namespace Qk
{
namespace Core
{

class QKCORE_EXPORT Config : public QObject
{
    Q_OBJECT

public:
    Config(QObject* pParent = 0);
    virtual ~Config();

public:
    virtual void init();
    virtual bool processCmdLine(int pArgc, char** pArgv, bool pHandleHelp = true, bool pHandleVersion = true);
    virtual bool processCmdLine(const QStringList& pArgs, bool pHandleHelp = true, bool pHandleVersion = true);
    virtual QString printHelp(QIODevice* pOutput = ConsoleDevice::stdOut());
    virtual QString printVersion(QIODevice* pOutput = ConsoleDevice::stdOut());
    virtual QString appDescription();
    virtual QString appHelp();
    QAbstractItemModel* model();

    virtual bool loadFromDefaultLocations();
    void loadJSON(const QString& pFileName);
    void loadJSONText(const QString& pJSON);
    QString saveJSON(const QString& pFileName = "");

private:
    void loadFile(const QString& pFileName);
    void applyJSONRecursive(const QJsonObject& pObj);
    QString formatHelpProperty(const QString& pName, int pMaxSz, const QString& pShort, const QString& pDesc, const QString& pType, const QString& pDefault);

protected:
    QMap<QString, QString> mDescriptions;
    QMap<QString, QString> mShortCut;
    QMap<QString, QString> mDefaults;
    Log mLog;

protected:
    QK_CONFIGPROPCMD(QString,  config,       "", "-c", "Load config from file specified")
    QK_CONFIGPROPCMD(QString,  sampleConf,   "", "", "Create sample of config in file name specified")
    QK_CONFIGPROPCMD(bool,  version,         false, "-V", "Show version information")
    QK_CONFIGPROPCMD(bool,  help,            false, "-h", "Show help information")
};

}
}
