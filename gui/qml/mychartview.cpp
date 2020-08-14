#include "mychartview.h"
#include <QDebug>

#include <QDateTimeAxis>
#include <QLineSeries>
#include <QValueAxis>

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
    static int cnt = 0;
    cnt+=1;
    cnt=cnt%10;
    auto momentInTime = QDateTime::currentDateTime().toMSecsSinceEpoch() ;
//    for( auto *s : m_sens) {
    for (int i=0; i<m_sens.size(); ++i){
        auto *sens = m_sens[i];
        auto *sers = static_cast<QLineSeries*>(m_sers[i]);
//        float v = sens->value_float();
        float v = m_sens.size()*cnt + i;
        sers->append(momentInTime, v);
    }

    m_axisDate->setRange(QDateTime::currentDateTime().addSecs(-m_secondsScala+1), QDateTime::currentDateTime().addSecs(1));
}

void MyChartView::setValuesPoints(QAbstractSeries *sers, QDateTime start, QDateTime finish, const QList<QPointF> &points)
{
    if (points.size()==0) return;
    auto sers_ = static_cast<QLineSeries*>(sers);
    if(sers) {
        auto dt = static_cast<QDateTimeAxis*>(m_axisDate);
        dt->setRange(start, finish);

        QList<QPointF> res;
        ModbusSensor *sens = nullptr;
        int sens_i;
        for (sens_i=0; sens_i<m_sers.size(); ++sens_i)
            if (m_sers[sens_i] == sers)
                sens = m_sens[sens_i];

        if (!sens) return;
        float v_min= 0;
        float v_max=v_min;
        for(auto &p : points) {
            quint32 iv = (quint32) p.y();
            if (iv == -6.49037e+32) continue;
            auto v = sens->value_float_from_int(iv);
            if (v == -6.49037e+32) continue;
            res.append(QPointF(p.x(), v));
//            p.setY(v);
            if (v<v_min) v_min = v;
            else if (v>v_max) v_max = v;
        }
        qDebug()<< "MyChartView::setValuesPoints sens_i="<<sens_i<<" range:"<<v_min<<", "<<v_max;
        auto avalues = static_cast<QValueAxis*>(m_axisTemer);
//        avalues->setRange(v_min, v_max);
        sers_->replace(res);
    } else qDebug()<<"MyChartView::setValuesPoints Error";
}

void MyChartView::pushValuesPoints(const QList<QPointF> &points)
{

}

