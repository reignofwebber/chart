#include "cursoritem.h"

#include <QChart>
#include <QPainter>
using namespace QtCharts;

CursorItem::CursorItem(QtCharts::QChart *parent)
    : QGraphicsItem(parent)
    , m_chart(parent)
{
    m_color = QColor(0x626262);
    m_textColor = QColor(0x6D7AE0);
}

void CursorItem::setText(const QString &text)
{
    m_text = text;
    QFontMetrics metrics(m_font);
    m_textRect = metrics.boundingRect(QRect(0, 0, 150, 150), Qt::AlignLeft, m_text);
    m_textRect.translate(5, 5);
}


QRectF CursorItem::boundingRect() const
{
    QRectF rect;
    rect.setTopLeft(QPointF(0, 0));
    rect.setSize(QSizeF(3, m_chart->size().height()));
    return rect | m_textRect;
}

void CursorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen;
    pen.setColor(m_color);
    painter->setPen(pen);
    painter->drawLine(QPointF(0, 0), QPointF(0, m_chart->size().height()));
    pen.setColor(m_textColor);
    painter->setPen(pen);
    painter->drawText(m_textRect, m_text);
}
