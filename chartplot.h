#ifndef CHARTPLOT_H
#define CHARTPLOT_H

#include <QWidget>
#include <QVector>
#include <QMap>

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

struct VariateData
{
    VariateData():package(-1){}
    QString id;
    int package;
    int wordOffset;
    int bitOffset;
    int length;
    float dimension;
    float ratio;
    // 只是对于温度
    float valueOffset;
    QString unite;
    QString variateName;
};

struct ChartData
{
    QString id;         // from VariateData
    QString name;       // from VariateData'
    int length;         // from VariateData
    float value = 0;    // value
    unsigned color;     // color
    bool show;          // if show in chart
    bool star;          // if save for next open
};

class ChartPanelView;

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
    void addVariate();
    void addDigitalVariate();
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

};

#endif // CHARTPLOT_H
