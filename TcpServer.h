#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QImage>
#include <QMessageBox>
#include <QDir>
#include <QFileInfo>
#include <QDataStream>
#include "serverform.h"

class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);
    ~TcpServer();
    QStringList images();//метод для получения списка путей к файлам

public slots:
    void slotNewConnection();
    void slotServerRead();
    void slotServerSend(QString str,unsigned int mode);
    void slotClientDisconnected();
private:
    QTcpServer * mTcpServer;//указатель для работы с функционалом сервера
    QTcpSocket * msTcpSocket;//указатель для работы с функционало сокета
    QByteArray imga;//массив для хранения изображений
    quint16 signal=0;//переменная для сигнала
    int format=0;//переменная для формата изображения
    unsigned int sizeInBytes=0;//переменная для размера изображения
    int width=0;//переменная для ширины изображения
    int height=0;//переменная для высоты изображения
    QString filename;//строка для имени файла
    QString path="D:/Practica/";//базовая папка сервера
    ServerForm* form;//указатель формы сервера
};

#endif // TCPSERVER_H
