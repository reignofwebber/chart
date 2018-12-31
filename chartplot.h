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
    // 所有变量加点完成
    void addPointComplete();
    // 获取所有变量id
    QVector<QString> getAllVariableIds();

public slots:
    void toggleColumnHide(bool checked);

private:
    Ui::ChartPanel *ui;

    QChart *m_chart;
    QValueAxis *m_analogAxisY;
    QValueAxis *m_digitalAxisY;

    QVector<QLineSeries *> m_analogSeriesPool;
    QVector<QLineSeries *> m_digitalSeriesPool;

    QMap<QString, QLineSeries *> m_analogSeriesMap;
    QMap<QString, QLineSeries *> m_digitalSeriesMap;
    QMap<QString, qreal> m_digitalOffsetMap;

    // analog
    // 模拟量y轴最小值
    qreal m_analogMinY = -1;
    // 模拟量y轴最大值
    qreal m_analogMaxY = 1;
    // 数字量y轴最小值
    qreal m_digitalMinY = 0;
    // 数字量y轴最大值
    qreal m_digitalMaxY = 12;

    // 波形图总秒数
    int m_totoalSeconds = 60;
    // 数字量间距
    qreal digitalOffset = 1.2;
};

#endif // CHARTPLOT_H
