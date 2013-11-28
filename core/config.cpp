#include "config.hpp"
#include <QJsonDocument>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qmetaobject.h>
#include "log.hpp"

namespace Qk
{
namespace Core
{

Config::Config(QObject* pParent)
    :   QObject(pParent),
        mLog(log(), "CONFIG")
{
}

Config::~Config()
{
}

void Config::init()
{
    for (int i = 0; i < this->metaObject()->propertyCount(); i++)
    {
        QMetaProperty prop = this->metaObject()->property(i);
        prop.reset(this);
    }
}

bool Config::processCmdLine(int pArgc, char** pArgv, bool pHandleHelp, bool pHandleVersion)
{
    QStringList list;
    for (int i = 0; i < pArgc; i++) list.append(pArgv[i]);
    return processCmdLine(list, pHandleHelp, pHandleVersion);
}

bool Config::processCmdLine(const QStringList& pArgs, bool pHandleHelp, bool pHandleVersion)
{
    for (int i = 1; i < pArgs.size(); i++)
    {
        QString key = pArgs[i];
        QString propname = "";
        if (key.startsWith("--"))
        {
            propname = key.mid(2);
        }
        else if (key.startsWith("-"))
        {
            auto list = mShortCut.keys(key);
            if (list.size() > 0) propname = list[0];
        }
        int idx = this->metaObject()->indexOfProperty(propname.toUtf8());
        if (idx >= 0)
        {
            QMetaProperty prop = this->metaObject()->property(idx);
            QVariant val;
            if (prop.type() != QVariant::Bool)
            {
                if (i + 1 < pArgs.size())
                {
                    val = QVariant::fromValue(pArgs[++i]);
                }
                else
                {
                    mLog.warn("Value of type '%1' required for param '%2'").arg(prop.typeName()).arg(key);
                }
            }
            else
            {
                val = true;
            }
            if (val.isValid()) this->setProperty(propname.toUtf8(), val);
        }
        else
        {
            mLog.warn("Command line key '%1' is unknown. See --help for usage.").arg(key);
        }
    }
    if (pHandleHelp && help())
    {
        printHelp();
        return true;
    }
    if (pHandleVersion && version())
    {
        printVersion();
        return true;
    }
    return false;
}

void Config::loadJSON(const QString& pFileName)
{
    QFile file(pFileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        return throw Error(ERRLOC, tr("Unable to load config file from '%1'").arg(pFileName), file.errorString());
    }
    loadJSONText(QString::fromUtf8(file.readAll()));
}

void Config::loadJSONText(const QString& pJSON)
{
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(pJSON.toUtf8(), &err);
    if (doc.isNull())
    {
        return throw Error(ERRLOC, tr("Unable to load valid JSON config"), err.errorString());
    }
    return applyJSONRecursive(doc.object());
}

void Config::applyJSONRecursive(const QJsonObject& pObj)
{
    foreach (QString key, pObj.keys())
    {
        int idx = this->metaObject()->indexOfProperty(key.toUtf8());
        if (idx < 0) continue;
        QMetaProperty prop = this->metaObject()->property(idx);
        if (!prop.isValid())
        {
            mLog.warn("Property '%1' from config is not defined in config class").arg(key);
        }
        if (!prop.isStored()) continue;
        QJsonValue val = pObj[key];
        if (val.isObject())
        {
            applyJSONRecursive(val.toObject());
        }
        else
        {
            setProperty(key.toUtf8(), val.toVariant());
        }
    }
}

QAbstractItemModel* Config::model()
{
    throw ErrorNotImplemented(ERRLOC);
}

QString Config::printHelp(QIODevice* pOutput)
{
    QString help = "\n" + appDescription() + "\n\n";
    int maxsz = 0;
    for (int i = 0; i < this->metaObject()->propertyCount(); i++)
    {
        QMetaProperty prop = this->metaObject()->property(i);
        if (mDescriptions.contains(prop.name()))
        {
            QString name = prop.name();
            if (name.size() > maxsz) maxsz = name.size();
        }
    }
    for (int i = 0; i < this->metaObject()->propertyCount(); i++)
    {
        QMetaProperty prop = this->metaObject()->property(i);
        QString name = prop.name();
        if (mDescriptions.contains(name))
        {
            help += formatHelpProperty(name, maxsz, mShortCut[name], mDescriptions[name], prop.typeName(), mDefaults[name]);
        }
    }
    help += appHelp() + "\n";
    if (pOutput)
    {
        pOutput->write(help.toUtf8());
        ConsoleDevice::stdOut()->flush();
    }
    return help;
}

QString Config::formatHelpProperty(const QString& pName, int pMaxSz, const QString& pShort, const QString& pDesc, const QString& pType, const QString& pDefault)
{
    QString alignname = pName;
    for (int i = 0; i < pMaxSz - pName.size(); i++) alignname += ' ';
    QString rv = QString("--%1\t%2\t%3.")
        .arg(alignname)
        .arg(pShort)
        .arg(pDesc);
    if (!pType.isEmpty()) rv += " Type: " + pType + ".";
    if (!pDefault.isEmpty()) rv += " Default: " + pDefault + ".";
    return rv + "\n";
}

QString Config::printVersion(QIODevice* pOutput)
{
    QString vers = qApp->applicationName() + " version " + qApp->applicationVersion() + "\n";
    if (pOutput)
    {
        pOutput->write(vers.toUtf8());
        ConsoleDevice::stdOut()->flush();
    }
    return vers;
}

QString Config::appHelp()
{
    return "Override Qk::Core::Config::appHelp() to change this text";
}

QString Config::appDescription()
{
    return "Override Qk::Core::Config::appDescription() to change this text";
}

QString Config::saveJSON(const QString& pFileName)
{
    QJsonDocument doc;
    QJsonObject obj;
    for (int i = 0; i < this->metaObject()->propertyCount(); i++)
    {
        QMetaProperty prop = this->metaObject()->property(i);
        if (prop.isStored() && mDescriptions.contains(prop.name()))
        {
            QVariant v = prop.read(this);
            switch ((QMetaType::Type)v.type())
            {
                case QMetaType::Bool:
                    obj[prop.name()] = v.toBool();
                    break;
                case QMetaType::Double:
                case QMetaType::Float:
                case QMetaType::SChar:
                case QMetaType::Short:
                case QMetaType::Int:
                case QMetaType::LongLong:
                case QMetaType::Long:
                case QMetaType::UChar:
                case QMetaType::UShort:
                case QMetaType::UInt:
                case QMetaType::ULongLong:
                case QMetaType::ULong:
                    obj[prop.name()] = v.toDouble();
                    break;
                default:
                    obj[prop.name()] = v.toString();
                    break;
            }
        }
    }
    doc.setObject(obj);
    QString rv = doc.toJson();
    if (!pFileName.isEmpty())
    {
        QFile file(pFileName);
        if (!file.open(QIODevice::WriteOnly))
        {
            throw Error(ERRLOC, tr("Невозможно сохранить файл конфигурации в '%1'").arg(pFileName), file.errorString());
        }
        else
        {
            file.write(rv.toUtf8());
            file.flush();
        }
    }
    return rv;
}

void Config::loadFile(const QString& pFileName)
{
    if (QFile::exists(pFileName))
    {
        log()->info("Loading config from %1").arg(pFileName);
        loadJSON(pFileName);
    }
}

}
}
