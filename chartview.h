#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QChartView>

#include <QDebug>


class ChartView : public QtCharts::QChartView
{
public:
    ChartView(QWidget *parent = 0)
        : QtCharts::QChartView(parent)
    {

    }

protected:
    void mousePressEvent(QMouseEvent *event)
    {
        if(rubberBand() == QChartView::NoRubberBand && cursor() == Qt::OpenHandCursor)
        {
            press_x = mapToScene(event->pos()).x();
            setCursor(Qt::ClosedHandCursor);
        }
        QChartView::mousePressEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent *event)
    {
        if(rubberBand() == QChartView::NoRubberBand && cursor() == Qt::ClosedHandCursor)
        {
            qreal distance = press_x - mapToScene(event->pos()).x();
            qDebug() << "release," << distance;
            this->chart()->scroll(distance, 0);
            setCursor(Qt::OpenHandCursor);
        }
        QChartView::mouseReleaseEvent(event);
    }

private:
    qreal press_x;      // 记录press点的x值

};

#endif // CHARTVIEW_H
