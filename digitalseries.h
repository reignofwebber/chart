#ifndef DIGITALSERIES_H
#define DIGITALSERIES_H

#include <QtCharts>

class DigitalSeries : public QLineSeries
{
public:
    DigitalSeries();

    int count() const;
};

#endif // DIGITALSERIES_H
