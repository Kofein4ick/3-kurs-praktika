#ifndef SERVERFORM_H
#define SERVERFORM_H

#include <QWidget>

namespace Ui {
class ServerForm;
}

class ServerForm : public QWidget
{
    Q_OBJECT

public:
    explicit ServerForm(QWidget *parent = nullptr);
    ~ServerForm();
    void printtext(QString str);//метод для вывода текста

private:
    Ui::ServerForm *ui;//указатель для работы с элементами интерфейса формы
};

#endif // SERVERFORM_H
