#-------------------------------------------------
#
# Project created by QtCreator 2018-12-28T18:30:05
#
#-------------------------------------------------

QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_chart
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    testthread.cpp \
    chartplot.cpp \
    testform.cpp \
    chartmodel.cpp \
    chartpanelview.cpp \
    chartdelegate.cpp \
    chartsettingsdls.cpp \
    cursoritem.cpp \
    chartview.cpp \
    configuredialog.cpp \
    dataconfigure.cpp

HEADERS += \
    testthread.h \
    chartplot.h \
    testform.h \
    chartmodel.h \
    chartpanelview.h \
    chartdelegate.h \
    chartview.h \
    chartsettingsdls.h \
    cursoritem.h \
    analyze_types.h \
    configuredialog.h \
    dataconfigure.h

FORMS += \
        mainwindow.ui \
    chartpanel.ui \
    testform.ui \
    chartsettingsdlg.ui \
    configuredialog.ui

RESOURCES += \
    chart.qrc
