#include "modbussensor.h"

#include "modbusdevice.h"
//#include <QModbusRtuSerialMaster>

ModbusSensor::ModbusSensor(QObject *parent)
    : QObject(parent)
{

}

ModbusSensor::ModbusSensor(const QString &name, quint16 address, QObject *parent)
    : QObject(parent), name(name), address(address)
{}

void ModbusSensor::setModule(const ModbusDevice *module){this->module=module;}

void ModbusSensor::setType_(SensorType_ type){type_=type;}

void ModbusSensor::updateValue()
{
    //    auto device = this->module->device();

}
