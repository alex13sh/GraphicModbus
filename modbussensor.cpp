#include "modbussensor.h"

#include "modbusdevice.h"
//#include <QModbusRtuSerialMaster>

ModbusSensor::ModbusSensor(QObject *parent)
    : QObject(parent)
{}

ModbusSensor::ModbusSensor(const QString &name, quint16 address, QObject *parent)
    : QObject(parent), m_name(name), m_address(address)
{}

ModbusSensor::ModbusSensor(const QString &name, quint8 pin, ModbusDevice *module)
    : QObject(module), m_name(name)//, m_module(module), m_pin(pin)
{
    setModule(module, pin);
}

void ModbusSensor::setModule(ModbusDevice *module, int pin){
    this->m_module=module;
    this->m_pin = pin;
//    module->setSensor(pin, this);
    module->m_sensors[pin] = this;
}

void ModbusSensor::setType_(SensorType_ type){m_type_=type;}

void ModbusSensor::updateValue()
{
    m_module->getValueSensor(m_pin);
}
