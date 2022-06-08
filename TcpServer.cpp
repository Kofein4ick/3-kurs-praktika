#include "TcpServer.h"
#include <QDebug>
#include <QCoreApplication>

TcpServer::TcpServer(QObject *parent) : QObject(parent)
{
    QDir dir(path);
    if (!dir.exists()) {//проеверка существования базовой папки
        dir.mkpath(path);//если не существует, то создаем
    }
    form=new ServerForm;
    form->show();//открытие формы сервера
    mTcpServer = new QTcpServer(this);
    connect(mTcpServer, SIGNAL(newConnection()),SLOT(slotNewConnection()));
    if(!mTcpServer->listen(QHostAddress::Any, 6000)){//прослушивание любого адреса, 6000 порта
         QMessageBox::critical(0,"Error","server is not started");
         exit(1);
    } else {
       form->printtext("server is started\n");
    }
}
TcpServer::~TcpServer(){};
void TcpServer::slotNewConnection()
{
    form->printtext("server connected\n");
    msTcpSocket= mTcpServer->nextPendingConnection();//получение сокета
    connect(msTcpSocket, SIGNAL(readyRead()), this,SLOT(slotServerRead()));
    connect(msTcpSocket,  SIGNAL(disconnected()), this, SLOT(slotClientDisconnected()));
}

void TcpServer::slotServerRead()
{
    QByteArray meta;
    QDataStream metas(&meta,QIODeviceBase::ReadOnly);
    if(signal!=2){
        QDataStream in(msTcpSocket);
        in>>signal;//получение сигнала
    }
    switch(signal){//в зависимости от сигнала
    case 1:{//получение метаданных
            while(msTcpSocket->bytesAvailable()>0){
                meta = msTcpSocket->readAll();//считываем все полученные данные
            }
            //заполняем переменные
            metas >> sizeInBytes;
            metas >> format;
            metas >> width;
            metas >> height;
            metas >> filename;
            break;
        }
    case 2:{//получение изображения
            QByteArray buf;
            QDataStream imgs(&imga,QIODeviceBase::ReadOnly);
            while(msTcpSocket->bytesAvailable()>0)
            {
                buf = msTcpSocket->readAll();//считываем все полученные данные
                imga.append(buf);//заполняем массив изображения
            }
            if(imga.size()==sizeInBytes)//проверяем текущий размер и сравниваем с переданынм
            {
                //получаем полный путь изображения
                QString filepath=path;
                filepath.append(filename);
                uchar* bytes = new uchar[sizeInBytes];
                imgs.readRawData((char*)bytes, sizeInBytes);
                QImage* img2 = new QImage(bytes, width, height, (QImage::Format)format);//создаем изображение с полученными данными
                img2->save(filepath);//сохранение изображения
                filename.append(" saved");
                form->printtext(filename);
                this->slotServerSend(0,3);//отправка сигнала о получении изображения
                //очистка переменных
                imga.clear();
                filename.clear();
                sizeInBytes=0;
                format=0;
                width=0;
                height=0;
            }
            break;
        }
    case 3:{//отправка списка
            this->slotServerSend(0,2);
            break;
        }
    case 5:{//отправка запрошенного изображения
        QString str;
        QByteArray buf;
        QDataStream imgs(&imga,QIODeviceBase::ReadOnly);
        while(msTcpSocket->bytesAvailable()>0)
        {
           buf = msTcpSocket->readAll();//считываем все полученные данные
           imga.append(buf);//заполняем массив
        }
        imgs>>str;//получаем строку с именем файла
        QString str2=str;
        str2.append(" send");
        imga.clear();
        this->slotServerSend(str,1);//отправка файла
        form->printtext(str2);
        break;
        }
    }
}

void TcpServer::slotServerSend(QString str,unsigned int mode)
{
    QByteArray meta;
    QDataStream metas(&meta,QIODeviceBase::WriteOnly);
    switch(mode)
    {
    case 1://отправка изображения
    {
        QImage* img = new QImage(str);
        //получение имени файла
        QFile f(str);
        QFileInfo fileInfo(f.fileName());
        QString filename(fileInfo.fileName());
        QByteArray imga;
        QDataStream imgs(&imga,QIODeviceBase::WriteOnly);
        sizeInBytes=img->sizeInBytes();
        format=(int)img->format();
        signal=1;
        //заполнение метаданных
        metas << signal;
        metas << sizeInBytes;
        metas << format;
        metas << img->width();
        metas << img->height();
        metas << filename;
        //заполнение данных изображения
        signal=2;
        imgs << signal;
        imgs.writeRawData((char*)img->bits(), img->sizeInBytes());
        //отправка метаданных, затем массива с байтами изображения
        msTcpSocket->write(meta);
        msTcpSocket->write(imga);
        signal=0;
        break;
    }
    case 2:{//отправка списка

        signal=3;
        QStringList jpgs =this->images();//получение списка
        //отправка сигнала и списка
        metas << signal;
        metas << jpgs;
        msTcpSocket->write(meta);
        form->printtext("List send");
        break;
    }
    case 3:{//отправка сигнала о получении
        signal=4;
        metas << signal;
        msTcpSocket->write(meta);
        break;
    }

    }
}

void TcpServer::slotClientDisconnected()
{
    msTcpSocket->close();
    form->close();
}
QStringList TcpServer::images()//составление списка и получение полных имен файлов в списке
{
    QDir dir(path);
    QStringList jpgs = dir.entryList(QStringList()<<"*.jpg" << "*.jpeg");//фильтр
    QStringList jpgs2;
    foreach (QString file, jpgs)
    {
        jpgs2.append(dir.absoluteFilePath(file));
    }
    return jpgs2;
}
