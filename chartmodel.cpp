#include "chartmodel.h"

#include <QDebug>

ChartModel::ChartModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    m_showState.resize(5);
    m_starState.resize(5);
}

int ChartModel::rowCount(const QModelIndex &parent) const
{
    return 5;
}

int ChartModel::columnCount(const QModelIndex &parent) const
{
    return 5;
}

bool ChartModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

    if(role == Qt::UserRole)
    {
        if(index.column() == COL_SHOW)
        {
            m_showState[index.row()] = value.toBool();
            emit dataChanged(index, index);
        }
        else if(index.column() == COL_STAR)
        {
            m_starState[index.row()] = value.toBool();
            emit dataChanged(index, index);
        }
    }
    return QAbstractTableModel::setData(index, value, role);
}

QVariant ChartModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::UserRole)
    {
        if(index.column() == COL_SHOW)
        {
            return m_showState[index.row()];
        }
        else if(index.column() == COL_STAR)
        {
            return m_starState[index.row()];
        }
    }
    return QVariant();
}

QVariant ChartModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QStringList headerList;
    headerList << "" << "名称" << "值" << "颜色"  << "";
    if(role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return headerList.at(section);
    }
    return QVariant();
}

Qt::ItemFlags ChartModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
    if(index.column() == COL_SHOW || index.column() == COL_STAR)
    {
        flags ^= Qt::ItemIsSelectable;
    }else if(index.column() == COL_NAME)
    {
        flags |= Qt::ItemIsEditable;
    }
    return flags;
}
