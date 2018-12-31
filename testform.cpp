#include "testform.h"
#include "ui_testform.h"

#include <QStandardItemModel>

#include <QDebug>

TestForm::TestForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestForm)
{
    ui->setupUi(this);
    ui->tableView->setModel(new QStandardItemModel(3,4));
//    ui->tableView->setAlternatingRowColors(true);

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(Del()));
}

TestForm::~TestForm()
{
    delete ui;
}

void TestForm::Del()
{
    qDebug() << ui->tableView->selectionModel()->selectedIndexes();
}
