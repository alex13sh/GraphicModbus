#include "modbusdevice_analog.h"
#include "modbussensor.h"
#include "modbusvalue.h"

ModbusDevice_Analog::ModbusDevice_Analog(const QString &name, QObject *parent) :
    ModbusDevice(name, DeviceType::OVEN_InpoutAnalog, parent)
{
}

ModbusSensor *ModbusDevice_Analog::createSensor(quint8 pin, const QString &name) {
    if(not (pin>=1 && pin<=8)) return nullptr;

    ModbusSensor_Analog *sens = new ModbusSensor_Analog(name, pin, this);

    auto addValue = [this, sens](const QString &name, quint16 address, quint8 size, bool readOnly = false, const QString &desc = ""){
        ModbusValue *value;
        value = new ModbusValue(this, name, address, size);
        value->setReadOnly(readOnly);
        value->setDescription(desc);
        sens->addValue(address, value);
        this->m_values.insert(address, value);
        return value;
    };
    addValue("value_float", 4000+(pin-1)*3, 2, true);
    addValue("interval", 4000+(pin-1)*3+2, 1); // <<--
    addValue("value_int", 4064+(pin-1), 1, true);
    addValue("type", 4100+(pin-1)*16, 2, false, "Тип датчика");
//    addValue("полоса фильтра", 4102+(pin-1)*16, 1);
    addValue("point", 4103+(pin-1)*16, 1, "положение десятичной точки");

//    addValue("Сдвиг характеристик", 4104+(pin-1)*16, 2);
//    addValue("Наклон характеристик", 4106+(pin-1)*16, 2);
//    addValue("Верхняя граница", 4108+(pin-1)*16, 2);
//    addValue("Нижняя граница", 4110+(pin-1)*16, 2);

    addValue("interval", 4113+(pin-1)*16, 1); // <<--

    setSensor(pin, sens);
    return sens;
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
    else if(name == "value_float") {
        v_value_float = value;
        connect(v_value_float, &ModbusValue::valuesChanged, this, &ModbusSensor_Analog::value_float_changed);
    }else if(name == "value_int") v_value_int = value;
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
