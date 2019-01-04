#ifndef ANALYZE_TYPES_H
#define ANALYZE_TYPES_H

#include <QString>

enum Configure_Type
{
    REALDATA_Configure,
    ERMDATA_Configure,
};

enum Chart_Type
{
    ANALOG_TYPE,
    DIGITAL_TYPE,
    Chart_Type_ALL,
};

struct VariateData
{
    QString id;
    int byteOffset;
    int bitOffset;
    int length;
    float dimension;
    float valueRatio;
    // 只是对于温度
    float valueOffset;
    QString unite;
    QString variateName;
    QString systemName;
    bool operator==(const VariateData& data)const
    {
        return (byteOffset == data.byteOffset) && (bitOffset == data.bitOffset) && (length == data.length);
    }
    bool operator>(const VariateData& data)const
    {
        return ((byteOffset < data.byteOffset)
                        || ((byteOffset == data.byteOffset) && (bitOffset < data.bitOffset)));
    }
    bool operator<(const VariateData& data)const
    {
        return ( (byteOffset > data.byteOffset)
                        || ((byteOffset == data.byteOffset) && (bitOffset > data.bitOffset)));
    }
};

struct ChartData
{
    QString id;         // from VariateData
    QString name;       // from VariateData'
    int length;         // from VariateData
    float value = 0;    // value
    unsigned color;     // color
    bool show;          // if show in chart
    bool star;          // if save for next open
};



#endif // ANALYZE_TYPES_H
