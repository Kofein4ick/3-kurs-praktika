#ifndef IMAGEDELEGATE_H
#define IMAGEDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QPixmap>
#include <QDebug>

class ImageDelegate : public QStyledItemDelegate
{
public:
    explicit ImageDelegate(QStandardItemModel *model, QObject *parent = nullptr);

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;//метод отрисовки изображения
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;//метод корректировки размера изображения
    QStandardItemModel* m_model;
};


#endif // IMAGEDELEGATE_H
