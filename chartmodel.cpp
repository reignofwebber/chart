#include "chartmodel.h"

#include <QColor>
#include <QDebug>

ChartModel::ChartModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}

int ChartModel::rowCount(const QModelIndex &parent) const
{
    return m_data.size();
}

int ChartModel::columnCount(const QModelIndex &parent) const
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
            m_data[index.row()].show = value.toBool();
        }
        else if(index.column() == COL_STAR)
        {
            m_data[index.row()].star = value.toBool();
        }
        else if(index.column() == COL_COLOR)
        {
            m_data[index.row()].color = value.toUInt();
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
        if(index.column() == COL_SHOW)
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
    endRemoveRows();
    return QAbstractTableModel::removeRow(row, parent);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "chartplot.h"

void ChartModel::addVariate()
{
    static int id = 0;
    VariateData data;
    data.id = QString::number(id);
    data.variateName = QString("列车速度%1").arg(id);
    data.length = rand() % 2 ? 1 : 8;
//    data.value = data.length == 1 ? (rand % 2) : (rand % 100);

    addVariate(data);
    ++id;
}

void ChartModel::addVariate(const VariateData &data)
{
    ChartData cdata;
    cdata.id = data.id;
    cdata.name = data.variateName;
    cdata.show = true;
    cdata.star = false;
    cdata.color = getRandomColor();
    beginResetModel();
    m_data.append(cdata);
    endResetModel();
}

unsigned ChartModel::getRandomColor() const
{
    unsigned data = 0;
    data += (std::rand() % 256) << 16;
    data += (std::rand() % 256) << 8;
    data += (std::rand() % 256);
    return data;
}
