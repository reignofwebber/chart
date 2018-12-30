#ifndef CHARTMODEL_H
#define CHARTMODEL_H

#include <QAbstractTableModel>

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

    const static int COL_SHOW = 0;
    const static int COL_NAME = 1;
    const static int COL_VALUE = 2;
    const static int COL_COLOR = 3;
    const static int COL_MARK = 4;

//    QVector<Qt::CheckState> m_showState{5, Qt::Unchecked};
    QVector<bool> m_showState;
};

#endif // CHARTMODEL_H
