#include "chartdelegate.h"

#include <QSpinBox>
#include <QCheckBox>
#include <QApplication>
#include <QStyle>
#include <QMouseEvent>

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

    if(index.column() == 0)
    {
        bool data = index.model()->data(index, Qt::UserRole).toBool();

        QStyleOptionButton checkBoxStyle;
        checkBoxStyle.state = data ? QStyle::State_On : QStyle::State_Off;
        checkBoxStyle.state |= QStyle::State_Enabled;

        QRect halfRect(option.rect.x() + option.rect.width()/4, option.rect.y() + option.rect.height()/4, option.rect.width()/2, option.rect.height()/2);
        checkBoxStyle.rect = halfRect;
        QCheckBox checkBox;
        QApplication::style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &checkBoxStyle, painter, &checkBox);
    }

}

bool ChartCheckBoxDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    QRect decorationRect(option.rect.x() + option.rect.width()/4, option.rect.y() + option.rect.height()/4, option.rect.width()/2, option.rect.height()/2);

    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    if(event->type() == QEvent::MouseButtonPress && decorationRect.contains(mouseEvent->pos()))
    {
        if(index.column() == 0)
        {
            bool data = model->data(index, Qt::UserRole).toBool();
            model->setData(index, !data, Qt::UserRole);
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
