#ifndef GRAPHICSITEM_H
#define GRAPHICSITEM_H

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include "../core/defines.h"

QT_CHARTS_BEGIN_NAMESPACE
class QSplineSeries;
class QValueAxis;
class QDateTimeAxis;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class GraphicsItem : public QChart
{
    Q_OBJECT
public:
    GraphicsItem(QObject *parent = nullptr);

    void setValues(const QVector<ModbusValue*> &values);
    void setSensors(const QVector<ModbusSensor*> &values);
    void updateValues();
    void updateSensors();
signals:

private:
    QDateTimeAxis *m_axisX;
    QValueAxis *m_axisY;
    QList<QSplineSeries *>m_series;
    QVector<ModbusValue*> m_values;
    QVector<ModbusSensor*> m_sensors;
    const int m_secondsScala = 50;
};

using QCharView = QtCharts::QChartView;
class GraphicsWidget : public QCharView
{
    Q_OBJECT
public:
    GraphicsWidget(QWidget *parent);
};

#endif // GRAPHICSITEM_H
