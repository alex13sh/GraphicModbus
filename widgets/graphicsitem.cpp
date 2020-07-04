#include "graphicsitem.h"
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>

#include <QDateTime>
#include <QDateTimeAxis>

GraphicsItem::GraphicsItem(QObject *parent)
    : QChart(QChart::ChartTypeCartesian, nullptr, {}),
      m_axisX(new QDateTimeAxis()),
      m_axisY(new QValueAxis())
{
    setParent(parent);
    setAnimationDuration(500);
    setAnimationOptions(QChart::NoAnimation);

    addAxis(m_axisX,Qt::AlignBottom);
    addAxis(m_axisY,Qt::AlignLeft);

    m_axisX->setTickCount(m_secondsScala+1);
    m_axisX->setRange(QDateTime::currentDateTime().addSecs(-m_secondsScala+1), QDateTime::currentDateTime().addSecs(1));
    m_axisX->setFormat("h:mm:ss");
    m_axisX->setTitleText("Date");
    m_axisY->setRange(-5, 300);
}

#include "../core/modbusvalue.h"
void GraphicsItem::setValues(const QVector<ModbusValue *> &values){
    m_values=values;
    m_series.clear();
    for(auto v:values){
        auto series = new QSplineSeries(this);
        series->setUseOpenGL(true);
        series->setPen(QPen(QBrush(Qt::GlobalColor(m_series.size()%10+7)),4));//(green);
        series->setName(v->name());
        addSeries(series);
        m_series.append(series);
        series->attachAxis(m_axisX);
        series->attachAxis(m_axisY);
    }
}

void GraphicsItem::updateValues() {
    static int CountTic=0;
    QDateTime momentInTime = QDateTime::currentDateTime() ;
//    qDebug()<<++CountTic<<") DateTime:"<< momentInTime;
    for(int i=0; i<m_values.size();++i){
        auto ser = m_series[i];
        ser->append( momentInTime.toMSecsSinceEpoch(),
                m_values[i]->isFloatType()?m_values[i]->value_float():m_values[i]->value_int());
    }
    m_axisX->setRange(QDateTime::currentDateTime().addSecs(-m_secondsScala+1), QDateTime::currentDateTime().addSecs(1));
}

GraphicsWidget::GraphicsWidget(QWidget *parent)
    : QCharView(parent)
{
    GraphicsItem *m_chart = new GraphicsItem(this);
    setChart(m_chart);
}
