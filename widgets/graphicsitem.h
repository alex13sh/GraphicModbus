#ifndef GRAPHICSITEM_H
#define GRAPHICSITEM_H

#include <QtCharts/QChart>
#include <QtCharts/QChartView>

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

signals:

private:
    QDateTimeAxis *m_axisX;
    QValueAxis *m_axisY;
};

using QCharView = QtCharts::QChartView;
class GraphicsWidget : public QCharView
{
    Q_OBJECT
public:
    GraphicsWidget(QWidget *parent);
};

#endif // GRAPHICSITEM_H
