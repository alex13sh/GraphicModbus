#include "modbussensor.h"

#include "modbusdevice.h"
//#include <QModbusRtuSerialMaster>

ModbusSensor::ModbusSensor(QObject *parent)
    : QObject(parent)
{}

ModbusSensor::ModbusSensor(const QString &name, quint16 address, QObject *parent)
    : QObject(parent), name(name), address(address)
{}

void ModbusSensor::setModule(ModbusDevice *module, int pin){
    this->module=module;
//    module->setSensor(pin, this);
    module->m_sensors[pin] = this;
}

void ModbusSensor::setType_(SensorType_ type){type_=type;}

void ModbusSensor::updateValue()
{
    auto device = this->module->device();

}
