#ifndef CHARTPLOT_H
#define CHARTPLOT_H

#include <QWidget>
#include <QVector>
#include <QMap>

#include "analyze_types.h"

namespace Ui {
class ChartPanel;
}

namespace QtCharts {
    class QLineSeries;
    class QChart;
    class QValueAxis;
}

using namespace QtCharts;

class AnalogSeries;
class DigitalSeries;
class ChartModel;
class QAbstractButton;


class ChartPanelView;
class ConfigureDialog;

class ChartPlot : public QWidget
{
    Q_OBJECT

public:
    explicit ChartPlot(QWidget *parent = 0);
    ~ChartPlot();

    // 添加变量
    void addVariable(const ChartData &data);


signals:
    void setValue(QString id, qreal value);
    void setDigitalValue(QString id, qreal value);

public slots:
    // 添加变量
    void openConfigureDlg();
    void addVariateData(const VariateData& data, bool add);
    void toggleColumnHide(bool checked);

    // group button for cursor
    void onCursorGroupToggled(QAbstractButton*,bool);

    // 为变量ID加点
    void addPoint(QString id, qreal time, qreal val);
    // 所有变量加点完成
    void addPointComplete();

private:

    //
    void toggleColumnHide(ChartPanelView *view, bool checked);
    void removeVariable(QString id);
    unsigned getRandomColor() const;

private:
    Ui::ChartPanel *ui;

    ChartModel *m_model;        // analog model
    ChartModel *m_model_d;      // digital model

    ConfigureDialog *m_configDlg;
};

#endif // CHARTPLOT_H
