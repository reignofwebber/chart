#include "chartpanel.h"

#include <QThread>
#include "testthread.h"

ChartPanel1::ChartPanel1(QWidget *parent)
    : QWidget(parent),m_tickCount(5), m_totoalSeconds(60 * 5), m_minY(0), m_maxY(1)
{
    QVBoxLayout *layout = new QVBoxLayout;
    m_series = new QLineSeries();
//![1]
//    for(int i = 0; i < 60; ++i)
//    {
//        m_series->append(QDateTime::currentMSecsSinceEpoch() + 1000 * i, i+10);
//    }

//![2]

    m_chart = new QChart;
//    chart->legend()->hide();
    m_chart->addSeries(m_series);
    m_chart->setTitle("Simple line chart example");
    m_axisX = new QDateTimeAxis;
    m_axisX->setFormat("yy-MM-dd h:mm:ss");
    auto now = QDateTime::currentDateTime();
    m_axisX->setMin(now);
    m_axisX->setMax(now.addSecs(m_totoalSeconds));
    m_axisX->setTickCount(m_tickCount);
    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_series->attachAxis(m_axisX);
    m_axisX->setVisible(false);

    m_axisY = new QValueAxis;
    m_axisY->setMin(m_minY);
    m_axisY->setMax(m_maxY);
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
    m_series->attachAxis(m_axisY);
    m_axisY->setLabelsVisible(false);
    m_axisY->setTickCount(3);

    QChartView *view = new QChartView(m_chart);
    view->setRenderHint(QPainter::Antialiasing);
    view->setContentsMargins(0, 0, 0, 0);
    view->setFrameShape(QFrame::NoFrame);
    view->setLineWidth(0);

    m_chart->setMargins(QMargins(0, 0, 0, 0));


    layout->addWidget(view);

    ////////////////////////////////////////////////////
    QLineSeries *series = new QLineSeries();
    for(int i = 0; i < 60; ++i)
    {
        series->append(QDateTime::currentMSecsSinceEpoch() + 1000 * i, i+10);
    }
    QChart *chart = new QChart;
//    chart->legend()->hide();
    chart->addSeries(series);
    chart->setTitle("Simple line chart example");
    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setFormat("yy-MM-dd h:mm:ss");
    now = QDateTime::currentDateTime();
    axisX->setMin(now);
    axisX->setMax(now.addSecs(60));
    axisX->setTickCount(3);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    axisX->setVisible(false);

    QValueAxis *axisY = new QValueAxis;
    axisY->setMin(m_minY);
    axisY->setMax(m_maxY);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    axisY->setLabelsVisible(false);
    axisY->setTickCount(3);

    view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);
    view->setContentsMargins(0, 0, 0, 0);
    view->setFrameShape(QFrame::NoFrame);
    view->setLineWidth(0);

    chart->setMargins(QMargins(0, 0, 0, 0));


    layout->addWidget(view);
    layout->setSpacing(0);


    setLayout(layout);
    //////////////////////////////////////////////////////////////////////////////
//    QThread *thread = new QThread;
//    Test *test = new Test(this);
//    test->moveToThread(thread);
//    connect(thread, SIGNAL(started()), test, SLOT(process()));
//    connect(test, SIGNAL(finished()), thread, SLOT(quit()));
//    connect(test, SIGNAL(finished()), thread, SLOT(deleteLater()));
//    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
//    thread->start();
    //////////////////////////////////////////////////////////////////////////////
}

void ChartPanel1::addPoint(qreal time, qreal val)
{

    if(val < m_minY)
    {
        m_minY = val;
        updateYValue(m_minY, m_axisY->max());
        m_axisY->setMin(m_minY);

    }
    else if(val > m_maxY)
    {
        m_maxY = val;
        updateYValue(m_axisY->min(), m_maxY);
        m_axisY->setMax(m_maxY + 10);
    }

    if(m_series->count() > m_totoalSeconds)
    {
        qreal pertick = m_chart->plotArea().width()/m_totoalSeconds;
        m_chart->scroll(pertick, 0);
    }

    if(m_series->count() > 0)
    {
        m_series->append(time, m_series->at(m_series->count() - 1).y());
        m_series->append(time, val);
    }
    else
    {
        m_series->append(time, val);
    }

//    m_mapedSeries->append(time, getMappedValue(m_axisY->min(), m_axisY->max(), val));

    m_chart->update();
}


void ChartPanel1::updateYValue(qreal curMin, qreal curMax)
{

}

qreal ChartPanel1::getRealValue(qreal val)
{
    qreal center = (m_axisY->max() - m_axisY->min()) / 2;
    return (val - center) * 2 + m_axisY->min();
}

qreal ChartPanel1::getMappedValue(qreal min, qreal max, qreal val)
{
    qreal center = (max - min) / 2;
    return center + (val - min) / 2;
}
