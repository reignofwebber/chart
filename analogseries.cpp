#include "analogseries.h"

AnalogSeries::AnalogSeries()
{

}

void AnalogSeries::append(qreal x, qreal y)
{
    QLineSeries::append(x, y / 2);
}
