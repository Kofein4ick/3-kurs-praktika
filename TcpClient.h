#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QImage>
#include <QDir>
#include <QMessageBox>
#include <QFileInfo>
#include <QDataStream>



class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent=0,const QString&strHost=0,int nPort=0);
    ~TcpClient();
    quint16 get_signal();//метод для получения значения сигнала
    QStringList get_jpgs();//метод для получения списка путей к файлам
public slots:
    void slotConnected();
    void slotClientRead();
    void slotClientSend(QString str,unsigned int mode);
    void slotError(QAbstractSocket::SocketError);

private:
    QTcpSocket * mTcpSocket;//указатель для работы с сокетом
    QByteArray imga;//массив для хранения изображения
    int format=0;//переменная для хранения формата
    unsigned int sizeInBytes=0;//переменная для хранения размера изображения
    int width=0;//переменная для хранения ширины изображения
    int height=0;//переменная для хранения длинны ихображения
    QString filename;//строка для пути к файлу
    QStringList jpgs;//список строк с путями к файлам
    quint16 signal=4;//сигнал
    QString filepath="D:/PracticaClient/";//путь к базовой папке клиента

};

#endif // TCPCLIENT_H
