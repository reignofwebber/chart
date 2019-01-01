#ifndef CHARTMODEL_H
#define CHARTMODEL_H

#include <QAbstractTableModel>
#include "chartplot.h"

enum ChartTableCol
{
    COL_SHOW = 0,
    COL_NAME,
    COL_VALUE,
    COL_COLOR,
    COL_STAR
};


class ChartModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    ChartModel(QObject *parent = 0);

    // override from parent
public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool removeRow(int row, const QModelIndex &parent);

signals:
    void showChanged(QString id, bool show);
    void colorChanged(QString id, unsigned color);

public:
    void addVariate(const ChartData &data);

private:
    unsigned getRandomColor() const;

private:

    // data
    QVector<ChartData> m_data;
};

#endif // CHARTMODEL_H
