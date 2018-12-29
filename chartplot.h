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
}

using namespace QtCharts;

class AnalogSeries;
class DigitalSeries;

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

class ChartPlot : public QWidget
{
    Q_OBJECT

public:
    explicit ChartPlot(QWidget *parent = 0);
    ~ChartPlot();

    // 添加变量
    void addVariable(const VariateData &data);
    // 为变量ID加点
    void addPoint(QString id, qreal time, qreal val);
    // 获取所有变量id
    QVector<QString> getAllVariableIds();

private:
    Ui::ChartPanel *ui;

    QChart *m_analogChart;
    QChart *m_digitalChart;

    QMap<QString, QLineSeries *> m_analogSeriesMap;
    QMap<QString, QLineSeries *> m_digitalSeriesMap;

    // analog
    // y轴最小值
    qreal m_minY;
    // y轴最大值
    qreal m_maxY;
    // 波形图总秒数
    int m_totoalSeconds;
};

#endif // CHARTPLOT_H
