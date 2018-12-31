#include "chartpanelview.h"

#include <QStandardItemModel>
#include <QHeaderView>
#include <QScrollBar>
#include <QPainter>
#include <QDebug>
#include <QLayout>

ChartPanelView::ChartPanelView(QWidget *parent)
    : QTableView(parent)
{
    verticalHeader()->setVisible(false);
    horizontalHeader()->setStretchLastSection(true);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setAlternatingRowColors(true);

}

void ChartPanelView::setColsWidthRatio(const QVector<int> &radios)
{
    m_radios.clear();
    total = 0;

    for(int i = 0; i < horizontalHeader()->count(); ++i)
    {
        if(radios.at(i) == 0)
        {
            setColumnHidden(i, true);
        }
        else
        {
            setColumnHidden(i, false);
        }

        m_radios.append(radios.at(i));
        total += radios.at(i);
    }

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
