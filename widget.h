#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPalette>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDropEvent>
#include <QScrollArea>
#include <QPushButton>
#include <QLabel>
#include <QListView>
#include <QGridLayout>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QTimer>
#include <QTime>
#include "TcpClient.h"

class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = 0,TcpClient* client=0);
    ~Widget();

    // Метод события перетаскивания
    virtual void dragEnterEvent(QDragEnterEvent* event) override;
    // Метод события отпускания объекта с данными
    virtual void dropEvent(QDropEvent *event) override;

private slots:
    // Слот для обработки кликов по элементам списка
    void onImagesListViewClicked(const QModelIndex& index);
    void on_m_button_clicked();
    void slotTimerAlarm();

private:
    QScrollArea*        m_scrollArea;// Область скроллинга изображения
    QLabel*             m_imageLabel;// Лейбл для отображения картинок
    QListView*          m_imagesListView;// Список с изображениями
    QGridLayout*        m_gridLayout;// Сетка для интерфейса
    QStandardItemModel* m_imagesModel;// Модель данных с изображениями
    QPushButton*        m_button;//Кнопка
    TcpClient*          wid_client;//Указатель для работы с функционалом клиента
    QTimer*             tmr;//Таймер
};

#endif // WIDGET_H
