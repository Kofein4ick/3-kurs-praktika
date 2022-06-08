#include "downloadform.h"
#include "ui_downloadform.h"

downloadform::downloadform(QWidget *parent,QStringList strlst,TcpClient* client) :
    QWidget(parent),
    ui(new Ui::downloadform)
{
    ui->setupUi(this);//установка элементов формы
    ui->listWidget->addItems(strlst);//добавление элементов в список
    dclient=client;
}

downloadform::~downloadform()
{
    delete ui;
}



void downloadform::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    file=item->text();//получение имени файла
    dclient->slotClientSend(file,3);//отправка имени выбранного файла
    this->close();//закрытие формы
}
