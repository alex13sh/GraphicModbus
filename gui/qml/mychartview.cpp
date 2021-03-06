#include "mychartview.h"
#include <QDebug>

#include <QDateTimeAxis>
#include <QLineSeries>
#include <QValueAxis>

MyChartView::MyChartView(QQuickItem *parent)
    : DeclarativeChart(parent)
{
    setAnimationOptions(NoAnimation);
//    setRubberBand(RectangleRubberBand);
}

#include "core/modbussensor.h"
QVector<QAbstractSeries*> MyChartView::setSensors(const QVector<ModbusSensor *> &sens)
{
    if (!(m_axisDate && m_axisTemer)) return QVector<QAbstractSeries*>();

    m_sens = sens;
    for (auto *s : sens){

        auto *series = createSeries(SeriesTypeLine, s->name(), m_axisDate,
                    s->hash()=="86c6deedfb"? m_axisDavl
                    : s->hash()=="8f2161a73d"? m_axisVibra
                    : m_axisTemer); // Кастыль!
        m_sers.append(series);
        series->setUseOpenGL(true);
    }
    return m_sers;
}

void MyChartView::setAxisDate(QAbstractAxis *axisDate) {
    m_axisDate=axisDate;
    auto dt = static_cast<QDateTimeAxis*>(m_axisDate);
    m_secondsScala = dt->tickCount()*20;
//    dt->setTickCount(m_secondsScala+1);
    dt->setRange(QDateTime::currentDateTime().addSecs(-m_secondsScala+1), QDateTime::currentDateTime().addSecs(1));
    dt->setFormat("h:mm:ss");
}

void MyChartView::updateSensors()
{
    qDebug()<< ">> MyChartView::updateSensors";
//    static int cnt = 0;
//    cnt+=1; cnt=cnt%10;
    auto momentInTime = QDateTime::currentDateTime().toMSecsSinceEpoch() ;
//    for( auto *s : m_sens) {
    for (int i=0; i<m_sens.size(); ++i){
        auto *sens = m_sens[i];
        auto *sers = static_cast<QLineSeries*>(m_sers[i]);
        float v = sens->get_value_float();
//        sens->updateValue();
//        float v = m_sens.size()*cnt + i;
        sers->append(momentInTime, v);
    }

    qDebug()<< "<< MyChartView::updateSensors";
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
            if (m_sers[sens_i] == sers) {
                sens = m_sens[sens_i];
                break;
            }

        if (!sens) return;
        sens->setValueMax(0);
        float v_min= 0;
        float v_max=v_min;
        for(auto &p : points) {
            quint32 iv = (quint32) p.y();
            if (iv == -6.49037e+32) continue;
            auto v = sens->get_value_float_from_int(iv);
            if (v == -6.49037e+32) continue;
            res.append(QPointF(p.x(), v));
//            p.setY(v);
            if (v<v_min) v_min = v;
            else if (v>v_max) v_max = v;
        }
        qDebug()<< "MyChartView::setValuesPoints sens_="<<sens->name()<<" range:"<<v_min<<", "<<v_max;
        auto avalues = static_cast<QValueAxis*>(m_axisTemer);
//        avalues->setRange(v_min, v_max);
        sers_->replace(res);
        sers_->setVisible(true);
    } else qDebug()<<"MyChartView::setValuesPoints Error";
}

void MyChartView::pushValuesPoints(const QList<QPointF> &points)
{

}

void MyChartView::clearValues()
{
    for (auto s:m_sers) {
        auto *sers = static_cast<QLineSeries*>(s);
        sers->clear();
    }
}

QPointF MyChartView::getValue(QLineSeries *ser, QDateTime dt)
{
    auto _dt = dt.toMSecsSinceEpoch();
    int i_min = 0, i_max = ser->count();
    int i;
    QPointF res;
    while(true) {
        i = i_min+(i_max-i_min)/2;
        qDebug()<<"binary seach i:"<<i<<", mix:"<<i_min<<", max:"<<i_max;
        res = ser->at(i);
        if (res.x()<_dt)
            i_min = i;
        else if (res.x()>_dt)
            i_max = i;
        else return res;
        if (i_max-i_min<=1)
            return res;
    }
}

void MyChartView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
//![1]
    case Qt::Key_Left:
        scrollLeft(10);
        break;
    case Qt::Key_Right:
        scrollRight(10);
        break;
    case Qt::Key_Up:
        scrollUp(10);
        break;
    case Qt::Key_Down:
        scrollDown(10);
        break;
    default:
        DeclarativeChart::keyPressEvent(event);
        break;
    }
}

void MyChartView::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        auto r = plotArea();
        if (event->modifiers() & Qt::ShiftModifier) {
            auto dlt = event->angleDelta().y() > 0 ? 10 : -10;
            r.setX(r.x()+dlt);
            r.setWidth(r.width()-2*dlt);
        } else {
            auto dlt = event->angleDelta().y() > 0 ? 10 : -10;
            r.setY(r.y()+dlt);
            r.setHeight(r.height()-2*dlt);
        }
        zoomIn(r);
    } else {
        if (event->modifiers() & Qt::ShiftModifier) {
            if (event->angleDelta().y() > 0)
                scrollRight(10);
            else if (event->angleDelta().y() < 0)
                scrollLeft(10);
        } else {
            if (event->angleDelta().y() > 0)
                scrollUp(10);
            else if (event->angleDelta().y() < 0)
                scrollDown(10);
        }
    }
}

void MyChartView::hoverMoveEvent(QHoverEvent *event)
{
    if (!m_isStart && event->modifiers() & Qt::ControlModifier) {
        auto pos = event->posF();
        QList<float> values;
        QDateTime dt = QDateTime::fromMSecsSinceEpoch( mapToValue(pos, m_sers.first()).x() );
        for (auto s : m_sers) {
            auto v = getValue(static_cast<QLineSeries*>(s), dt);
            values.append( v.y() );
        }
        emit selectedValues(dt, values);
    }
}
