#include "testthread.h"
#include <chrono>

#include <thread>
#include <QDateTime>

Test::Test(ChartPanel *panel)
{
    this->panel = panel;
}

void Test::process()
{
    qDebug() << "Test start";
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        panel->addPoint(QDateTime::currentDateTime().toMSecsSinceEpoch(), std::rand() %100);
    }
    emit finished();
}
