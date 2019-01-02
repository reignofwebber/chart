#include "chartplot.h"
#include "ui_chartpanel.h"
#include "testthread.h"
#include "chartdelegate.h"
#include "chartmodel.h"
#include "cursoritem.h"

#include <QThread>
#include <QtCharts>
#include <QStandardItemModel>

#include "chartview.h"


ChartPlot::ChartPlot(QWidget *parent)
    :QWidget(parent)
    , ui(new Ui::ChartPanel)
{
    ui->setupUi(this);


    //////////////////////////////////////////////////////////////////////////////
    //// tableview part
    // analog part
    m_model = new ChartModel;
    ui->m_analogPanelView->setModel(m_model);
    toggleColumnHide(ui->m_analogPanelView, false);
    ui->m_analogPanelView->setItemDelegateForColumn(COL_SHOW, new ChartCheckBoxDelegate);
    ui->m_analogPanelView->setItemDelegateForColumn(COL_STAR, new ChartCheckBoxDelegate);
    ui->m_analogPanelView->setItemDelegateForColumn(COL_COLOR, new ChartButtonDelegate);

    connect(m_model, SIGNAL(showChanged(QString,bool)), ui->m_view, SLOT(onShowChanged(QString,bool)));
    connect(m_model, SIGNAL(colorChanged(QString,uint)), ui->m_view, SLOT(onColorChanged(QString,uint)));
    connect(ui->m_view, SIGNAL(setValue(QString,qreal)), m_model, SLOT(setValue(QString,qreal)));

    // selection
    connect(ui->m_analogPanelView->selectionModel(), &QItemSelectionModel::selectionChanged, [=](const QItemSelection &selected, const QItemSelection &deselected)
    {
        for(const QModelIndex &index : selected.indexes())
        {
            QString id = m_model->data(index, Qt::UserRole).toString();
            ui->m_view->setChartBold(ChartView::ANALOG_TYPE, id, true);
        }
        for(const QModelIndex &index : deselected.indexes())
        {
            QString id = m_model->data(index, Qt::UserRole).toString();
            ui->m_view->setChartBold(ChartView::DIGITAL_TYPE, id, false);
        }
    });

    // btns
    connect(ui->m_enableAnalogBtn, &QPushButton::clicked, [=](bool checked)
    {
        if(checked)
        {
            // restore maxY
            if(ui->m_enableDigitalBtn->isChecked())
                ui->m_view->setChartHalf();
            else
                ui->m_view->setChartFull(ChartView::ANALOG_TYPE);

            ui->m_view->setChartShow(ChartView::ANALOG_TYPE, true);
        }
        else
        {
            if(ui->m_enableDigitalBtn->isChecked())
                ui->m_view->setChartFull(ChartView::DIGITAL_TYPE);

            ui->m_view->setChartShow(ChartView::ANALOG_TYPE, false);
        }
    });
    connect(ui->m_editBtn, SIGNAL(clicked(bool)), this, SLOT(toggleColumnHide(bool)));
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
            ui->m_view->removeVariable(ChartView::ANALOG_TYPE, id);
            m_model->removeRow(list.at(i).row(), QModelIndex());
        }
    });

    // digital part
    m_model_d = new ChartModel;
    ui->m_digitalPanelView->setModel(m_model_d);
    toggleColumnHide(ui->m_digitalPanelView, false);

    ui->m_digitalPanelView->setItemDelegateForColumn(COL_SHOW, new ChartCheckBoxDelegate);
    ui->m_digitalPanelView->setItemDelegateForColumn(COL_STAR, new ChartCheckBoxDelegate);
    ui->m_digitalPanelView->setItemDelegateForColumn(COL_COLOR, new ChartButtonDelegate);

    connect(m_model_d, SIGNAL(showChanged(QString,bool)), ui->m_view, SLOT(onShowChanged(QString,bool)));
    connect(m_model_d, SIGNAL(colorChanged(QString,uint)), ui->m_view, SLOT(onColorChanged(QString,uint)));
    connect(ui->m_view, SIGNAL(setDigitalValue(QString,qreal)), m_model_d, SLOT(setValue(QString,qreal)));

    // selection
    connect(ui->m_digitalPanelView->selectionModel(), &QItemSelectionModel::selectionChanged, [=](const QItemSelection &selected, const QItemSelection &deselected)
    {
        for(const QModelIndex &index : selected.indexes())
        {
            QString id = m_model_d->data(index, Qt::UserRole).toString();
            ui->m_view->setChartBold(ChartView::DIGITAL_TYPE, id, true);
        }
        for(const QModelIndex &index : deselected.indexes())
        {
            QString id = m_model_d->data(index, Qt::UserRole).toString();
            ui->m_view->setChartBold(ChartView::DIGITAL_TYPE, id, false);
        }
    });

    //btns
    connect(ui->m_enableDigitalBtn, &QPushButton::clicked, [=](bool checked)
    {
        if(checked)
        {
            if(ui->m_enableAnalogBtn->isChecked())
                ui->m_view->setChartHalf();
            else
                ui->m_view->setChartFull(ChartView::DIGITAL_TYPE);

            ui->m_view->setChartShow(ChartView::DIGITAL_TYPE, true);
        }
        else
        {
            if(ui->m_enableAnalogBtn->isChecked())
                ui->m_view->setChartFull(ChartView::ANALOG_TYPE);

            ui->m_view->setChartShow(ChartView::DIGITAL_TYPE, false);
        }
    });
    connect(ui->m_editBtn_d, SIGNAL(clicked(bool)), this, SLOT(toggleColumnHide(bool)));
    connect(ui->m_addBtn_d, SIGNAL(clicked()), this, SLOT(addDigitalVariate()));
    connect(ui->m_removeBtn_d, &QPushButton::clicked, [=](){
        QItemSelectionModel *selModel = ui->m_digitalPanelView->selectionModel();

        QModelIndexList list = selModel->selectedIndexes();
        std::sort(list.begin(), list.end(), [](const QModelIndex &l, const QModelIndex &r)
        {
            return l.row() < r.row();
        });

        for(int i = list.size() - 1; i >=0; --i)
        {
            QString id = m_model_d->data(list.at(i), Qt::UserRole).toString();
            ui->m_view->removeVariable(ChartView::DIGITAL_TYPE, id);
            m_model_d->removeRow(list.at(i).row(), QModelIndex());
        }
    });
    connect(ui->m_playBtn, &QPushButton::clicked, [=](bool checked)
    {
        if(checked)
        {
            QIcon icon;
            icon.addFile(QStringLiteral(":/images/play.png"), QSize(), QIcon::Normal, QIcon::Off);
            ui->m_playBtn->setIcon(icon);
            ui->m_playBtn->setToolTip("播放");
        }
        else
        {
            QIcon icon;
            icon.addFile(QStringLiteral(":/images/pause.png"), QSize(), QIcon::Normal, QIcon::Off);
            ui->m_playBtn->setIcon(icon);
            ui->m_playBtn->setToolTip("暂停");
        }
    });


    //////////////////////////////////////////////////////////////////////////////
    // panel part
    ui->m_dataTypeCombo->setItemDelegate(new QStyledItemDelegate);
    ui->m_dataFileCombo->setItemDelegate(new QStyledItemDelegate);

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
    connect(ui->m_resetBtn, SIGNAL(clicked(bool)), ui->m_view, SLOT(resetAxisX()));

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
    // 区分数字量和模拟量
    if(data.length == 1)
    {
        // add to chart
        ui->m_view->addVariable(ChartView::DIGITAL_TYPE, data.id, data.color);
        // add to panel
        m_model_d->addVariate(data);
    }
    else
    {
        // add to chart
        ui->m_view->addVariable(ChartView::ANALOG_TYPE, data.id, data.color);
        // add to panel
        m_model->addVariate(data);
    }
}

void ChartPlot::addPoint(QString id, qreal time, qreal val)
{
    ui->m_view->addPoint(id, time, val);
}


// 只要有一个超界， 就滚动chart
void ChartPlot::addPointComplete()
{
    if(ui->m_playBtn->isChecked()) return;
    ui->m_view->addPointComplete();
}

void ChartPlot::toggleColumnHide(ChartPanelView *view, bool checked)
{
    if(checked)
    {
       view->setColsWidthRatio(QVector<int>() << 1 << 4 << 3 << 1 << 1);
    }
    else
    {
       view->setColsWidthRatio(QVector<int>() << 0 << 5 << 3 << 1 << 0);
    }
}

void ChartPlot::toggleColumnHide(bool checked)
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if(ui->m_editBtn == btn)
    {
        toggleColumnHide(ui->m_analogPanelView, checked);
    }
    else if(ui->m_editBtn_d)
    {
        toggleColumnHide(ui->m_digitalPanelView, checked);
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

void ChartPlot::addDigitalVariate()
{
    static int id = 100;
    VariateData data;
    data.id = QString::number(id);
    data.variateName = QString("信号%1").arg(id);
    data.length = 1;

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



unsigned ChartPlot::getRandomColor() const
{
    unsigned data = 0;
    data += (std::rand() % 256) << 16;
    data += (std::rand() % 256) << 8;
    data += (std::rand() % 256);
    return data;
}


