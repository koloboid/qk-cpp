#ifndef MESSAGEBOX_HPP
#define MESSAGEBOX_HPP

#include <QDialog>
#include <QNetworkReply>

namespace Ui {
class MessageBox;
}

class MessageBox : public QDialog
{
    Q_OBJECT

private:
    explicit MessageBox(QWidget *parent = 0);

public:
    ~MessageBox();

    static void showError(QNetworkReply* pNetError);

private:
    Ui::MessageBox *ui;
};

#endif // MESSAGEBOX_HPP
