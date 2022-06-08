#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "widget.h"
#include "downloadform.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr,TcpClient* client=0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void on_Download_button_clicked();
    void on_Upload_button_clicked();
    void slotTimerAlarm();

private:
    Ui::MainWindow *ui;//указатель для работы с элементами интерфейса формы
    Widget *wid;//указатель формы загрузки изображений
    downloadform *form;//указатель формы скачивания изображений
    TcpClient* Mclient;//указатель для работы с функционалом клиента
    QTimer* tmr;//таймер
};
#endif // MAINWINDOW_H
