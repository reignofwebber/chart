#ifndef CHARTDELEGATE_H
#define CHARTDELEGATE_H

#include <QStyledItemDelegate>

class ChartCheckBoxDelegate : public QStyledItemDelegate
{
public:
    ChartCheckBoxDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

};


class ChartButtonDelegate : public QStyledItemDelegate
{
public:
    ChartButtonDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
};


#endif // CHARTDELEGATE_H
