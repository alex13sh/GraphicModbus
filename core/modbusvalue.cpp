#include "modbusvalue.h"
#include "modbusdevice.h"
#include "modbussensor.h"

#include <QTimer>
#include <QDebug>
#include <QTime>

ModbusValue::ModbusValue(ModbusDevice *module, const QString &name, quint16 address, quint8 size)
    : QObject(module)
    , m_module(module)
    , m_name(name), m_address(address), m_size(size)
{
    m_updateValue = new QTimer(this);
    connect(m_updateValue, &QTimer::timeout, this, [this](){this->updateValues();});
    update_hash();
}

ModbusValue::ModbusValue(ModbusSensor *sensor, const QString &name, quint16 address, quint8 size)
    : QObject(sensor)
    , m_module(sensor->module()), m_sensor(sensor)
    , m_name(name), m_address(address), m_size(size)
{
    update_hash();
}

void ModbusValue::setValues(const ValuesType &values) {
    qDebug()<<"ModbusValue::setValues:"<<name()<<values.size()<<values<<"; adr:"<<m_address;
    if(values.size()!=m_size) return;
    if(m_values == values) return;
    m_values = values;
    // отправить значение
    if(m_module) m_module->sendWrite(m_address, values);
    emit valuesChanged();
//    qDebug()<<"_ModbusValue::setValues";
}

void ModbusValue::updateValues(const ValuesType &values) {
    if(values.size()!=m_size) return;
//    m_countUpdate++;
//    qDebug()<<"ModbusValue::updateValues:"<<name()<<values.size()<<values<<"; adr:"<<m_address;
    if(m_values == values) return;
    m_values = values;
    m_countUpdate++;
    qDebug()<<"ModbusValue::updateValues:"<<name()<<values.size()<<values<<"; time:"<<QTime::currentTime().toString("ss.zzz");
    emit valuesChanged();
}

void ModbusValue::updateValues() {
    if(m_module) m_module->sendRead(m_address, m_size);
}

ValuesType ModbusValue::values() const {
    return m_values;
}

QVariant ModbusValue::value_var() const {
    switch (m_type) {
    case ValueType_INT16: return QVariant(value_int16());
    case ValueType_INT32: return QVariant(value_int32());
    case ValueType_FLOAT: return QVariant(value_float());
    default: return QVariant();
    }
}

void ModbusValue::setValue_int(int value) {
//    qDebug()<<"ModbusValue::setValue_int:"<<value<<QString("(%1, %2)").arg(m_values.size()).arg(m_size);
    if(m_size==1)
        setValue_int16(value);
    else if(m_size==2)
        setValue_uint32(value);
}

void ModbusValue::setValue_int16(qint16 value) {
    qDebug()<<"ModbusValue::setValue_int16:"<<value;
    if(m_size!=1) return;
    ValuesType values(1);
    values[0]=value;
    setValues(values);
}

void ModbusValue::setValue_uint32(quint32 value) {
//    qDebug()<<"ModbusValue::setValue_uint32:"<<value;
    if(m_size!=2) return;
    ValuesType values(2);
    values[0]=value & 0xffff;
    values[1]=value>>16;
    setValues(values);
//    setValues({3,0});
}

void ModbusValue::setValue_float(float value) {
//    qDebug()<<"ModbusValue::setValue_float:"<<value;
    if(m_size!=2) return;
    quint32 ivalue = (quint32&)value;
    ValuesType values(2);
    values[0]=ivalue & 0xffff;
    values[1]=ivalue>>16;
    setValues(values);
}

int ModbusValue::value_int() const {
    if(m_size==1)
        return value_int16();
    else if(m_size==2)
        return value_int32();
    return -1;
}

float ModbusValue::value_float() const {
    if(m_size<2 || m_values.size()<2) return -1.0;
    qint32 ivalue = (((qint32)m_values[1]<<16)|(qint32)m_values[0]);
    float fvalue=(float&)ivalue;
    return fvalue;
}

qint8 ModbusValue::value_int8() const {
    if(m_size<1 || m_values.size()<1) return -1;
    qint8 ivalue=(qint8)(m_values.value(0,0));
    return ivalue;
}

qint16 ModbusValue::value_int16() const {
    if(m_size<1 || m_values.size()<1) return -1;
    qint16 ivalue=(qint16)(m_values.value(0,0));
    return ivalue;
}

qint32 ModbusValue::value_int32() const {
    qint32 ivalue=-5;
    if(m_size==1 || m_values.size()<1) ivalue = (qint32)m_values.value(0, 0);
    else if(m_size==2) ivalue = (((qint32)m_values[1]<<16)|(qint32)m_values[0]);
    return ivalue;
}

#include <QCryptographicHash>
void ModbusValue::update_hash() {
    if(!m_module) return;
    auto v = this;
    QString summary, hash,
            value_name, sensor_name, module_name;
    int value_address, sensor_pin;
    value_name = v->name();
    value_address = v->address();
    module_name = v->module()->name();
    if(m_sensor){
        sensor_name = v->sensor()->name();
        sensor_pin = v->sensor()->pin();
        summary = module_name+"/"+QString::number(sensor_pin)+" - "+sensor_name+"/"+value_name;
    }else{
        summary = module_name+"/adr:"+QString::number(value_address);
    }
    m_hash = QCryptographicHash::hash(summary.toLatin1(), QCryptographicHash::Sha256).left(16);
    m_hash_str = m_hash.toHex();
}
