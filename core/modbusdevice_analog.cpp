#include "modbusdevice_analog.h"
#include "modbussensor.h"
#include "modbusvalue.h"
#include <QDebug>
ModbusDevice_Analog::ModbusDevice_Analog(const QString &name, QObject *parent) :
    ModbusDevice(name, DeviceType::OVEN_InpoutAnalog, parent)
{
    v_adc = new ModbusValue(this, "adc_max", 4097, 1);
}

void ModbusDevice_Analog::setADC(bool enable){
    if(!v_adc) return;
    if(enable) v_adc->setValue_int16(1);
    else v_adc->setValue_int16(0);
}

ModbusSensor *ModbusDevice_Analog::createSensor(quint8 pin, const QString &name) {
    if(not (pin>=1 && pin<=8)) return nullptr;
    if(m_sensors.contains(pin)) return m_sensors.value(pin, nullptr);

    ModbusSensor_Analog *sens = new ModbusSensor_Analog(name, pin, this);
    initSensor(sens);
    setSensor(pin, sens);
    return sens;
}

void ModbusDevice_Analog::initSensor(ModbusSensor *sens) {
    auto pin = sens->pin();

    auto addValue = [this, sens](const QString &name, quint16 address, quint8 size, ValueType typ, bool readOnly = false, const QString &desc = ""){
        ModbusValue *value;
        value = new ModbusValue(sens, name, address, size);
        value->setReadOnly(readOnly);
        value->setDescription(desc);
        value->setType(typ);
        sens->addValue(address, value);
        this->m_values.insert(address, value);
        return value;
    };
    addValue("value_float", 4000+(pin-1)*3, 2, ValueType_FLOAT, true);
///    addValue("interval_read", 4002+(pin-1)*3, 1, ValueType_UINT16, true); // <<--
///    addValue("value_int", 4064+(pin-1), 1, ValueType_INT16, true);
    addValue("type", 4100+(pin-1)*16, 2, ValueType_UINT32, false, "Тип датчика");
//    addValue("полоса фильтра", 4102+(pin-1)*16, 1);
    addValue("point", 4103+(pin-1)*16, 1, ValueType_UINT16, false, "положение десятичной точки");

//    addValue("Сдвиг характеристик", 4104+(pin-1)*16, 2);
//    addValue("Наклон характеристик", 4106+(pin-1)*16, 2);
    addValue("Верхняя граница", 4108+(pin-1)*16, 2, ValueType_FLOAT);
    addValue("Нижняя граница", 4110+(pin-1)*16, 2, ValueType_FLOAT);

    addValue("interval", 4113+(pin-1)*16, 1, ValueType_UINT16, false); // <<--
}

//void ModbusDevice_Analog::getValues(quint16 adr, ValuesType values) {
//    ModbusDevice::getValues(adr, values);
//    return;
////    if(adr>=4000 && adr<=4071)
////        updateValueSensor(adr, values);
//}

ModbusSensor_Analog::ModbusSensor_Analog(const QString &name, quint8 pin, ModbusDevice *module)
    : ModbusSensor(name, pin, module)
{

}

void ModbusSensor_Analog::setType(ModbusSensor_Analog::SensorType typ) {
//    ModbusValue *v_type = m_mapNameValue.value("type", nullptr);
    if(!v_type) return;
    v_type->setValue_uint32(quint32(typ));
    m_type = typ;
}

void ModbusSensor_Analog::addValue(quint16 address, ModbusValue *value) {
    ModbusSensor::addValue(address, value);
    auto name = value->name();
    if(name == "type") v_type = value;
    else if(name=="interval") v_interval=value;
    else if(name == "value_float") {
        v_value_float = value;
        v_value = value;
        connect(v_value_float, &ModbusValue::valuesChanged, this, &ModbusSensor_Analog::value_float_changed);
        connect(v_value_float, &ModbusValue::valuesChanged, this, &ModbusSensor::fvalueChanged);
    }else if(name == "value_int") v_value_int = value;
    else if(name=="Нижняя граница") v_ain_l = value;
    else if(name=="Верхняя граница") v_ain_h = value;
}

void ModbusSensor_Analog::setRange(float l, float h){
    if(!v_ain_h || !v_ain_l) return;
    v_ain_l->setValue_float(l);
    v_ain_h->setValue_float(h);
}

qint16 ModbusSensor_Analog::value_int16() const {
//    auto v_value_int = m_mapNameValue.value("value_int", nullptr);
    if(v_value_int) return v_value_int->value_int16();
    return 0;
}

float ModbusSensor_Analog::value_float() const {
    if(v_value_float) return v_value_float->value_float();
    return 0;
}
