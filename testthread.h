#ifndef TESTTHREAD_H
#define TESTTHREAD_H

#include <QString>
#include <QObject>

#include "chartpanel.h"

class Test : public QObject
{
    Q_OBJECT
public:
    Test(ChartPanel *panel);


public slots:
    void process();
signals:
    void finished();
    void error(QString err);

private:
    ChartPanel *panel;

};

#endif // TESTTHREAD_H
