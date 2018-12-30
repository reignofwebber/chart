#include "chartpanelview.h"

#include <QStandardItemModel>
#include <QHeaderView>

#include <QDebug>

ChartPanelView::ChartPanelView(QWidget *parent)
    : QTableView(parent)
{
    verticalHeader()->setVisible(false);
    horizontalHeader()->setStretchLastSection(true);

}

void ChartPanelView::initColsWidthRatio(const QVector<int> &radios)
{
    if(std::find(radios.begin(), radios.end(), 0) != radios.end())
    {
        qDebug() << "radio can not be 0";
        return;
    }

    m_radios.clear();
    total = 0;
    std::for_each(radios.begin(), radios.end(), [this](const int &radio)
    {
        m_radios.append(radio);
        total += radio;
    });

}

void ChartPanelView::paintEvent(QPaintEvent *event)
{
    resizeColumnsToContents();
    QTableView::paintEvent(event);
}

int ChartPanelView::sizeHintForColumn(int column) const
{
    if(m_radios.size() == 0)
    {
        return width() / horizontalHeader()->count();
    }
    else
    {
        return width() * m_radios.at(column) / total;
    }
}
