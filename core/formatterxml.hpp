#pragma once

#include "formatter.hpp"

namespace Qk {
namespace Core {
/*
class FormatterXml : public Formatter
{
public:
    FormatterXml(QIODevice* pOutDevice);

public:
    virtual QString contentType() const override { return "application/xml"; }
    virtual QString extension() const override { return "xml"; }

    virtual QVariantMap read() override;
    virtual Formatter& startObject(const QString& pName) override;
    virtual Formatter& endObject() override;
    virtual Formatter& startArray(const QString& pName) override;
    virtual Formatter& endArray() override;
    virtual Formatter& write(const QString& pName, const QVariant& pValue) override;
    virtual QIODevice* flush() override { return mOut; }

private:
    QIODevice* mOut;
};
*/
}
}
