#include "configuredialog.h"
#include "ui_configuredialog.h"

#include <QTreeView>
#include <QStandardItemModel>
#include <QDebug>

#include "dataconfigure.h"

ConfigureDialog::ConfigureDialog(Configure_Type configureType, Chart_Type chartType, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConfigureDialog)
    , m_configureType(configureType)
    , m_chartType(chartType)
{
    ui->setupUi(this);

    m_model = new QStandardItemModel(this);
    m_model->setHorizontalHeaderLabels(QStringList() << "变量名称");
    ui->m_view->setModel(m_model);

    updateModelData();
    connect(ui->m_searchBtn, SIGNAL(clicked()), this, SLOT(onSearchItem()));
    connect(m_model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(onItemChanged(QStandardItem*)));

}

ConfigureDialog::~ConfigureDialog()
{
    qDebug() << "delete";
    delete ui;
}

void ConfigureDialog::setType(Configure_Type configureType, Chart_Type chartType)
{
    m_configureType = configureType;
    m_chartType = chartType;
    updateModelData();
}

void ConfigureDialog::setReadyVariates(const QVector<QString> &id)
{
   m_readyIds = id;
   updateModelData();
}

void ConfigureDialog::onSearchItem()
{
    updateModelData(ui->m_searchEdit->text());
}

void ConfigureDialog::onItemChanged(QStandardItem *item)
{
    emit addVariateData(m_item_data_map[item], item->checkState() == Qt::Checked ? true : false);
}

void ConfigureDialog::updateModelData(QString text)
{
    m_model->removeRows(0, m_model->rowCount());
    m_item_data_map.clear();

    if(m_configureType == REALDATA_Configure)
    {
        for(const VariateData &data : DataConfigure::GetInstance().m_variates)
        {
            if(m_chartType == ANALOG_TYPE)
            {
                if(data.length == 1) continue;
            }else if(m_chartType == DIGITAL_TYPE)
            {
                if(data.length > 1) continue;
            }

            if(text != "" && !data.variateName.contains(text, Qt::CaseInsensitive)) continue;

            // name
            QString displayName;
            if(data.systemName != "")
                displayName = QString("%1_%2").arg(data.systemName).arg(data.variateName);
            else
                displayName = data.variateName;

            QStandardItem *nameItem = new QStandardItem(displayName);
            nameItem->setCheckable(true);
            if(m_readyIds.contains(data.id))
            {
                nameItem->setCheckState(Qt::Checked);
            }
            else
            {
                nameItem->setCheckState(Qt::Unchecked);
            }
            m_item_data_map.insert(nameItem, data);
            m_model->appendRow(nameItem);
        }

    }
    else if(m_configureType == ERMDATA_Configure)
    {
        for(const VariateData &data : DataConfigure::GetInstance().m_ermVariates)
        {
            if(m_chartType == ANALOG_TYPE)
            {
                if(data.length == 1) continue;
            }else if(m_chartType == DIGITAL_TYPE)
            {
                if(data.length > 1) continue;
            }

            if(text != "" && !data.variateName.contains(text, Qt::CaseInsensitive)) continue;

            // name
            QString displayName;
            if(data.systemName != "")
                displayName = QString("%1_%2").arg(data.systemName).arg(data.variateName);
            else
                displayName = data.variateName;

            QStandardItem *nameItem = new QStandardItem(displayName);
            nameItem->setCheckable(true);
            if(m_readyIds.contains(data.id))
            {
                nameItem->setCheckState(Qt::Checked);
            }
            else
            {
                nameItem->setCheckState(Qt::Unchecked);
            }
            m_item_data_map.insert(nameItem, data);
            m_model->appendRow(nameItem);
        }

    }else
    {
        qDebug() << "unsupport configureType " << m_configureType;
    }

}
