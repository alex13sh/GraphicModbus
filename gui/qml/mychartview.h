#ifndef MYCHARTVIEW_H
#define MYCHARTVIEW_H

#include <QObject>
#include <QQuickItem>
#include <private/declarativechart_p.h>
QT_CHARTS_USE_NAMESPACE

//#include "core/modbussensor.h"
#include <core/defines.h>

class MyChartView : public DeclarativeChart
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QAbstractAxis *axisDate WRITE setAxisDate)
    Q_PROPERTY(QAbstractAxis *axisTemer WRITE setAxisTemer)
    Q_PROPERTY(QAbstractAxis *axisDavl WRITE setAxisDavl)
    Q_PROPERTY(QVector<QAbstractSeries*> myseries READ myseries NOTIFY myseriesChanged)
public:
    MyChartView(QQuickItem *parent = 0);

    Q_INVOKABLE QVector<QAbstractSeries*> setSensors(const QVector<ModbusSensor*> &sens);
    QVector<QAbstractSeries*> myseries() const {return m_sers;}

    void setAxisDate(QAbstractAxis *axisDate)   {m_axisDate=axisDate;}
    void setAxisTemer(QAbstractAxis *axisTemer) {m_axisTemer=axisTemer;}
    void setAxisDavl(QAbstractAxis *axisDavl)   {m_axisDavl=axisDavl;}

signals:
    void myseriesChanged();

private:
    QVector<ModbusSensor*> m_sens;
    QVector<QAbstractSeries*> m_sers;
    QAbstractAxis *m_axisDate=nullptr,
        *m_axisTemer=nullptr, *m_axisDavl=nullptr;
};

#endif // MYCHARTVIEW_H
