#include "TcpClient.h"
#include <QDebug>
#include <QCoreApplication>

TcpClient::TcpClient(QObject *parent,const QString&strHost,int nPort) : QObject(parent)
{
    mTcpSocket = new QTcpSocket(this);
    mTcpSocket->connectToHost(strHost, nPort);//подклчение к хосту по заданному адресу и порту
    connect(mTcpSocket, SIGNAL(connected()),SLOT(slotConnected()));
    connect(mTcpSocket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)),SLOT(slotError(QAbstractSocket::SocketError)));
    QDir dir(filepath);
    if (!dir.exists()) {//проеверка существования базовой папки
        dir.mkpath(filepath);//если не существует, то создаем
    }
}
TcpClient::~TcpClient(){};
void TcpClient::slotConnected()
{
       connect(mTcpSocket, SIGNAL(readyRead()),SLOT(slotClientRead()));
}

void TcpClient::slotClientRead()
{
    if((signal!=2)&&(signal!=3)){
        QDataStream in(mTcpSocket);
        in>>signal;//получение сигнала
    }
    switch(signal){//в зависимости от сигнала
    case 1:{//получение метаданных
            QByteArray meta;
            QDataStream metas(&meta,QIODeviceBase::ReadOnly);
            while(mTcpSocket->bytesAvailable()>0){
                meta = mTcpSocket->readAll();//считываем все полученные данные
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
            while(mTcpSocket->bytesAvailable()>0)
            {
                buf = mTcpSocket->readAll();//считываем все полученные данные
                imga.append(buf);//заполняем массив изображения
            }
            if(imga.size()==sizeInBytes)//проверяем текущий размер и сравниваем с переданынм
            {
                //получаем полный путь изображения
                QString path=filepath;
                path.append(filename);
                uchar* bytes = new uchar[sizeInBytes];
                imgs.readRawData((char*)bytes, sizeInBytes);
                QImage* img2 = new QImage(bytes, width, height, (QImage::Format)format);//создаем изображение с полученными данными
                img2->save(path);//сохранение изображения
                filename.append(" скачано");
                QMessageBox::information(0,"Info",filename);//вывод информационного окна
                //очистка переменных
                imga.clear();
                sizeInBytes=0;
                format=0;
                width=0;
                height=0;
            }
            break;
        }
    case 3:{//получение списка имен файлов
            QByteArray buf;
            QDataStream imgs(&imga,QIODeviceBase::ReadOnly);
            while(mTcpSocket->bytesAvailable()>0)
            {
                buf = mTcpSocket->readAll();//считываем все полученные данные
                imga.append(buf);//заполняем
            }
            imgs>>jpgs;//заполнение списка
            imga.clear();//очистка массива
            break;
        }
    case 4:{
            break;
        }
    }
}

void TcpClient::slotClientSend(QString str, unsigned int mode)
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
        //заполнение метаданных
        signal=1;
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
        mTcpSocket->write(meta);
        mTcpSocket->write(imga);
        signal=0;
        break;
    }
    case 2://отправка запроса на получение списка
    {
        signal=3;
        metas<<signal;
        mTcpSocket->write(meta);
        signal=0;
        break;
    }
    case 3://отправка имени изображения, запрошенного на скачивание
    {
        signal=5;
        metas<<signal;
        metas<<str;
        mTcpSocket->write(meta);
        signal=0;
        break;
    }
    }
}

void TcpClient::slotError(QAbstractSocket::SocketError err)//обработка ошибок сокета
{
    QString strError =
        "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                     "The host was not found." :
                     err == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed." :
                     err == QAbstractSocket::ConnectionRefusedError ?
                     "The connection was refused." :
                     QString(mTcpSocket->errorString())
                    );
    QMessageBox::critical(0,"Error",strError);
}
QStringList TcpClient::get_jpgs()//метод получения списка имен
{
    return jpgs;
}
quint16 TcpClient::get_signal()//метод получения сигнала
{
    return signal;
}
