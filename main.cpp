
#include "chartplot.h"
#include "testform.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    a.setStyleSheet("");
            qApp->setStyleSheet("QCheckBox::indicator{\n	width:15px;\n	height:15px;\n}\n\nQCheckBox::indicator::unchecked{\n	image:url(:/images/unchecked_checkbox.png)\n}\n\nQCheckBox::indicator::checked{\n	image:url(:/images/checked_checkbox.png)\n}");

//    MainWindow w;
//    w.show();
    ChartPlot form;
    form.show();
//    TestForm form;
//    form.show();
    return a.exec();
}
