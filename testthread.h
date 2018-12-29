#ifndef TESTTHREAD_H
#define TESTTHREAD_H

#include <QString>
#include <QObject>

class ChartPlot;

class Test : public QObject
{
    Q_OBJECT
public:
    Test();
    Test(ChartPlot *panel);
    virtual ~Test();

public slots:
    void process();
signals:
    void finished();
    void error(QString err);

private:
    ChartPlot *plot;

};

#endif // TESTTHREAD_H
