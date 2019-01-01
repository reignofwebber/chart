#ifndef CHARTSETTINGSDLS_H
#define CHARTSETTINGSDLS_H

#include <QDialog>

namespace Ui {
class ChartSettingsDlg;
}

class ChartSettingsDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ChartSettingsDlg(QWidget *parent = 0);
    ~ChartSettingsDlg();

private:
    Ui::ChartSettingsDlg *ui;
};

#endif // CHARTSETTINGSDLS_H
