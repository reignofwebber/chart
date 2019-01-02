#include "cursoritem.h"

#include <QChart>
#include <QPainter>
using namespace QtCharts;

CursorItem::CursorItem(QtCharts::QChart *parent)
    : QGraphicsItem(parent)
    , m_chart(parent)
{

}

void CursorItem::setText(const QString &text)
{

}


QRectF CursorItem::boundingRect() const
{
    QRectF rect;
    rect.setTopLeft(QPointF(0, 0));
    rect.setSize(QSizeF(3, m_chart->size().height()));
    return rect;
}

void CursorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawLine(QPointF(0, 0), QPointF(0, m_chart->size().height()));
}
