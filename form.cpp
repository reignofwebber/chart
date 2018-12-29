#include "form.h"
#include "ui_form.h"

#include <QtCharts>

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    QChart *chart = new QChart;
    QLineSeries *series = new QLineSeries;
    series->append(2, 5);
    series->append(5, 4);
    series->append(7, 8);
    chart->addSeries(series);
    chart->createDefaultAxes();
    ui->m_analogView->setChart(chart);
    chart->legend()->hide();


}

Form::~Form()
{
    delete ui;
}
