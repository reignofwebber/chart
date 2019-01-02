#include "chartview.h"

#include <QChart>
#include <QLineSeries>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QDateTime>
#include <QGraphicsLayout>

#include "cursoritem.h"



ChartView::ChartView(QWidget *parent)
    : QtCharts::QChartView(parent)
{
    // view set
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHint(QPainter::Antialiasing);
    setMouseTracking(true);

    // chart set
    //////////////////////////////////////////////////////////////////////////////
    //calculate digitalY
    m_digitalMaxY = (1 + digitalOffset) * m_poolSize * 2;
    for(int i = 0; i < m_poolSize; ++i)
    {
        m_digitalOffsetPool.append(i * (1 + digitalOffset));
    }

    //////////////////////////////////////////////////////////////////////////////
    // chart part
    // analog
    m_chart = new QChart;
    setChart(m_chart);
    m_chart->legend()->hide();


    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setFormat("yy-MM-dd h:mm:ss");
    auto now = QDateTime::currentDateTime();
    axisX->setMin(now);
    axisX->setMax(now.addSecs(m_totoalSeconds));
    axisX->setTickCount(5);
//    axisX->hide();
    m_chart->addAxis(axisX, Qt::AlignBottom);

    m_analogAxisY = new QValueAxis;
    m_analogAxisY->setRange(m_analogMinY, m_analogMaxY);
    m_analogAxisY->setLabelsVisible(false);
    m_analogAxisY->setTickCount(3);
    m_chart->addAxis(m_analogAxisY, Qt::AlignLeft);

    m_digitalAxisY = new QValueAxis;
    m_digitalAxisY->setRange(m_digitalMinY, m_digitalMaxY);
    m_digitalAxisY->setTickCount(2);
    m_digitalAxisY->hide();
    m_chart->addAxis(m_digitalAxisY, Qt::AlignLeft);


    m_chart->layout()->setContentsMargins(0, 0, 0, 0);
    m_chart->setBackgroundRoundness(0);
    m_chart->setMargins(QMargins(0, 0, 0, -1));


    // cursor item
    m_cursor = new CursorItem(m_chart);


    // init pool
    for(int i = 0; i < m_poolSize; ++i)
    {
        // analog series
        QLineSeries *series = new QLineSeries;
        m_chart->addSeries(series);

        m_analogSeriesPool.append(series);
        series->attachAxis(axisX);
        series->attachAxis(m_analogAxisY);

        // digital series
        series = new QLineSeries;
        m_chart->addSeries(series);

        m_digitalSeriesPool.append(series);
        series->attachAxis(axisX);
        series->attachAxis(m_digitalAxisY);

    }

}

void ChartView::mousePressEvent(QMouseEvent *event)
{
    if(rubberBand() == QChartView::NoRubberBand && cursor().shape() == Qt::OpenHandCursor)
    {
        press_x = mapToScene(event->pos()).x();
        setCursor(Qt::ClosedHandCursor);
    }
    QChartView::mousePressEvent(event);
}

void ChartView::mouseReleaseEvent(QMouseEvent *event)
{
    if(rubberBand() == QChartView::NoRubberBand && cursor().shape() == Qt::ClosedHandCursor)
    {
        qreal distance = press_x - mapToScene(event->pos()).x();
        this->chart()->scroll(distance, 0);
        setCursor(Qt::OpenHandCursor);
    }
    QChartView::mouseReleaseEvent(event);
}

void ChartView::mouseMoveEvent(QMouseEvent *event)
{
    QChartView::mouseMoveEvent(event);
    m_cursor->setPos(mapToScene(event->pos()).x(), 0);
}


//
void ChartView::setChartBold(ChartType type, const QString &id, bool bold)
{
    if(type == ANALOG_TYPE)
    {
        if(m_analogSeriesMap.contains(id))
        {
            QLineSeries *series = m_analogSeriesMap[id];
            QPen pen;
            pen.setWidth(bold ? 5 : 2);
            pen.setColor(series->color());
            series->setPen(pen);
        }

    }else if(type == DIGITAL_TYPE)
    {
        if(m_digitalSeriesMap.contains(id))
        {
            QLineSeries *series = m_digitalSeriesMap[id];
            QPen pen;
            pen.setWidth(bold ? 5 : 2);
            pen.setColor(series->color());
            series->setPen(pen);
        }
    }else
    {
        qDebug() << "setChartBold -> unsupport type " << type;
    }

}

void ChartView::setChartHalf()
{
    m_analogAxisY->setRange(-m_analogAxisY->max(), m_analogAxisY->max());
    m_digitalAxisY->setRange(0, m_digitalMaxY);
}

void ChartView::setChartFull(ChartType type)
{
    if(type == ANALOG_TYPE)
    {
        m_analogAxisY->setRange(0, m_analogAxisY->max());
    }
    else if(type == DIGITAL_TYPE)
    {
        m_digitalAxisY->setRange(0, m_digitalMaxY / 2);
    }
    else
    {
        qDebug() << "setChartFull -> unsupport type " << type;
    }
}
void ChartView::setChartShow(ChartType type, bool show)
{
    if(type == ANALOG_TYPE)
    {
        for(auto itr = m_analogSeriesMap.begin(); itr != m_analogSeriesMap.end(); ++itr)
        {
            if(show)
                itr.value()->show();
            else
                itr.value()->hide();
        }

    }
    else if(type == DIGITAL_TYPE)
    {
        for(auto itr = m_digitalSeriesMap.begin(); itr != m_digitalSeriesMap.end(); ++itr)
        {
            if(show)
                itr.value()->show();
            else
                itr.value()->hide();
        }

    }
    else
    {
        qDebug() << "setChartShow -> unsupport type " << type;
    }
}

void ChartView::addVariable(ChartType type, const QString &id, unsigned color)
{
    QLineSeries *series;
    if(type == ANALOG_TYPE)
    {
        if(m_analogSeriesPool.empty())
        {
            qDebug() << "m_digitalSeriesPool is empty";
            return;
        }
        series = m_analogSeriesPool.back();
        m_analogSeriesPool.pop_back();
        m_analogSeriesMap[id] = series;

    }
    else if(type == DIGITAL_TYPE)
    {
        if(m_digitalSeriesPool.empty())
        {
            qDebug() << "m_digitalSeriesPool is empty";
            return;
        }
        series = m_digitalSeriesPool.back();
        m_digitalSeriesPool.pop_back();
        m_digitalSeriesMap[id] = series;

        qreal offset = m_digitalOffsetPool.front();
        m_digitalOffsetPool.pop_front();
        m_digitalOffsetMap[id] = offset;

    }
    else
    {
        qDebug() << "addVariable -> unsupport type " << type;
    }

    QPen pen;
    pen.setWidth(2);
    pen.setColor(QColor(color));
    series->setPen(pen);
}

void ChartView::removeVariable(ChartType type, const QString &id)
{
    QLineSeries *series;

    if(type == ANALOG_TYPE)
    {
        if(m_analogSeriesMap.contains(id))
        {
            series = m_analogSeriesMap[id];
            m_analogSeriesMap.remove(id);
            series->clear();
            m_analogSeriesPool.append(series);
        }else
        {
            qDebug() << "removeVariable -> no registered id " << id;
        }
    }
    else if(type == DIGITAL_TYPE)
    {
        if(m_digitalSeriesMap.contains(id))
        {
            series = m_digitalSeriesMap[id];
            m_digitalSeriesMap.remove(id);
            series->clear();
            m_digitalSeriesPool.append(series);

            qreal offset = m_digitalOffsetMap[id];
            m_digitalOffsetMap.remove(id);
            m_digitalOffsetPool.append(offset);
            std::sort(m_digitalOffsetPool.begin(), m_digitalOffsetPool.end());
        }else
        {
            qDebug() << "removeVariable -> no registered id " << id;
        }
    }
    else
    {
        qDebug() << "removeVariable -> unsupport type " << type;
    }
}

void ChartView::onShowChanged(const QString &id, bool show)
{
    if(m_analogSeriesMap.contains(id))
    {
        if(show) m_analogSeriesMap[id]->show();
        else m_analogSeriesMap[id]->hide();
    }
    else if(m_digitalSeriesMap.contains(id))
    {
        if(show) m_digitalSeriesMap[id]->show();
        else m_digitalSeriesMap[id]->hide();
    }
    else
    {
        qDebug() << "showChanged: no registered id" << id;
    }
}

void ChartView::onColorChanged(const QString &id, unsigned color)
{
    if(m_analogSeriesMap.contains(id))
    {
        m_analogSeriesMap[id]->setColor(QColor(color));
    }
    else if(m_digitalSeriesMap.contains(id))
    {
        m_digitalSeriesMap[id]->setColor(QColor(color));
    }
    else
    {
        qDebug() << "colorChanged: no registered id" << id;
    }
}

void ChartView::resetAxisX()
{
    QDateTime now = QDateTime::currentDateTime();
    m_chart->axisX()->setRange(now, now.addSecs(m_totoalSeconds));
}

void ChartView::addPoint(QString id, qreal time, qreal val)
{
    if(m_analogSeriesMap.contains(id))
    {
        if(val < m_analogAxisY->min() || val > m_analogAxisY->max())
        {
            qreal positive = std::abs(val) + 10;
            m_analogAxisY->setMax(positive);
            // if min is negative
            if(m_analogAxisY->min() < -0.5)
                m_analogAxisY->setMin(-positive);
        }

        m_analogSeriesMap[id]->append(time, val);

        // update panel
        emit setValue(id, val);
    }
    else if(m_digitalSeriesMap.contains(id))
    {
        if(m_digitalSeriesMap[id]->count() > 0)
        {
            m_digitalSeriesMap[id]->append(time, m_digitalSeriesMap[id]->at(m_digitalSeriesMap[id]->count() - 1).y());
            m_digitalSeriesMap[id]->append(time, val + m_digitalOffsetMap[id]);
        }
        else
        {
            m_digitalSeriesMap[id]->append(time, val + m_digitalOffsetMap[id]);
        }

        // update panel
        emit setDigitalValue(id, val);
    }
    else
    {
        qDebug() << "unregistered id " << id;
        return;
    }

    // 更新chart
    m_chart->update();
}

QVector<QString> ChartView::getAllVariableIds()
{
    QVector<QString> ids;
    for(auto itr = m_analogSeriesMap.begin(); itr != m_analogSeriesMap.end(); ++itr)
    {
        ids.append(itr.key());
    }
    for(auto itr = m_digitalSeriesMap.begin(); itr != m_digitalSeriesMap.end(); ++itr)
    {
        ids.append(itr.key());
    }
    return ids;
}

void ChartView::addPointComplete()
{
    for(auto itr = m_analogSeriesMap.begin(); itr != m_analogSeriesMap.end(); ++itr)
    {
        QLineSeries *series = itr.value();
        if(series->count() == 0) break;
        QDateTimeAxis *axisX = dynamic_cast<QDateTimeAxis *>(m_chart->axisX());

        if(series->at(series->count() - 1).x() > axisX->max().toMSecsSinceEpoch())
        {
            quint64 totalSeconds = axisX->max().toSecsSinceEpoch() - axisX->min().toSecsSinceEpoch();
            if(totalSeconds < 1) totalSeconds = 1;
            qreal pertick = m_chart->plotArea().width()/totalSeconds;
            m_chart->scroll(pertick, 0);
            return;
        }
    }
    for(auto itr = m_digitalSeriesMap.begin(); itr != m_digitalSeriesMap.end(); ++itr)
    {
        QLineSeries *series = itr.value();
        if(series->count() == 0) break;
        QDateTimeAxis *axisX = dynamic_cast<QDateTimeAxis *>(m_chart->axisX());

        if(series->at(series->count() - 1).x() > axisX->max().toMSecsSinceEpoch())
        {
            quint64 totalSeconds = axisX->max().toSecsSinceEpoch() - axisX->min().toSecsSinceEpoch();
            if(totalSeconds < 1) totalSeconds = 1;
            qreal pertick = m_chart->plotArea().width()/totalSeconds;
            m_chart->scroll(pertick, 0);
            return;
        }
    }
}