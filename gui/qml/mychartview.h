#ifndef MYCHARTVIEW_H
#define MYCHARTVIEW_H

#include <QObject>
#include <QQuickItem>
#include <private/declarativechart_p.h>
QT_CHARTS_USE_NAMESPACE

//#include "core/modbussensor.h"
#include <core/defines.h>
#include <QDateTime>
#include <QLineSeries>

//class QLineSeries;

class MyChartView : public DeclarativeChart
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QAbstractAxis *axisDate WRITE setAxisDate)
    Q_PROPERTY(QAbstractAxis *axisTemer WRITE setAxisTemer)
    Q_PROPERTY(QAbstractAxis *axisVibra MEMBER m_axisVibra)
    Q_PROPERTY(QAbstractAxis *axisDavl WRITE setAxisDavl)
    Q_PROPERTY(QVector<QAbstractSeries*> myseries READ myseries NOTIFY myseriesChanged)
    Q_PROPERTY(bool isStart MEMBER m_isStart)
public:
    MyChartView(QQuickItem *parent = 0);

    Q_INVOKABLE QVector<QAbstractSeries*> setSensors(const QVector<ModbusSensor*> &sens);
    QVector<QAbstractSeries*> myseries() const {return m_sers;}

    void setAxisDate(QAbstractAxis *axisDate);
    void setAxisTemer(QAbstractAxis *axisTemer) {m_axisTemer=axisTemer;}
    void setAxisDavl(QAbstractAxis *axisDavl)   {m_axisDavl=axisDavl;}

signals:
    void myseriesChanged();
    void selectedValues(const QDateTime &time, const QList<float> &values);

public slots:
    void updateSensors();
    void setValuesPoints(QAbstractSeries* sers, QDateTime start, QDateTime finish, const QList<QPointF> &points);
    void pushValuesPoints(const QList<QPointF> &points);

    void clearValues();
private:
    QVector<ModbusSensor*> m_sens;
    QVector<QAbstractSeries*> m_sers;
    QAbstractAxis *m_axisDate=nullptr,
        *m_axisTemer=nullptr, *m_axisDavl=nullptr, *m_axisVibra=nullptr;
    int m_secondsScala = 50;
    bool m_isStart=false;

    QPointF getValue(QLineSeries *ser, QDateTime dt);
    // QQuickItem interface
protected:
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
    void hoverMoveEvent(QHoverEvent *event) override;
};

#endif // MYCHARTVIEW_H
