#ifndef CHARTMODEL_H
#define CHARTMODEL_H

#include <QAbstractTableModel>
#include <QMap>
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
    void addVariateData(const ChartData &data);
    void removeVariateData(const QString &id);


public slots:
    // 必须使用信号？
    void setValue(QString id, qreal value);

private:

    // data
    QVector<ChartData> m_data;
    // aux
    QMap<QString, int> m_id_row_map;
};

#endif // CHARTMODEL_H
