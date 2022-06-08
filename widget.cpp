#include "widget.h"
#include "ui_widget.h"
#include <QStandardItem>
#include "imagedelegate.h"
//счётчики
unsigned int counts=0;
unsigned int i=0;
QString images[5];//массив имен файлов

Widget::Widget(QWidget *parent,TcpClient* client) :
    QWidget(parent)
{
    setAcceptDrops(true);       // разрешаем события отпускания объектов данных
    setMinimumWidth(640);
    setMinimumHeight(480);
    wid_client=client;
    counts=0;
    for(int j=0;j<5;j++)
        images[i].clear();
    /// Настраиваем интерфейс
    m_gridLayout = new QGridLayout(this);
    m_imagesListView = new QListView(this);

    // Создадим модель данных для списка изображений
    m_imagesModel = new QStandardItemModel(m_imagesListView);
    m_imagesListView->setModel(m_imagesModel);  // Установим модель во вьюшку для превью изображений
    m_imagesListView->setFixedWidth(200);

    // Без делегата не удастся избавиться от текста в элементе списка и настроить отображение превью
    m_imagesListView->setItemDelegate(new ImageDelegate(m_imagesModel, m_imagesListView));
    m_button=new QPushButton("Отправить");
    // Настраиваем область скроллинга для текущего изображения
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setBackgroundRole(QPalette::Dark);
    m_imageLabel = new QLabel(this);
    m_scrollArea->setWidget(m_imageLabel);
    m_gridLayout->addWidget(m_scrollArea, 0, 0);
    m_gridLayout->addWidget(m_imagesListView, 0, 1);
    m_gridLayout->addWidget(m_button, 0, 2);

    connect(m_imagesListView, &QListView::clicked, this, &Widget::onImagesListViewClicked);
    connect(m_button, SIGNAL(clicked()), this, SLOT(on_m_button_clicked()));
}
void Widget::on_m_button_clicked()
{
    tmr = new QTimer();
    connect(tmr, SIGNAL(timeout()), this, SLOT(slotTimerAlarm()));
    tmr->start(1000);//запуск таймера
}

void Widget::slotTimerAlarm()
{
    if(i==counts)//проверка кол-ва отправленых изображений
    {
        i=0;counts=0;
        for(int j=0;j<5;j++)
            images[i].clear();
        tmr->stop();//остановка таймера
        QMessageBox::information(0,"Info","Все изображения успешно переданы");
        this->close();//закрытие формы
    }
    else
    {
        if(wid_client->get_signal()==4)//если сервер готов принимать следующее изображение
           {
                wid_client->slotClientSend(images[i],1);//отправка изображения
                i++;
           }
        tmr->start(1000);//перезапуск таймера
    }
}
Widget::~Widget()
{
    counts=0;i=0;
    for(int j=0;j<5;j++)
        images[i].clear();
}

void Widget::dragEnterEvent(QDragEnterEvent *event)
{
    if(counts==5)//ограничиваем кол-во загружаемых изображений
    {
        event->ignore();//блокируем событие
        QMessageBox::information(0,"Info","Можно отправить только 5 изображений за раз");
    }
    else  // Обязательно необходимо допустить событие переноса данных в область окна приложения
        event->accept();
}

void Widget::dropEvent(QDropEvent *event)
{
    // Когда отпускаем файл в область приложения,
    // то забираем путь к файлу из MIME данных
    QFileInfo fi(event->mimeData()->urls()[0].toLocalFile());
    QString ext = fi.suffix();
    if((ext!="jpg")&&(ext!="jpeg"))
    {
        QMessageBox::warning(0,"Warning","Загрузите изображение jpg или jpeg");
    }
    else{
    QString filePath = event->mimeData()->urls()[0].toLocalFile();
    images[counts]=filePath;//заполняем массив имён
    counts++;
    // Создаём изображение
    QPixmap pixmap(filePath);
    // Помещаем его в область скроллинга через QLabel
    m_imageLabel->setPixmap(pixmap);
    m_imageLabel->resize(pixmap.size());

    // Добавляем элемент в список
    m_imagesModel->appendRow(new QStandardItem(QIcon(pixmap), filePath));
    }
}

void Widget::onImagesListViewClicked(const QModelIndex &index)
{
    // Когда кликаем по элементу в списке, то забираем путь к файлу
    QPixmap pixmap(m_imagesModel->data(index).toString());
    // И устанавливаем файл в область основного просмотра
    m_imageLabel->setPixmap(pixmap);
    m_imageLabel->resize(pixmap.size());
}
