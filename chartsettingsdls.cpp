#include "chartsettingsdls.h"
#include "ui_chartsettingsdlg.h"

ChartSettingsDlg::ChartSettingsDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChartSettingsDlg)
{
    ui->setupUi(this);

    QDateTime now = QDateTime::currentDateTime();
    ui->m_startTime->setDateTime(now);
    ui->m_endTime->setDateTime(now);
}

ChartSettingsDlg::~ChartSettingsDlg()
{
    delete ui;
}
