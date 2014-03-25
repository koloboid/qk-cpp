#ifndef QK_UI_CLIENT_VIEWHOST_HPP
#define QK_UI_CLIENT_VIEWHOST_HPP

#include <QObject>

namespace Qk {
namespace Ui {
namespace Client {

class ViewHost : public QObject
{
    Q_OBJECT
public:
    explicit ViewHost(QObject *parent = 0);

signals:

public slots:

};

} // namespace Client
} // namespace Ui
} // namespace Qk

#endif // QK_UI_CLIENT_VIEWHOST_HPP
