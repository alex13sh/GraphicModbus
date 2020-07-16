#include "modbussensor.h"

#include "modbusdevice.h"
#include "modbusvalue.h"

ModbusSensor::ModbusSensor(QObject *parent)
    : QObject(parent)
{}

ModbusSensor::ModbusSensor(const QString &name, quint8 pin, ModbusDevice *module)
    : QObject(module), m_name(name)//, m_module(module), m_pin(pin)
{
    setModule(module, pin);
}

void ModbusSensor::setModule(ModbusDevice *module, int pin){
    this->m_module=module;
    this->m_pin = pin;
    module->setSensor(pin, this);
//    module->m_sensors[pin] = this;
}

void ModbusSensor::addValue(quint16 address, ModbusValue *value) {
    m_values.insert(address, value);
    m_mapNameValue.insert(value->name(), value);
}

ListValues ModbusSensor::values() const {
    return m_values;
}

void ModbusSensor::setInterval(quint16 interval)
{
    if(!v_interval) return;
    v_interval->setValue_int16(interval);
}

void ModbusSensor::updateValue()
{
    if(v_value) v_value->updateValues();
}
