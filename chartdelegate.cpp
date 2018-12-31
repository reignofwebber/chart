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
    bool data = index.model()->data(index, Qt::UserRole).toBool();

    QCheckBox checkBox;
    if(index.column() == COL_SHOW)
        checkBox.setStyleSheet("QCheckBox::indicator{\n	width:20px;\n	height:20px;\n}\n\nQCheckBox::indicator::unchecked{\n	image:url(:/images/unchecked_checkbox.png)\n}\n\nQCheckBox::indicator::checked{\n	image:url(:/images/checked_checkbox.png)\n}");
    else if(index.column() == COL_STAR)
        checkBox.setStyleSheet("QCheckBox::indicator{\n	width:20px;\n	height:20px;\n}\n\nQCheckBox::indicator::unchecked{\n	image:url(:/images/star_unstar.png)\n}\n\nQCheckBox::indicator::unchecked::hover{\n	image:url(:/images/star_hover.png)\n}\n\nQCheckBox::indicator::checked{\n	image:url(:/images/star_star.png)\n}");
    checkBox.setChecked(data);
    checkBox.resize(option.rect.size()/2);
    painter->save();
    painter->translate(option.rect.x() + (option.rect.width() - 20)/2, option.rect.y() + (option.rect.height() - 20)/2);
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <QPushButton>


ChartButtonDelegate::ChartButtonDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

void ChartButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    unsigned data = index.model()->data(index, Qt::UserRole).toUInt();
    QColor color(data);

    QPushButton btn;
    btn.setStyleSheet(QString("background:rgb(%1, %2, %3);\nborder-radius:10px;").arg(color.red()).arg(color.green()).arg(color.blue()));
    btn.resize(option.rect.size() * 3 / 4);
    painter->save();
    painter->translate(option.rect.x() + option.rect.width() / 8, option.rect.y() + option.rect.height() / 8);
    btn.render(painter);
    painter->restore();

}

bool ChartButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    QRect decorationRect(option.rect.x() + option.rect.width()/8, option.rect.y() + option.rect.height()/8, option.rect.width()*3/4, option.rect.height()*3/4);

    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    if(event->type() == QEvent::MouseButtonPress && decorationRect.contains(mouseEvent->pos()))
    {
        unsigned data = 0;
        data += (std::rand() % 256) << 16;
        data += (std::rand() % 256) << 8;
        data += (std::rand() % 256);
        model->setData(index, data, Qt::UserRole);
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
