#pragma once

#include <QObject>
#include <qk/rpc/context.hpp>

using namespace Qk::Rpc;

namespace Qk {
namespace Ui {
namespace Server {

class Widget : public QObject
{
    Q_OBJECT
public:
    explicit Widget(QObject *parent = 0);

public:
    Context* ctx() const { return mCurrentCtx; }
    virtual void show();
    QUuid id() const { return mID; }

    template<class T>
    T get(const QString& pWidgetName, const QString& pPropertyName, const T& pDefault = T())
    {
        return pDefault;
    }

private:
    Context* mCurrentCtx = nullptr;
    QUuid mID;
};

}
}
}
