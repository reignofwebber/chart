#ifndef CHARTMODEL_H
#define CHARTMODEL_H

#include <QAbstractTableModel>

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

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    // show
    QVector<bool> m_showState;
    // color
    QVector<unsigned> m_colors;
    // star
    QVector<bool> m_starState;
};

#endif // CHARTMODEL_H
