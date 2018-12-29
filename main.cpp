#include "mainwindow.h"
#include "chartplot.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
    ChartPlot form;
    form.show();
    return a.exec();
}
