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


    const int m_secondsScala = 20;
    m_axisX->setTickCount(m_secondsScala+1);
    m_axisX->setRange(QDateTime::currentDateTime().addSecs(-m_secondsScala+1), QDateTime::currentDateTime().addSecs(1));
    m_axisX->setFormat("h:mm:ss");
    m_axisX->setTitleText("Date");
    m_axisY->setRange(-5, 10);
}

GraphicsWidget::GraphicsWidget(QWidget *parent)
    : QCharView(parent)
{
    GraphicsItem *chart = new GraphicsItem(this);
    setChart(chart);
}
