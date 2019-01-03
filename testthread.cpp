#include "testthread.h"
#include "chartplot.h"
#include <chrono>

#include <thread>
#include <QDateTime>

#include <QDebug>

Test::Test(ChartPlot *plot)
{
    this->plot = plot;
    connect(this, SIGNAL(addPoint(QString,qreal,qreal)), plot, SLOT(addPoint(QString,qreal,qreal)));
    connect(this, SIGNAL(addPointComplete()), plot, SLOT(addPointComplete()));
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

        emit addPoint("0", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 100);
        emit addPoint("1", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 100);
        emit addPoint("2", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 100);
        emit addPoint("3", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 100);
        emit addPoint("4", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 200);
        emit addPoint("100", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 2);
        emit addPoint("101", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 2);
        emit addPoint("102", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 2);
        emit addPoint("103", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 2);
        emit addPoint("104", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 2);
        emit addPoint("105", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 2);
        emit addPoint("106", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 2);
        emit addPoint("107", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 2);
        emit addPoint("108", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 2);
        emit addPoint("109", QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() % 2);
        emit addPointComplete();
    }
    emit finished();
}
