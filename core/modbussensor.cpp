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
//    module->m_sensors.insert(pin, this);
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

void ModbusSensor::setValueErr(float warn, float err)
{
    m_value_warn = warn;
    m_value_err = err;
}

QString ModbusSensor::hash() const
{
    if (v_value) return v_value->hash_str().left(10);
    return QString();
}

void ModbusSensor::updateValue()
{
    if(v_value) v_value->updateValues();
}

float ModbusSensor::get_value_float() {
    float v = value_float();
    if (v>m_value_max)
        setValueMax(v);
    return v;
}

float ModbusSensor::get_value_float_from_int(quint32 ivalue)
{
    float v = value_float_from_int(ivalue);
    if (v>m_value_max)
        setValueMax (v);
    return v;
}

float ModbusSensor::value_float_from_int(quint32 ivalue) {
    float fvalue=(float&)ivalue;
    return fvalue;
}

void ModbusSensor::setValueMax(float value)
{
    if (value == m_value_max) return;
    m_value_max = value;
    emit value_maxChanged();
}
