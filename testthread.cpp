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

    while(true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        plot->addPoint("0", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 100);
        plot->addPoint("1", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 100);
        plot->addPoint("2", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 100);
        plot->addPoint("3", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 100);
        plot->addPoint("4", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 200);
        plot->addPoint("100", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 2);
        plot->addPoint("101", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 2);
        plot->addPoint("102", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 2);
        plot->addPoint("103", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 2);
        plot->addPoint("104", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 2);
        plot->addPoint("105", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 2);
        plot->addPoint("106", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 2);
        plot->addPoint("107", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 2);
        plot->addPoint("108", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 2);
        plot->addPoint("109", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 2);
        plot->addPointComplete();
    }
    emit finished();
}
