#include "mychartview.h"
#include <QDebug>

MyChartView::MyChartView(QQuickItem *parent)
    : DeclarativeChart(parent)
{

}

#include "core/modbussensor.h"
QVector<QAbstractSeries*> MyChartView::setSensors(const QVector<ModbusSensor *> &sens)
{
    if (!(m_axisDate && m_axisTemer)) return QVector<QAbstractSeries*>();

    for (auto *s : sens){
        auto *as = createSeries(SeriesTypeLine, s->name(), m_axisDate, m_axisTemer);
        m_sers.append(as);
    }
    return m_sers;
}

