#include "chartplot.h"
#include "ui_chartpanel.h"
#include "testthread.h"
#include "chartdelegate.h"
#include "chartmodel.h"

#include <QThread>
#include <QtCharts>
#include <QStandardItemModel>


ChartPlot::ChartPlot(QWidget *parent)
    :QWidget(parent)
    , ui(new Ui::ChartPanel)
{
    ui->setupUi(this);

    //////////////////////////////////////////////////////////////////////////////
    // chart part
    // analog
    m_chart = new QChart;
    ui->m_view->setChart(m_chart);
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
    ui->m_view->setRenderHint(QPainter::Antialiasing);


    // init pool
    for(int i = 0; i < 5; ++i)
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

    //////////////////////////////////////////////////////////////////////////////
    // tableview part
//    QStandardItemModel *model = new QStandardItemModel(5, 5);
//    ui->m_analogPanelView->setModel(model);
//    model->setHorizontalHeaderLabels(QStringList() << "" << "名称" << "值" << "颜色"  << "");
//    ui->m_analogPanelView->initColsWidthRatio(QVector<int>() << 1 << 4 << 3 << 1 << 1);
//    // delegates
//    ui->m_analogPanelView->setItemDelegateForColumn(0, new ChartCheckBoxDelegate);
    ui->m_analogPanelView->setModel(new ChartModel);
    ui->m_analogPanelView->initColsWidthRatio(QVector<int>() << 1 << 4 << 3 << 1 << 1);
    ui->m_analogPanelView->setItemDelegateForColumn(COL_SHOW, new ChartCheckBoxDelegate);
    ui->m_analogPanelView->setItemDelegateForColumn(COL_STAR, new ChartCheckBoxDelegate);

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
    // 区分数字量和模拟量
    if(data.length == 1)
    {
        if(m_digitalSeriesPool.empty())
        {
            qDebug() << "m_digitalSeriesPool is empty";
            return;
        }
        QLineSeries *series = m_digitalSeriesPool.back();
        m_digitalSeriesPool.pop_back();
        m_digitalSeriesMap[data.id] = series;
        m_digitalOffsetMap[data.id] = (4 - m_digitalSeriesPool.size()) * digitalOffset;
    }
    else
    {
        if(m_analogSeriesPool.empty())
        {
            qDebug() << "m_digitalSeriesPool is empty";
            return;
        }
        QLineSeries *series = m_analogSeriesPool.back();
        m_analogSeriesPool.pop_back();
        m_analogSeriesMap[data.id] = series;
    }

}

void ChartPlot::addPoint(QString id, qreal time, qreal val)
{
    if(m_analogSeriesMap.contains(id))
    {
        if(val < m_analogAxisY->min() || val > m_analogAxisY->max())
        {
            qreal positive = std::abs(val);
            m_analogAxisY->setRange(-positive, positive);
        }

        m_analogSeriesMap[id]->append(time, val);

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

    }
    else
    {
        qDebug() << "unregistered id " << id;
        return;
    }

    // 更新chart
    m_chart->update();
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

// 只要有一个超界， 就滚动chart
void ChartPlot::addPointComplete()
{
    for(auto itr = m_analogSeriesMap.begin(); itr != m_analogSeriesMap.end(); ++itr)
    {
        if(itr.value()->count() > m_totoalSeconds)
        {
            qreal pertick = m_chart->plotArea().width()/m_totoalSeconds;
            m_chart->scroll(pertick, 0);
            break;
        }
    }
}
