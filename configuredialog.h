#ifndef CONFIGUREDIALOG_H
#define CONFIGUREDIALOG_H

#include <QDialog>
#include <QMap>
#include "analyze_types.h"

namespace Ui {
class ConfigureDialog;
}

class QStandardItemModel;
class QStandardItem;
class VariateData;

class ConfigureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigureDialog(Configure_Type configureType, Chart_Type chartType, QWidget *parent = 0);
    ~ConfigureDialog();

    void setType(Configure_Type configureType, Chart_Type chartType);
    void setReadyVariates(const QVector<QString> &id);

signals:
    void addVariateData(const VariateData &data, bool add);

private:
    void updateModelData(QString text = "");

private slots:
    void onSearchItem();
    void onItemChanged(QStandardItem *item);

private:
    Ui::ConfigureDialog *ui;
    // type
    Configure_Type m_configureType;
    Chart_Type m_chartType;
    // ready variates
    QVector<QString> m_readyIds;

    QStandardItemModel *m_model;
    QMap<QStandardItem *, VariateData> m_item_data_map;
};

#endif // CONFIGUREDIALOG_H
