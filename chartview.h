#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QChartView>

#include <QDebug>
#include <tuple>

namespace QtCharts {
    class QChart;
    class QLineSeries;
    class QValueAxis;
    class QDateTimeAxis;
}

class CursorItem;
class ChartData;
class QDateTime;

using namespace QtCharts;

class ChartView : public QtCharts::QChartView
{
    Q_OBJECT

public:
    enum ChartType
    {
        ANALOG_TYPE,
        DIGITAL_TYPE
    };
public:
    ChartView(QWidget *parent = 0);

    //
    void setChartBold(ChartType type, const QString &id, bool bold);
    //
    void setChartHalf();
    void setChartFull(ChartType type);
    void setChartShow(ChartType type, bool show);

    //
    void addVariable(ChartType type, const QString &id, const QString &name, unsigned color);
    void removeVariable(ChartType type, const QString &id);
    // 为变量ID加点
    void addPoint(QString id, qreal time, qreal val);
    // 获取所有变量id
    QVector<QString> getAllVariableIds();
    // 所有变量加点完成
    void addPointComplete();

signals:
    void setValue(QString id, qreal value);
    void setDigitalValue(QString id, qreal value);

public slots:
    void onShowChanged(const QString &id, bool show);
    void onColorChanged(const QString &id, unsigned color);
    void resetAxisX();
    // show active cursor
    void showActiveCursor(bool show);
    // show cursors
    void showCursors(bool show);
    // delete last cursor from m_cursors
    void popCursor();

    // if show min max point
    void showMinMax(bool show);


protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    // find series index when x is known
    int getIndex(const QString id, quint64 time) const;
    qreal interpolateY(const QString id, int index, quint64 time) const;

    // update cursors position
    void updateCursorsPos();
    // change or no change to min and max point
    void changeMinMaxPos(const QString &id);
    // update min max point position
    void updateMinMaxPos();

    // show min max
    void showMinMax(const QString &id, bool show);


private:
    qreal press_x;      // 记录press点的x值

    // graphic items
    QtCharts::QChart *m_chart;


    // just for analog
    //1. cursors
    CursorItem *m_activeCursor;
    QMap<quint64, CursorItem *> m_cursorMap;
    // aux for popCursor
    QVector<CursorItem *> m_cursors;

    //2. min max id --> <min, max, min(x), max(x)>
    QMap<QString, std::tuple<QGraphicsPolygonItem *, QGraphicsEllipseItem *, quint64, quint64>> m_minMaxMap;
    // optimize only show updateMinMaxPos
    QMap<QString, bool> m_showMinMax;
    bool m_showMinMaxOnCreate = false;

    QValueAxis *m_analogAxisY;
    QValueAxis *m_digitalAxisY;

    QVector<QLineSeries *> m_analogSeriesPool;
    QVector<QLineSeries *> m_digitalSeriesPool;
    QVector<qreal> m_digitalOffsetPool;

    QMap<QString, QLineSeries *> m_analogSeriesMap;
    QMap<QString, QLineSeries *> m_digitalSeriesMap;
    QMap<QString, qreal> m_digitalOffsetMap;

    // id --> name map
    QMap<QString, QString> m_id_name_map;


    // analog
    // 模拟量y轴最小值
    qreal m_analogMinY = -1;
    // 模拟量y轴最大值
    qreal m_analogMaxY = 1;
    // 数字量y轴最小值
    qreal m_digitalMinY = 0;
    // 数字量y轴最大值
    qreal m_digitalMaxY;    //need calculate by digitalOffset and poolSize

    // 波形图总秒数
    int m_totoalSeconds = 60;
    // 数字量间距
    qreal digitalOffset = 0.2;
    int m_poolSize = 10;

};

#endif // CHARTVIEW_H
