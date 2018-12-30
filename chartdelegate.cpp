#include "chartdelegate.h"

#include "chartmodel.h"
#include <QSpinBox>
#include <QCheckBox>
#include <QApplication>
#include <QStyle>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

ChartCheckBoxDelegate::ChartCheckBoxDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

void ChartCheckBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    if(option.state.testFlag(QStyle::State_HasFocus))
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;
    QStyledItemDelegate::paint(painter, viewOption, index);


    bool data = index.model()->data(index, Qt::UserRole).toBool();

    QCheckBox checkBox;
    if(index.column() == COL_SHOW)
        checkBox.setStyleSheet("QCheckBox::indicator{\n	width:20px;\n	height:20px;\n}\n\nQCheckBox::indicator::unchecked{\n	image:url(:/images/unchecked_checkbox.png)\n}\n\nQCheckBox::indicator::checked{\n	image:url(:/images/checked_checkbox.png)\n}");
    else if(index.column() == COL_STAR)
        checkBox.setStyleSheet("QCheckBox::indicator{\n	width:20px;\n	height:20px;\n}\n\nQCheckBox::indicator::unchecked{\n	image:url(:/images/star_unstar.png)\n}\n\nQCheckBox::indicator::checked{\n	image:url(:/images/star_star.png)\n}");
    checkBox.setChecked(data);
    checkBox.resize(option.rect.size());
    painter->save();
    painter->translate(option.rect.x() + (option.rect.width()-20)/2, option.rect.y());
    checkBox.render(painter);
    painter->restore();


}

bool ChartCheckBoxDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    QRect decorationRect(option.rect.x() + option.rect.width()/4, option.rect.y() + option.rect.height()/4, option.rect.width()/2, option.rect.height()/2);

    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    if(event->type() == QEvent::MouseButtonPress && decorationRect.contains(mouseEvent->pos()))
    {
        bool data = model->data(index, Qt::UserRole).toBool();
        model->setData(index, !data, Qt::UserRole);
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
