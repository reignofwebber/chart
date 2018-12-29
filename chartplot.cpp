#include "chartplot.h"
#include "ui_chartpanel.h"
#include "testthread.h"

#include "analogseries.h"
#include "digitalseries.h"

#include <QThread>
#include <QtCharts>


ChartPlot::ChartPlot(QWidget *parent)
    :QWidget(parent)
    , ui(new Ui::ChartPanel)
    , m_minY(0), m_maxY(1), m_totoalSeconds(60 * 5)
{
    ui->setupUi(this);

    // analog
    m_analogChart = new QChart;
    ui->m_analogView->setChart(m_analogChart);
    m_analogChart->legend()->hide();


    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setFormat("yy-MM-dd h:mm:ss");
    auto now = QDateTime::currentDateTime();
    axisX->setMin(now);
    axisX->setMax(now.addSecs(60));
    axisX->setTickCount(5);
    axisX->hide();
    m_analogChart->addAxis(axisX, Qt::AlignBottom);

    QValueAxis *axisY = new QValueAxis;
    axisY->setMin(m_minY);
    axisY->setMax(m_maxY);
    axisY->setLabelsVisible(false);
    axisY->setTickCount(2);
    m_analogChart->addAxis(axisY, Qt::AlignLeft);


    m_analogChart->layout()->setContentsMargins(0, 0, 0, 0);
    m_analogChart->setBackgroundRoundness(0);
    m_analogChart->setMargins(QMargins(0, 0, 0, -1));
    ui->m_analogView->setRenderHint(QPainter::Antialiasing);

    // digital
    m_digitalChart = new QChart;
    ui->m_digitalView->setChart(m_digitalChart);
    m_digitalChart->legend()->hide();

    axisX = new QDateTimeAxis;
    axisX->setFormat("yy-MM-dd h:mm:ss");
    axisX->setMin(now);
    axisX->setMax(now.addSecs(60));
    axisX->setTickCount(5);
    axisX->hide();
    m_digitalChart->addAxis(axisX, Qt::AlignBottom);

    axisY = new QValueAxis;
    axisY->setMin(0);
    axisY->setMax(6);
    axisY->setLabelsVisible(false);
    axisY->setTickCount(6);
    m_digitalChart->addAxis(axisY, Qt::AlignLeft);

    m_digitalChart->layout()->setContentsMargins(0, 0, 0, 0);
    m_digitalChart->setBackgroundRoundness(0);
    m_digitalChart->setMargins(QMargins(0, -1, 0, 0));
    ui->m_digitalView->setRenderHint(QPainter::Antialiasing);



    //////////////////////////////////////////////////////////////////////////////
    QThread *thread = new QThread;
    Test *test = new Test(this);
    test->moveToThread(thread);
    connect(thread, SIGNAL(started()), test, SLOT(process()));
    connect(test, SIGNAL(finished()), thread, SLOT(quit()));
    connect(test, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
    //////////////////////////////////////////////////////////////////////////////

}

ChartPlot::~ChartPlot()
{
    delete ui;
}

void ChartPlot::addVariable(const VariateData &data)
{
    QLineSeries *series = new QLineSeries;
    // 区分数字量和模拟量
    if(data.length == 1)
    {
        m_digitalSeriesMap[data.id] = series;
        m_digitalChart->addSeries(series);
        series->attachAxis(m_digitalChart->axisX());
        series->attachAxis(m_digitalChart->axisY());
    }
    else
    {
        m_analogSeriesMap[data.id] = series;
        m_analogChart->addSeries(series);
        series->attachAxis(m_analogChart->axisX());
        series->attachAxis(m_analogChart->axisY());
    }

}

void ChartPlot::addPoint(QString id, qreal time, qreal val)
{
    if(m_analogSeriesMap.contains(id))
    {
        if(val < m_minY)
        {
            m_minY = val;
            m_analogChart->axisY()->setMin(m_minY);

        }
        else if(val > m_maxY)
        {
            m_maxY = val;
            m_analogChart->axisY()->setMax(m_maxY + 10);
        }

        if(m_analogSeriesMap[id]->count() > m_totoalSeconds)
        {
            qreal pertick = m_analogChart->plotArea().width()/m_totoalSeconds;
            m_analogChart->scroll(pertick, 0);
        }

        m_analogSeriesMap[id]->append(time, val);
        m_analogChart->update();
    }
    else if(m_digitalSeriesMap.contains(id))
    {
        if(m_digitalSeriesMap[id]->count() > 0)
        {
            qDebug() << m_digitalSeriesMap[id]->count();
            m_digitalSeriesMap[id]->append(time, m_digitalSeriesMap[id]->at(m_digitalSeriesMap[id]->count() - 1).y());
            m_digitalSeriesMap[id]->append(time, val);
        }
        else
        {
            m_digitalSeriesMap[id]->append(time, val);
        }

        if(m_digitalSeriesMap[id]->count() * 2 - 1 > m_totoalSeconds)
        {
            qreal pertick = m_digitalChart->plotArea().width()/m_totoalSeconds;
            m_digitalChart->scroll(pertick, 0);
        }

        m_digitalChart->update();
    }
    else
    {
        qDebug() << "unregistered id " << id;
    }
}

QVector<QString> ChartPlot::getAllVariableIds()
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
