#include "modbusdevice_analog.h"
#include "modbussensor.h"
#include "modbusvalue.h"

ModbusDevice_Analog::ModbusDevice_Analog(const QString &name, QObject *parent) :
    ModbusDevice(name, DeviceType::OVEN_InpoutAnalog, parent)
{
}

ModbusSensor *ModbusDevice_Analog::createSensor(quint8 pin, const QString &name) {
    if(not (pin>=1 && pin<=8)) return nullptr;

    ModbusSensor *sens = new ModbusSensor(name, pin, this);

    auto addValue = [this, sens](const QString &name, quint16 address, quint8 size, bool readOnly = false){
        ModbusValue *value;
        value = new ModbusValue(this, name, address, size);
        value->setReadOnly(readOnly);
        sens->addValue(address, value);
        this->m_values.insert(address, value);
        return value;
    };
    sens->value_float = addValue("value_float", 4000+(pin-1)*3, 2, true);
    addValue("interval", 4000+(pin-1)*3+2, 1); // <<--
    sens->value_int = addValue("value_int", 4064+(pin-1), 1, true);
    addValue("type", 4100+(pin-1)*16, 2);
//    addValue("полоса фильтра", 4102+(pin-1)*16, 1);
    addValue("положение десятичной точки", 4103+(pin-1)*16, 1);

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
