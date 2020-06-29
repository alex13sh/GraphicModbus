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

    auto addValue = [this, sens](const QString &name, quint16 address, quint8 size){
        ModbusValue *value;
        value = new ModbusValue(this, name, address, size);
        sens->addValue(address, value);
        this->m_values.insert(address, value);
    };
    addValue("value_float", 4000+(pin-1)*3, 2);
    addValue("interval", 4000+(pin-1)*3+2, 1); // <<--
    addValue("value_int", 4064+(pin-1), 1);
    addValue("type", 4100+(pin-1)*16, 2);
    addValue("полоса фильтра", 4102+(pin-1)*16, 1);
    addValue("положение десятичной точки", 4103+(pin-1)*16, 1);

    addValue("Сдвиг характеристик", 4104+(pin-1)*16, 2);
    addValue("Наклон характеристик", 4106+(pin-1)*16, 2);
    addValue("Верхняя граница", 4108+(pin-1)*16, 2);
    addValue("Нижняя граница", 4110+(pin-1)*16, 2);

    addValue("interval", 4113+(pin-1)*16, 1); // <<--

    setSensor(pin, sens);
    return sens;
}

void ModbusDevice_Analog::getValues(quint16 adr, ValuesType values) {
    if(adr>=4000 && adr<=4071)
        updateValueSensor(adr, values);
}

#include "modbussensor.h"
void ModbusDevice_Analog::updateValueSensor(quint16 adr, ValuesType value) {
    if(adr>=4000 && adr<4064){
        qint32 ivalue = (((qint32)value[1]<<16)|(qint32)value[0]);
        float fvalue=(float&)ivalue;
        quint8 pin = (adr-4000)/3+1;
//        m_sensors[pin]->m_fvalue = fvalue;
        m_sensors[pin]->setValueF(fvalue);
    }else if(adr>=4064 && adr<=4071){
        qint16 ivalue=(quint16)(value[0]);
        quint8 pin = (adr-4064)/1+1;
//        m_sensors[pin]->m_ivalue = ivalue;
    }
}

void ModbusDevice_Analog::getValueSensor(quint8 pin) const {
    // Возможно эта хрень уже и не нужна))
    sendRead(4000+(pin-1)*3, 2); // float value
    sendRead(4064+(pin-1), 1);  // int value
}
