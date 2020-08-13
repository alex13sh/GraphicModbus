#include "mychartview.h"
#include <QDebug>

#include <QDateTimeAxis>
#include <QLineSeries>

MyChartView::MyChartView(QQuickItem *parent)
    : DeclarativeChart(parent)
{
    setAnimationOptions(NoAnimation);
}

#include "core/modbussensor.h"
QVector<QAbstractSeries*> MyChartView::setSensors(const QVector<ModbusSensor *> &sens)
{
    if (!(m_axisDate && m_axisTemer)) return QVector<QAbstractSeries*>();

    m_sens = sens;
    for (auto *s : sens){
        auto *series = createSeries(SeriesTypeLine, s->name(), m_axisDate, m_axisTemer);
        m_sers.append(series);
        series->setUseOpenGL(true);
    }
    return m_sers;
}

void MyChartView::setAxisDate(QAbstractAxis *axisDate) {
    m_axisDate=axisDate;
    auto dt = static_cast<QDateTimeAxis*>(m_axisDate);
    m_secondsScala = dt->tickCount();
//    dt->setTickCount(m_secondsScala+1);
    dt->setRange(QDateTime::currentDateTime().addSecs(-m_secondsScala+1), QDateTime::currentDateTime().addSecs(1));
    dt->setFormat("h:mm:ss");
}

void MyChartView::updateSensors()
{
    auto momentInTime = QDateTime::currentDateTime().toMSecsSinceEpoch() ;
//    for( auto *s : m_sens) {
    for (int i=0; i<m_sens.size(); ++i){
        auto *sens = m_sens[i];
        auto *sers = static_cast<QLineSeries*>(m_sers[i]);
        float v = sens->value_float();
        sers->append(momentInTime, v);
    }

    m_axisDate->setRange(QDateTime::currentDateTime().addSecs(-m_secondsScala+1), QDateTime::currentDateTime().addSecs(1));
}

