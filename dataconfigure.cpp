#include "dataconfigure.h"

#include <QApplication>
#include <QXmlStreamReader>
#include <QFile>
#include <QDebug>

DataConfigure::DataConfigure()
{
    if(!parseConfigureXml(REALDATA_Configure))
    {
        qDebug() << "实时协议配置文件不存在!";
    }
    if(!parseConfigureXml(ERMDATA_Configure))
    {
        qDebug() << "ERM协议配置文件不存在!";
    }
}

DataConfigure &DataConfigure::GetInstance()
{
    static DataConfigure configure;
    return configure;
}

bool DataConfigure::parseConfigureXml(Configure_Type type)
{
    // real or erm
    QString path = QApplication::applicationDirPath() + "/dataconfigure.xml";

    if(type == ERMDATA_Configure)
    {
        path = QApplication::applicationDirPath() + "/ermdataconfigure.xml";
    }

    // read file
    QFile *xmlFile = new QFile(path);
    if(!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "can not open " << path;
        return false;
    }

    // parse file
    QXmlStreamReader *xml = new QXmlStreamReader(xmlFile);

    while(!xml->atEnd() && !xml->hasError())
    {
        if(xml->readNext() == QXmlStreamReader::StartElement)
        {
            if(xml->name() == "variate")
            {
                VariateData data = parseVariate(xml->attributes());
                if(type == REALDATA_Configure)
                {
                    m_variates.append(data);
                }else if(type == ERMDATA_Configure)
                {
                    m_ermVariates.append(data);
                }

            }
        }
    }

    if(xml->hasError())
    {
        qDebug() << "dataconfigure.xml has error";
        return false;
    }else
    {
        return true;
    }
}

VariateData DataConfigure::parseVariate(const QXmlStreamAttributes &attrs)
{
    static int id = 0;
    VariateData data;
    // id
    data.id = QString::number(id);
    // systemName
    if(attrs.hasAttribute("systemName"))
    {
        data.systemName = attrs.value("systemName").toString();
    }
    // variateName
    if(attrs.hasAttribute("variateName"))
    {
        data.variateName = attrs.value("variateName").toString();
    }
    // byteOffset
    if(attrs.hasAttribute("byteOffset"))
    {
        data.byteOffset = attrs.value("byteOffset").toInt();
    }
    // bitOffset
    if(attrs.hasAttribute("bitOffset"))
    {
        data.bitOffset = attrs.value("bitOffset").toInt();
    }
    // length
    if(attrs.hasAttribute("length"))
    {
        data.length = attrs.value("length").toInt();
    }
    // valueRatio
    if(attrs.hasAttribute("valueRatio"))
    {
        data.valueRatio = attrs.value("valueRatio").toFloat();
    }
    // valueOffset
    if(attrs.hasAttribute("valueOffset"))
    {
        data.valueOffset = attrs.value("valueOffset").toFloat();
    }
    // unite
    if(attrs.hasAttribute("unit"))
    {
        data.unite = attrs.value("unit").toString();
    }
    // dimension
    if(attrs.hasAttribute("dimension"))
    {
        data.dimension = attrs.value("dimension").toFloat();
    }
    ++id;
    return data;
}
