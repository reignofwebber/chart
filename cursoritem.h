#ifndef CURSORITEM_H
#define CURSORITEM_H

#include <QGraphicsItem>
#include <QFont>

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

    QColor m_color;
    QColor m_textColor;
    QFont m_font;
    QString m_text;
    QRectF m_textRect;
};

#endif // CURSORITEM_H
