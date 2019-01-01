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
    m_analogAxisY->setTickCount(2);
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
    // panel part
    ui->m_dataTypeCombo->setItemDelegate(new QStyledItemDelegate);
    ui->m_dataFileCombo->setItemDelegate(new QStyledItemDelegate);


    //////////////////////////////////////////////////////////////////////////////
    // tableview part
    m_model = new ChartModel;
    ui->m_analogPanelView->setModel(m_model);
    toggleColumnHide(false);
    ui->m_analogPanelView->setItemDelegateForColumn(COL_SHOW, new ChartCheckBoxDelegate);
    ui->m_analogPanelView->setItemDelegateForColumn(COL_STAR, new ChartCheckBoxDelegate);
    ui->m_analogPanelView->setItemDelegateForColumn(COL_COLOR, new ChartButtonDelegate);

    connect(m_model, SIGNAL(showChanged(QString,bool)), this, SLOT(onShowChanged(QString,bool)));
    connect(m_model, SIGNAL(colorChanged(QString,uint)), this, SLOT(onColorChanged(QString,uint)));

    //////////////////////////////////////////////////////////////////////////////
    // connections
    connect(ui->m_editBtn, SIGNAL(clicked(bool)), this, SLOT(toggleColumnHide(bool)));
//    connect(ui->m_addBtn, SIGNAL(clicked()), model, SLOT(addVariate()));
    connect(ui->m_addBtn, SIGNAL(clicked()), this, SLOT(addVariate()));
    connect(ui->m_removeBtn, &QPushButton::clicked, [=](){
        QItemSelectionModel *selModel = ui->m_analogPanelView->selectionModel();

        QModelIndexList list = selModel->selectedIndexes();
        std::sort(list.begin(), list.end(), [](const QModelIndex &l, const QModelIndex &r)
        {
            return l.row() < r.row();
        });

        for(int i = list.size() - 1; i >=0; --i)
        {
            QString id = m_model->data(list.at(i), Qt::UserRole).toString();
            removeVariable(id);
            m_model->removeRow(list.at(i).row(), QModelIndex());
        }
    });

    // cursor type
    connect(ui->m_cursorNormalBtn, &QPushButton::clicked, [=](bool checked)
    {
        ui->m_cursorNormalBtn->setChecked(checked);
        if(checked)
        {
            emit ui->m_cursorHandBtn->clicked(false);
            ui->m_view->setRubberBand(QChartView::HorizontalRubberBand);
        }
        else
        {
            ui->m_view->setRubberBand(QChartView::NoRubberBand);
        }
    });
    // default m_cursorNormalBtn is enabled
    ui->m_view->setRubberBand(QChartView::HorizontalRubberBand);
    connect(ui->m_cursorHandBtn, &QPushButton::clicked, [=](bool checked)
    {
        ui->m_cursorHandBtn->setChecked(checked);
        if(checked)
        {
            emit ui->m_cursorNormalBtn->clicked(false);
            ui->m_view->setCursor(Qt::OpenHandCursor);
        }
        else
        {
            ui->m_view->setCursor(Qt::ArrowCursor);
        }
    });
    // reset btn clicked
    connect(ui->m_resetBtn, &QPushButton::clicked, [=]()
    {
        QDateTime now = QDateTime::currentDateTime();
        axisX->setRange(now, now.addSecs(m_totoalSeconds));
    });

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

void ChartPlot::addVariable(const ChartData &data)
{
    QLineSeries *series;
    // 区分数字量和模拟量
    if(data.length == 1)
    {
        if(m_digitalSeriesPool.empty())
        {
            qDebug() << "m_digitalSeriesPool is empty";
            return;
        }
        series = m_digitalSeriesPool.back();
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
        series = m_analogSeriesPool.back();
        m_analogSeriesPool.pop_back();
        m_analogSeriesMap[data.id] = series;
    }

    series->setColor(QColor(data.color));
    if(!data.show) series->hide();

    // add to panel
    m_model->addVariate(data);
}

void ChartPlot::addPoint(QString id, qreal time, qreal val)
{
    if(m_analogSeriesMap.contains(id))
    {
        if(val < m_analogAxisY->min() || val > m_analogAxisY->max())
        {
            qreal positive = std::abs(val) + 10;
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
        QLineSeries *series = itr.value();
        if(series->count() == 0) break;
        QDateTimeAxis *axisX = dynamic_cast<QDateTimeAxis *>(m_chart->axisX());

        if(series->at(series->count() - 1).x() > axisX->max().toMSecsSinceEpoch())
        {
            quint64 totalSeconds = axisX->max().toSecsSinceEpoch() - axisX->min().toSecsSinceEpoch();
            if(totalSeconds < 1) totalSeconds = 1;
            qreal pertick = m_chart->plotArea().width()/totalSeconds;
            m_chart->scroll(pertick, 0);
            break;
        }
    }
}

void ChartPlot::toggleColumnHide(bool checked)
{
    if(checked)
    {
       ui->m_analogPanelView->setColsWidthRatio(QVector<int>() << 1 << 4 << 3 << 1 << 1);
    }
    else
    {
       ui->m_analogPanelView->setColsWidthRatio(QVector<int>() << 0 << 5 << 3 << 1 << 0);
    }
}

void ChartPlot::addVariate()
{
    static int id = 0;
    VariateData data;
    data.id = QString::number(id);
    data.variateName = QString("列车速度%1").arg(id);
    data.length = 8;

    ChartData cdata;
    cdata.id = data.id;
    cdata.name = data.variateName;
    cdata.length = data.length;
    cdata.show = true;
    cdata.star = false;
    cdata.color = getRandomColor();
    addVariable(cdata);
    ++id;
}

void ChartPlot::onShowChanged(QString id, bool show)
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

void ChartPlot::onColorChanged(QString id, unsigned color)
{
    if(m_analogSeriesMap.contains(id))
    {
        m_analogSeriesMap[id]->setColor(QColor(color));
    }
    else if(m_digitalSeriesMap.contains(id))
    {
        m_analogSeriesMap[id]->setColor(QColor(color));
    }
    else
    {
        qDebug() << "colorChanged: no registered id" << id;
    }
}


unsigned ChartPlot::getRandomColor() const
{
    unsigned data = 0;
    data += (std::rand() % 256) << 16;
    data += (std::rand() % 256) << 8;
    data += (std::rand() % 256);
    return data;
}

void ChartPlot::removeVariable(QString id)
{
    QLineSeries *series;
    if(m_analogSeriesMap.contains(id))
    {
        series = m_analogSeriesMap[id];
        m_analogSeriesMap.remove(id);
        series->clear();
        m_analogSeriesPool.append(series);
    }
    else if(m_digitalSeriesMap.contains(id))
    {
        series = m_digitalSeriesMap[id];
        m_digitalSeriesMap.remove(id);
        series->clear();
        m_digitalSeriesPool.append(series);
    }
    else
    {
        qDebug() << "removeVariable: no registered id " << id;
    }
}


