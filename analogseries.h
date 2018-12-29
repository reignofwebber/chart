#ifndef ANALOGSERIES_H
#define ANALOGSERIES_H

#include <QtCharts>


class AnalogSeries : public QLineSeries
{
    Q_OBJECT
public:
    AnalogSeries();

    void append(qreal x, qreal y);

};

#endif // ANALOGSERIES_H
