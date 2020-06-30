#include "modbusvalue.h"
#include "modbusdevice.h"
#include "modbussensor.h"

#include <QTimer>

ModbusValue::ModbusValue(ModbusDevice *module, const QString &name, quint16 address, quint8 size)
    : QObject(module)
    , m_module(module)
    , m_name(name), m_address(address), m_size(size)
{
    m_updateValue = new QTimer(this);
    connect(m_updateValue, &QTimer::timeout, this, [this](){this->updateValues();});
}

ModbusValue::ModbusValue(ModbusSensor *sensor, const QString &name, quint16 address, quint8 size)
    : QObject(sensor)
    , m_module(sensor->module()), m_sensor(sensor)
    , m_name(name), m_address(address), m_size(size)
{

}

void ModbusValue::setValues(const ValuesType &values) {
    if(m_values == values) return;
    m_values = values;
    // отправить значение
    emit valuesChanged();
}

void ModbusValue::updateValues(const ValuesType &values) {
    if(m_values == values) return;
    m_values = values;
    emit valuesChanged();
}

void ModbusValue::updateValues() {
    m_module->sendRead(m_address, m_size);
}

ValuesType ModbusValue::values() const {
    return m_values;
}

int ModbusValue::value_int() const {
    if(m_values.size()==1)
        return value_int16();
    else if(m_values.size()==2)
        return value_int32();
    return -1;
}

float ModbusValue::value_float() const {
    if(m_values.size()<2) return -1.0;
    qint32 ivalue = (((qint32)m_values[1]<<16)|(qint32)m_values[0]);
    float fvalue=(float&)ivalue;
    return fvalue;
}

qint8 ModbusValue::value_int8() const {
    if(m_values.size()<1) return -1;
    qint8 ivalue=(qint8)(m_values[0]);
    return ivalue;
}

qint16 ModbusValue::value_int16() const {
    if(m_values.size()<1) return -1;
    qint16 ivalue=(qint16)(m_values[0]);
    return ivalue;
}

qint32 ModbusValue::value_int32() const {
    if(m_values.size()<2) return -1;
    qint32 ivalue = (((qint32)m_values[1]<<16)|(qint32)m_values[0]);
    return ivalue;
}
