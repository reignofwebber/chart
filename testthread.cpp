#include "testthread.h"
#include "chartplot.h"
#include <chrono>

#include <thread>
#include <QDateTime>

#include <QDebug>

Test::Test(ChartPlot *plot)
{
    this->plot = plot;
}

Test::Test()
{

}

Test::~Test()
{

}

void Test::process()
{
    qDebug() << "Test start";
    VariateData data;
    data.id = "1";
    data.length = 8;
    plot->addVariable(data);
    data.id = "2";
    data.length = 1;
    plot->addVariable(data);
    data.id = "3";
    data.length = 1;
    plot->addVariable(data);

    while(true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        plot->addPoint("1", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 100);
        plot->addPoint("2", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 2);
        plot->addPoint("3", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 2);
    }
    emit finished();
}
