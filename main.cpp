#include "mainwindow.h"
#include "TcpClient.h"
#include "TcpServer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TcpServer server;
    TcpClient client(0,"127.0.0.1",6000);
    MainWindow w(0,&client);
    w.show();
    return a.exec();
}
