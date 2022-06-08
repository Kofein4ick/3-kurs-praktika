#include "serverform.h"
#include "ui_serverform.h"

ServerForm::ServerForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerForm)
{
    ui->setupUi(this);//установка элементов формы
    ui->textEdit->setReadOnly(true);//установка формата элемента textEdit только для чтения
}

ServerForm::~ServerForm()
{
    delete ui;
}
void ServerForm::printtext(QString str)
{
      ui->textEdit->append(str);//вывод текста в textEdit
}
