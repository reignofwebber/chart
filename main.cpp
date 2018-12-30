
#include "chartplot.h"
#include "testform.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
    ChartPlot form;
    form.show();
//    TestForm form;
//    form.show();
    return a.exec();
}
