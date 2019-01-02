#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QChartView>
#include <QChart>

#include <QDebug>

#include "cursoritem.h"



class ChartView : public QtCharts::QChartView
{
public:
    ChartView(QWidget *parent = 0)
        : QtCharts::QChartView(parent)
    {
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        setMouseTracking(true);

    }

    void setChart(QtCharts::QChart *chart)
    {
        m_chart = chart;
        QChartView::setChart(chart);
    }

    void addCursor(CursorItem * item)
    {
        m_item = item;
    }

protected:
    void mousePressEvent(QMouseEvent *event)
    {
        if(rubberBand() == QChartView::NoRubberBand && cursor().shape() == Qt::OpenHandCursor)
        {
            press_x = mapToScene(event->pos()).x();
            setCursor(Qt::ClosedHandCursor);
        }
        QChartView::mousePressEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent *event)
    {
        if(rubberBand() == QChartView::NoRubberBand && cursor().shape() == Qt::ClosedHandCursor)
        {
            qreal distance = press_x - mapToScene(event->pos()).x();
            this->chart()->scroll(distance, 0);
            setCursor(Qt::OpenHandCursor);
        }
        QChartView::mouseReleaseEvent(event);
    }

    void mouseMoveEvent(QMouseEvent *event)
    {
        QChartView::mouseMoveEvent(event);
        m_item->setPos(mapToScene(event->pos()).x(), 0);
    }

private:
    qreal press_x;      // 记录press点的x值

    QtCharts::QChart *m_chart;
    CursorItem *m_item;

};

#endif // CHARTVIEW_H
