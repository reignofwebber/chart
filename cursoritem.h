#ifndef CURSORITEM_H
#define CURSORITEM_H

#include <QGraphicsItem>

namespace QtCharts {
    class QChart;
}


class CursorItem : public QGraphicsItem
{
public:
    CursorItem(QtCharts::QChart *parent);

    void setText(const QString &text);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QtCharts::QChart *m_chart;
    qreal m_positionX;
};

#endif // CURSORITEM_H
