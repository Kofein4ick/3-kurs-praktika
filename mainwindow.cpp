#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent,TcpClient* client)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);//установка элементов формы
    Mclient=client;

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::closeEvent(QCloseEvent *event)//привязка закрытия главной формы к завершению работы программы
{
    event->accept();
    exit(0);
}

void MainWindow::on_Download_button_clicked()
{
    Mclient->slotClientSend(0,2);//отправляем сигнал
    tmr = new QTimer();
    connect(tmr, SIGNAL(timeout()), this, SLOT(slotTimerAlarm()));
    tmr->start(1000);//запускаем таймер на 1000мс
}

void MainWindow::slotTimerAlarm()
{
    if(Mclient->get_signal()==3)//проверка текущего значения сигнала
    {
        tmr->stop();//остановка таймера
        if(Mclient->get_jpgs().isEmpty())//проверка пустоты списка с именами файлов
        {
          QMessageBox::information(0,"Info","Изображений на сервере нет");
        }
        else
        {
            form=new downloadform(0,Mclient->get_jpgs(),Mclient);//открытие формы
            form->setWindowModality(Qt::ApplicationModal);//установка модальности
            form->show();//открытие формы

        }
    }
    else
    {
        tmr->start(1000);//перезапуск таймера
    }
}
void MainWindow::on_Upload_button_clicked()
{
    wid=new Widget(0,Mclient);
    wid->setWindowModality(Qt::ApplicationModal);//устновка модальности
    wid->show();//открытие формы
}
