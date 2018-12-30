#ifndef CHARTPANELVIEW_H
#define CHARTPANELVIEW_H

#include <QTableView>
#include <QVector>


class ChartPanelView : public QTableView
{
public:
    ChartPanelView(QWidget *parent = 0);
    void initColsWidthRatio(const QVector<int> &radios);

    void paintEvent(QPaintEvent *event);

    int sizeHintForColumn(int column) const;

private:
    QVector<int> m_radios;
    // optimized radio之和
    int total;

};

#endif // CHARTPANELVIEW_H
