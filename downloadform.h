#ifndef DOWNLOADFORM_H
#define DOWNLOADFORM_H

#include <QWidget>
#include <QListWidget>
#include "TcpClient.h"
namespace Ui {
class downloadform;
}

class downloadform : public QWidget
{
    Q_OBJECT

public:
    explicit downloadform(QWidget *parent = nullptr,QStringList strlst = QStringList()<< "",TcpClient* client=0);
    ~downloadform();
    QString get_file();//метод получения имени файла

private slots:

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);//метод обработки события двойного нажатия по элемнету списка

private:
    Ui::downloadform *ui;//указатель для работы с элементами интерфейса формы
    QString file;//строка содержащая полный путь к файлу
    TcpClient* dclient;//указатель для работы с функционалом клиента
};

#endif // DOWNLOADFORM_H
