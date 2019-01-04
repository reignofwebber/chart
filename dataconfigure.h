#ifndef DATACONFIGURE_H
#define DATACONFIGURE_H

#include <QString>



#include <QVector>

#include "analyze_types.h"

class QXmlStreamAttributes;

class DataConfigure
{
public:
    static DataConfigure &GetInstance();

private:


    DataConfigure();

    // parse data configure xml file
    bool parseConfigureXml(Configure_Type type);
    VariateData parseVariate(const QXmlStreamAttributes &attrs);

public:
    // once init, don't change any more
    QVector<VariateData> m_variates;
    QVector<VariateData> m_ermVariates;
};

#endif // DATACONFIGURE_H
