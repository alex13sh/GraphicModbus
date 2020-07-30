#ifndef MODBUSDEVICEINIT_H
#define MODBUSDEVICEINIT_H

#include <QObject>
#include <QtQml>
#include "core/defines.h"
Q_DECLARE_METATYPE(MapDevices)

#include "core/modbusvalue.h"
Q_DECLARE_METATYPE(QVector<ModbusValue*>)

#include "core/modbussensor.h"
Q_DECLARE_METATYPE(ModbusSensor*)
Q_DECLARE_METATYPE(QVector<ModbusSensor*>)

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
    void init_devices_digital();

//    ListSensors getMapSensors();
    QVector<ModbusSensor*> getListSensors();
//    QList<ModbusValue*> getListValues();
    MapDevices mapDevices() const {return m_mapDevices;}
    QVector<ModbusValue*> getValues(bool readOnly=false);

signals:
    void mapDevicesChanged();

private:
    MapDevices m_mapDevices;
};

#endif // MODBUSDEVICEINIT_H
