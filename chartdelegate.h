#ifndef CHARTDELEGATE_H
#define CHARTDELEGATE_H

#include <QStyledItemDelegate>

class ChartCheckBoxDelegate : public QStyledItemDelegate
{
public:
    ChartCheckBoxDelegate(QObject *parent = 0);

//    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
//                          const QModelIndex &index) const override;

//    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
//    void setModelData(QWidget *editor, QAbstractItemModel *model,
//                      const QModelIndex &index) const override;

//    void updateEditorGeometry(QWidget *editor,
//        const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
};

#endif // CHARTDELEGATE_H
