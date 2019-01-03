#include "chartmodel.h"

#include <QColor>
#include <QDebug>


ChartModel::ChartModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}

int ChartModel::rowCount(const QModelIndex &/*parent*/) const
{
    return m_data.size();
}

int ChartModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 5;
}

bool ChartModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid()) return false;
    if(role == Qt::UserRole)
    {
        if(index.column() == COL_SHOW)
        {
            bool show = value.toBool();
            m_data[index.row()].show = show;
            emit showChanged(m_data[index.row()].id, show);
        }
        else if(index.column() == COL_STAR)
        {
            m_data[index.row()].star = value.toBool();
        }
        else if(index.column() == COL_COLOR)
        {
            unsigned color = value.toUInt();
            m_data[index.row()].color = color;
            emit colorChanged(m_data[index.row()].id, color);
        }
        emit dataChanged(index, index);
    }
    return QAbstractTableModel::setData(index, value, role);
}

QVariant ChartModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()) return QVariant();
    if(role == Qt::DisplayRole)
    {
        if(index.column() == COL_NAME)
        {
            return m_data[index.row()].name;
        }
        else if(index.column() == COL_VALUE)
        {
            return m_data[index.row()].value;
        }
    }
    if(role == Qt::TextAlignmentRole)
    {
        if(index.column() == COL_NAME || index.column() == COL_VALUE)
        {
            return (int)(Qt::AlignVCenter | Qt::AlignHCenter);
        }

    }
    if(role == Qt::UserRole)
    {
        if(index.column() == COL_NAME)
        {
            return m_data[index.row()].id;
        }else if(index.column() == COL_SHOW)
        {
            return m_data[index.row()].show;
        }
        else if(index.column() == COL_STAR)
        {
            return m_data[index.row()].star;
        }
        else if(index.column() == COL_COLOR)
        {
            return m_data[index.row()].color;
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
    if(index.column() != COL_NAME)
    {
        flags ^= Qt::ItemIsSelectable;
    }
    return flags;
}

bool ChartModel::removeRow(int row, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row);
    m_data.removeAt(row);

    // remove from m_id_row_map
    m_id_row_map.clear();
    for(int i = 0; i < rowCount(QModelIndex()); ++i)
    {
        QModelIndex inx = index(i, COL_NAME);
        m_id_row_map[data(inx, Qt::UserRole).toString()] = i;
    }

    endRemoveRows();
    return QAbstractTableModel::removeRow(row, parent);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void ChartModel::addVariate(const ChartData &data)
{
    beginResetModel();
    m_data.append(data);
    m_id_row_map[data.id] = m_data.size() - 1;
    endResetModel();
}

void ChartModel::setValue(QString id, qreal value)
{
    if(m_id_row_map.contains(id))
    {
        m_data[m_id_row_map[id]].value = value;
        QModelIndex i = index(m_id_row_map[id], COL_VALUE);
        emit dataChanged(i, i);
    }
    else
    {
        qDebug() << "setValue : can not find id "  << id;
    }
}
