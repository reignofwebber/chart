#ifndef CHARTPANEL_H
#define CHARTPANEL_H

#include <QtCharts>
#include <QtWidgets>


class ChartPanel : public QWidget
{
    Q_OBJECT

public:
    ChartPanel(QWidget *parent = 0);

    void addPoint(qreal x, qreal y);

private:
    void updateYValue(qreal curMin, qreal curMax);
    void addValue();
    qreal getRealValue(qreal val);
    qreal getMappedValue(qreal min, qreal max, qreal val);


private:
    // 模拟量
    QLineSeries *m_series;
    // x轴（时间轴）
    QDateTimeAxis *m_axisX;
    // y轴
    QValueAxis *m_axisY;
    // 图表
    QChart *m_chart;

    // x轴刻度数量
    int m_tickCount;
    // 波形图总秒数
    int m_totoalSeconds;
    // y轴最小值
    qreal m_minY;
    // y轴最大值
    qreal m_maxY;
};

#endif // CHARTPANEL_H
