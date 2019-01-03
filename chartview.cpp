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
    m_activeCursor = new CursorItem(m_chart);
    m_activeCursor->hide();


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

    if(m_activeCursor->isVisible())
    {
        m_cursors.append(m_activeCursor);
        m_cursorMap[m_chart->mapToValue(event->pos()).x()] = m_activeCursor;
        m_activeCursor = new CursorItem(m_chart);
    }

    QChartView::mousePressEvent(event);
}

void ChartView::mouseReleaseEvent(QMouseEvent *event)
{
    if(rubberBand() == QChartView::NoRubberBand && cursor().shape() == Qt::ClosedHandCursor)
    {
        qreal distance = press_x - event->pos().x();
        this->chart()->scroll(distance, 0);
        setCursor(Qt::OpenHandCursor);
    }

    // let parent do scaling
    QChartView::mouseReleaseEvent(event);

    // finally update cursor's position
    updateCursorsPos();
    updateMinMaxPos();
}

void ChartView::mouseMoveEvent(QMouseEvent *event)
{
    QChartView::mouseMoveEvent(event);
    m_activeCursor->setPos(event->pos().x(), 0);

    quint64 time = m_chart->mapToValue(event->pos()).x();

    QString text = QString("\n时间: ");
    text.append(QDateTime::fromMSecsSinceEpoch(time).toString("yyyy.MM.dd-hh:mm:ss.zzz\n\n"));

    for(auto itr = m_analogSeriesMap.begin(); itr != m_analogSeriesMap.end(); ++itr)
    {
        QLineSeries *series = itr.value();
        if(!series->isVisible()) continue;
        int index = getIndex(itr.key(), time);
        if(index == -1) continue;
        text.append(m_id_name_map[itr.key()]);
        text.append(QString(": %1\n").arg(interpolateY(itr.key(), index, time)));
    }

    m_activeCursor->setText(text);
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
    updateMinMaxPos();
}

void ChartView::setChartFull(ChartType type)
{
    if(type == ANALOG_TYPE)
    {
        m_analogAxisY->setRange(0, m_analogAxisY->max());
        updateMinMaxPos();
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
            onShowChanged(itr.key(), show);
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

void ChartView::addVariable(ChartType type, const QString &id, const QString &name, unsigned color)
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

        // id name map
        m_id_name_map[id] = name;

        // set series width and color
        QPen pen;
        pen.setWidth(2);
        pen.setColor(QColor(color));
        series->setPen(pen);

        // new min max
        // max
        QGraphicsEllipseItem *max_point = scene()->addEllipse(-7.5, -7.5, 15, 15, pen, QColor(color));
        // min
        QPolygonF Triangle;
        Triangle.append(QPointF(-7.5, 4.33));
        Triangle.append(QPointF(0, -8.66));
        Triangle.append(QPointF(7.5, 4.33));
        Triangle.append(QPointF(-7.5, 4.33));
        QGraphicsPolygonItem *min_point = scene()->addPolygon(Triangle, pen, QColor(color));

        // set max_point very high y value
        max_point->setPos(0, 1000000);
        // set min_point very low y value
        min_point->setPos(0, -1000000);

        if(!m_showMinMaxOnCreate)
        {
            max_point->hide();
            min_point->hide();
        }

        // add to map
        m_minMaxMap[id] = std::make_tuple(min_point, max_point, 0, 0);
        m_showMinMax[id] = m_showMinMaxOnCreate;
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

        // id name map
        m_id_name_map[id] = name;

        QPen pen;
        pen.setWidth(2);
        pen.setColor(QColor(color));
        series->setPen(pen);
    }
    else
    {
        qDebug() << "addVariable -> unsupport type " << type;
    }


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

            // id name map
            m_id_name_map.remove(id);

            // min max map
            if(m_minMaxMap.contains(id))
            {
                auto min_max = m_minMaxMap[id];
                delete std::get<0>(min_max);
                delete std::get<1>(min_max);
                m_minMaxMap.remove(id);
            }
            else
            {
                qDebug() << "m_minMaxMap not contains " << id;
            }
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

            // id name map
            m_id_name_map.remove(id);
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
        showMinMax(id, show);
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
        if(m_minMaxMap.contains(id))
        {
            QPen pen;
            pen.setWidth(2);
            pen.setColor(QColor(color));
            // change min triangle color
            std::get<0>(m_minMaxMap[id])->setPen(pen);
            std::get<0>(m_minMaxMap[id])->setBrush(QColor(color));
            // change max circle color
            std::get<1>(m_minMaxMap[id])->setPen(pen);
            std::get<1>(m_minMaxMap[id])->setBrush(QColor(color));
        }
        else
        {
            qDebug() << "minMaxMap not contain " << id;
        }
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
    updateCursorsPos();
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

        // update min max value
        changeMinMaxPos(id);

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
//        qDebug() << "unregistered id " << id;
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
            updateCursorsPos();
            updateMinMaxPos();
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

void ChartView::showActiveCursor(bool show)
{
    if(show)
    {
        m_activeCursor->show();
    }
    else
    {
        m_activeCursor->hide();
    }
}

void ChartView::showCursors(bool show)
{
    if(show)
    {
        for(auto itr = m_cursorMap.begin(); itr != m_cursorMap.end(); ++itr)
        {
            itr.value()->show();
        }
    }
    else
    {
        for(auto itr = m_cursorMap.begin(); itr != m_cursorMap.end(); ++itr)
        {
            itr.value()->hide();
        }
    }
}

int ChartView::getIndex(const QString id, quint64 time) const
{
    // retrieve series;
    QLineSeries *series;
    if(m_analogSeriesMap.contains(id))
        series = m_analogSeriesMap[id];
    else if(m_digitalSeriesMap.contains(id))
        series = m_digitalSeriesMap[id];
    else
    {
        qDebug() << "getIndex -> no registered id";
        return 0;
    }

    // calculate approximately position
    if(series->count() == 0) return -1;
    int left = 0;
    int right = series->count() - 1;


    quint64 min_time = series->at(left).x();
    quint64 max_time = series->at(right).x();

    if(time <= min_time) return 0;
    if(time >= max_time) return series->count() - 1;

    while(true)
    {
        int mark = (left + right) / 2;
        if(series->at(mark).x() > time)
        {
            right = mark;
        }else if(series->at(mark).x() < time)
        {
            left = mark;
        }else
        {
            return mark;
        }
        if(right - left <= 1)
        {
            return left;
        }
    }
}

qreal ChartView::interpolateY(const QString id, int index, quint64 time) const
{
    // retrieve series;
    QLineSeries *series;
    if(m_analogSeriesMap.contains(id))
        series = m_analogSeriesMap[id];
    else if(m_digitalSeriesMap.contains(id))
        series = m_digitalSeriesMap[id];
    else
    {
        qDebug() << "getIndex -> no registered id";
        return 0;
    }

    if(index >= series->count() - 1) return series->at(series->count() - 1).y();
    if(index < 0) return 0;

    quint64 start_time = series->at(index).x();
    quint64 end_time = series->at(index + 1).x();

    qreal start_val = series->at(index).y();
    qreal end_val = series->at(index + 1).y();

    if(time < start_time) return 0;

    // discard presicion
    if(end_time - start_time < 0.01) return start_val;
    return start_val + (time - start_time) * (end_val - start_val) / (end_time - start_time);
}

void ChartView::popCursor()
{
    if(m_cursors.empty()) return;
    CursorItem *item = m_cursors.back();
    m_cursors.pop_back();


    quint64 time = m_cursorMap.key(item);
    if(m_cursorMap.contains(time))
    {
        m_cursorMap.remove(time);
    }
    else
    {
        qDebug() << "m_cursorMap not contain time " << time;
    }

    delete item;
}

void ChartView::updateCursorsPos()
{
    for(auto itr = m_cursorMap.begin(); itr != m_cursorMap.end(); ++itr)
    {
        itr.value()->setPos(m_chart->mapToPosition(QPointF(itr.key(), 0)).x(), 0);
    }
}

void ChartView::updateMinMaxPos()
{
    for(auto itr = m_minMaxMap.begin(); itr != m_minMaxMap.end(); ++itr)
    {
        if(m_analogSeriesMap.contains(itr.key()))
        {
            // if hide not update
            if(!m_showMinMax[itr.key()]) continue;

            auto &min_max_point = itr.value();
            int min_index = getIndex(itr.key(), std::get<2>(min_max_point));
            int max_index = getIndex(itr.key(), std::get<3>(min_max_point));
            if(min_index < 0 || max_index < 0) continue;

            QPointF min_value = m_analogSeriesMap[itr.key()]->at(min_index);
            QPointF max_value = m_analogSeriesMap[itr.key()]->at(max_index);

            std::get<0>(min_max_point)->setPos(m_chart->mapToPosition(min_value));
            std::get<1>(min_max_point)->setPos(m_chart->mapToPosition(max_value));

        }
        else
        {
            qDebug() << "updateMinMaxPos : can not find " << itr.key() << " in analogMap";
        }
    }
}

void ChartView::changeMinMaxPos(const QString &id)
{
    if(m_analogSeriesMap.contains(id))
    {
        QLineSeries *series = m_analogSeriesMap[id];
        if(series->count() < 1)
        {
            qDebug() << "series count < 1";
            return;
        }

        QPointF lastValue = series->at(series->count() - 1);

        auto &min_max_point = m_minMaxMap[id];

        int min_index = getIndex(id, std::get<2>(min_max_point));
        int max_index = getIndex(id, std::get<3>(min_max_point));

        bool change = false;
        // min y
        if(series->at(min_index).y() > lastValue.y())
        {
            std::get<2>(min_max_point) = lastValue.x();
            change = true;
        }
        // max y
        if(series->at(max_index).y() < lastValue.y())
        {
            std::get<3>(min_max_point) = lastValue.x();
            change = true;
        }

        if(change)
            updateMinMaxPos();
    }
    else
    {
        qDebug() << "updateMinMaxPos : can not find " << id << " in analogMap";
    }
}

void ChartView::showMinMax(const QString &id, bool show)
{
    if(m_minMaxMap.contains(id))
    {
        m_showMinMax[id] = show;
        if(show)
        {
            std::get<0>(m_minMaxMap[id])->show();
            std::get<1>(m_minMaxMap[id])->show();
        }else
        {
            std::get<0>(m_minMaxMap[id])->hide();
            std::get<1>(m_minMaxMap[id])->hide();
        }

    }else
    {
        qDebug() << "showMinMax not contains" << id;
    }
}

void ChartView::showMinMax(bool show)
{
    m_showMinMaxOnCreate = show;
    for(auto itr = m_minMaxMap.begin(); itr != m_minMaxMap.end(); ++itr)
    {
        showMinMax(itr.key(), show);
    }
}
