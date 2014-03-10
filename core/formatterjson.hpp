#include <QIODevice>
#include "formatter.hpp"

#pragma once

namespace Qk {
namespace Core {

class FormatterJson : public Formatter
{
public:
    FormatterJson(QIODevice* pOutDevice = nullptr);

public:
    virtual QString contentType() const override { return "application/json; charset=utf-8"; }
    virtual QString extension() const override { return "json"; }

    virtual QIODevice* flush() override;
    virtual Formatter& startObject(const QString& pName) override;
    virtual Formatter& endObject() override;
    virtual Formatter& startArray(const QString& pName) override;
    virtual Formatter& endArray() override;
    virtual Formatter& write(const QString& pName, const QString& pValue);
    virtual Formatter& write(const QString& pName, const char* pValue);
    virtual Formatter& write(const QString& pName, const QVariant& pValue) override;

protected:
    void writeHead(const QString& pName);
    QString escapeStr(const QString& pStr);

private:
    enum EState
    {
        Start,
        NeedComma,
    };

private:
    EState mState = Start;
    QString mToFinishBrace;
};

}
}
