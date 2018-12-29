#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "chartpanel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ChartPanel * panel = new ChartPanel;
//    panel->show();
    setCentralWidget(panel);
}

MainWindow::~MainWindow()
{
    delete ui;
}
