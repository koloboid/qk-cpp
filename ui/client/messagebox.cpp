#include "messagebox.hpp"
#include "ui_messagebox.h"

#include <QDirIterator>
#include <QDebug>

MessageBox::MessageBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageBox)
{
    ui->setupUi(this);
    ui->imgIcon->setPixmap(QPixmap(":/res/img/info.png"));
    QDirIterator it(":", QDirIterator::Subdirectories);
    while (it.hasNext()) {
        qDebug() << it.next();
    }
}

MessageBox::~MessageBox()
{
    delete ui;
}

void MessageBox::showError(QNetworkReply *pNetError)
{
    MessageBox m;
    m.ui->pushButton->setText("sdfsdfdsa");
    m.show();
    m.exec();
}
