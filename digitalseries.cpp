#include "digitalseries.h"

DigitalSeries::DigitalSeries()
{

}


int DigitalSeries::count() const
{
    return 2 * QLineSeries::count() - 1;
}
