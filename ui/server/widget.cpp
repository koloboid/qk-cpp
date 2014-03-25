#include "widget.hpp"

namespace Qk {
namespace Ui {
namespace Server {

Widget::Widget(QObject *parent) :
    QObject(parent), mID(QUuid::createUuid())
{
}

void Widget::show()
{

}

}
}
}
