#include <QIODevice>
#include "formatter.hpp"

#pragma once

namespace Qk {
namespace Core {

class FormatterJson : public Formatter
{
public:
    FormatterJson(QIODevice* pOutDevice);

public:
    virtual QString contentType() const override { return "application/json"; }
    virtual QString extension() const override { return "json"; }

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

}
}
