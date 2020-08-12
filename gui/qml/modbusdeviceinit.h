#ifndef MODBUSDEVICEINIT_H
#define MODBUSDEVICEINIT_H

#include <QObject>
#include <QtQml>
#include "core/defines.h"
//Q_DECLARE_METATYPE(MapDevices)

#include "core/modbusvalue.h"

#include "core/modbussensor.h"

#include <QLineSeries>
//class QLineSeries;
QT_CHARTS_USE_NAMESPACE
Q_DECLARE_METATYPE(QList<QPointF>)

class ModbusDevicesInit : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(MapDevices mapDevices READ mapDevices NOTIFY mapDevicesChanged)
    Q_PROPERTY(QVector<ModbusValue*> values READ getValues NOTIFY mapDevicesChanged)
    Q_PROPERTY(QVector<ModbusSensor*> sensors READ getListSensors NOTIFY mapDevicesChanged)
public:
    ModbusDevicesInit(QObject *parent=nullptr);

public slots:

    void init_devices();
    void init_devices_analog();
    void init_devices_analog_2();
    void init_devices_digital();

//    ListSensors getMapSensors();
    QVector<ModbusSensor*> getListSensors();
//    QList<ModbusValue*> getListValues();
    MapDevices mapDevices() const {return m_mapDevices;}
    QVector<ModbusValue*> getValues(bool readOnly=false);

    Q_INVOKABLE void series_setPoints(const QString &hash, QList<QPointF> points, QLineSeries *series);
    Q_INVOKABLE qint64 getMSecsSinceEpoch() const ;

signals:
    void mapDevicesChanged();

private:
    MapDevices m_mapDevices;
    QMap<QString, ModbusSensor*> m_mapSensors;
};

#endif // MODBUSDEVICEINIT_H
