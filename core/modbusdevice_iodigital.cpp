#include "modbusdevice_iodigital.h"
#include "modbussensor.h"
#include "modbusvalue.h"
#include <QDebug>

ModbusDevice_IODigital::ModbusDevice_IODigital(const QString &name, QObject *parent)
    : ModbusDevice(name, DeviceType::OVEN_IODiget, parent)
{

}

ModbusSensor *ModbusDevice_IODigital::createSensor(quint8 pin, const QString &name) {
    if(not (pin>=1 && pin<=12)) return nullptr;

    ModbusSensor *sens = new ModbusSensor_IDigital(name, pin, this);
    initSensor(sens);
    setSensor(pin, sens);
    return sens;
}

void ModbusDevice_IODigital::initSensor(ModbusSensor *sens) {
    auto pin = sens->pin();
    bool output = dynamic_cast<ModbusSensor_ODigital*>(sens)!=0;
    auto addValue = [this, sens](const QString &name, quint16 address, quint8 size, ValueType typ, bool readOnly = false, const QString &desc = ""){
        ModbusValue *value;
        value = new ModbusValue(sens, name, address, size);
        value->setReadOnly(readOnly);
        value->setType(typ);
        value->setDescription(desc);
        sens->addValue(address, value);
        this->m_values.insert(address, value);
        return value;
    };

    if(pin>=1 && pin<=8 && !output)  addValue("type_input" , 64 +(pin-1), 1, ValueType_UINT16, false, "Дополнительный режим"); // 1 - 8
    if(pin>=1 && pin<=12) addValue("filter", 96 +(pin-1), 1, ValueType_UINT16, false, "Фильтр"); // 1 - 12
    if(pin>=1 && pin<=8 && !output)  addValue("interval", 128 +(pin-1), 1, ValueType_UINT16, false, "Период измерения частоты"); // 1 - 8
    if(pin>=1 && pin<=12 && !output) addValue("value", 160 +(pin-1)*2, 2, ValueType_UINT32, true, "Значение входа в дополнительном режиме"); // 1 - 12

    if(pin>=1 && pin<=8)  addValue("reset_value", 224 +(pin-1)*1, 1, ValueType_UINT16, false, "Сброс значения дополнительного режима"); // 1 - 8
    if(pin>=9 && pin<=12) addValue("reset_counter", 232 +(pin-1)*1, 1, ValueType_UINT16, false, "Сброс значения счётчика импульсв"); // 9 - 12

    if(pin>=1 && pin<=4 && output)  addValue("type_output", 272 +(pin-1)*1, 1, ValueType_UINT16, false, "Режим работы выхода"); // 1 - 4
    if(pin>=1 && pin<=4 && output)  addValue("Период ШИМ", 308 +(pin-1)*1, 1, ValueType_UINT16); // 1 - 4
    if(pin>=1 && pin<=4 && output)  addValue("Коэффициент заполнения ШИМ", 340 +(pin-1)*1, 1, ValueType_UINT16); // 1 - 4

    if(pin>=1 && pin<=4 && output)  addValue("Безопасное состояние выхода", 474 +(pin-1)*1, 1, ValueType_UINT16); // 1 - 4

    if(pin>=1 && pin<=4 && output)  addValue("Битовая маска состояния", 468, 1, ValueType_UINT8, true); // 1 - 4
    if(pin>=1 && pin<=4 && output) {
        if(!v_bit_setup){
//            v_bit_setup = addValue("Битовая маска установки", 470, 1, ValueType_UINT8); // 1 - 4
            v_bit_setup = new ModbusValue(this, "Битовая маска установки", 470, 1);
            v_bit_setup->setType(ValueType_UINT8);
            this->m_values.insert(v_bit_setup->address(), v_bit_setup);
        }
        sens->addValue(v_bit_setup->address(), v_bit_setup);
    }
}

void ModbusDevice_IODigital::updateValues()
{
    sendRead(160, 24);
}

ModbusSensor_IDigital::ModbusSensor_IDigital(const QString &name, quint8 pin, ModbusDevice *module)
    : ModbusSensor(name, pin, module)
{
}

void ModbusSensor_IDigital::addValue(quint16 address, ModbusValue *value) {
    ModbusSensor::addValue(address, value);
    auto name = value->name();
    if(name == "type_input") v_type_input = value;
//    else if(name == "type_output") v_type_output = value;
    else if(name == "value") {
        v_value = value;
        connect(v_value, &ModbusValue::valuesChanged, this, &ModbusSensor_IDigital::valueChanged);
    }
}

void ModbusSensor_IDigital::setTypeInput(ModbusSensor_IDigital::SensorInputType type) {
    if(!v_type_input) return;
    v_type_input->setValue_int16(qint16(type));
    m_type_input = type;
}

int ModbusSensor_IDigital::value() const {
    if(v_value) return v_value->value_int();
    return 0;
}

float ModbusSensor_IDigital::value_float() const
{
    if(v_value) {
        if(v_value->type() == ValueType_FLOAT) return v_value->value_float();
        else if(v_value->type() == ValueType_UINT32) return v_value->value_int32();
    }
    return -2.0;
}

ModbusSensor_ODigital::ModbusSensor_ODigital(const QString &name, quint8 pin, ModbusDevice *module)
    : ModbusSensor(name, pin, module)
{

}

void ModbusSensor_ODigital::addValue(quint16 address, ModbusValue *value) {
    ModbusSensor::addValue(address, value);
    auto name = value->name();

    if(name == "type_output") v_type_output = value;
    else if(name == "value") {
        v_value = value;
//        connect(v_value, &ModbusValue::valuesChanged, this, &ModbusSensor_IDigital::valueChanged);
    } else if (name == "Битовая маска состояния")
        v_bit_state = value;
    else if (name == "Битовая маска установки") {
        v_bit_setup = value;
        v_logic = new ModbusValue((ModbusDevice*)0, "Logic out", m_pin, 1);
        connect(v_logic, &ModbusValue::valuesChanged, [this](){
            qDebug()<<"Set logic ("<<v_logic->address()<<") = "<<v_logic->value_int8();
            this->setLogicLevel(v_logic->value_int8());
        });
        this->m_values.insert(228+m_pin, v_logic);
    }
}

void ModbusSensor_ODigital::setTypeOutput(ModbusSensor_ODigital::SensorOutputType type) {

}

void ModbusSensor_ODigital::setLogicLevel(bool lvl) {
    if(!v_bit_setup) return;
    quint8 v = (quint8) v_bit_setup->value_int8();
    auto pin = m_pin-1;
    if(lvl) v |= 1<<pin;
    else v &= ~(1<<pin);
    v_bit_setup->setValue_int16(v);
    qDebug()<<"ModbusSensor_ODigital::setLogicLevel:"<<v;
}

int ModbusSensor_ODigital::value() const
{
    return 0;
}

float ModbusSensor_ODigital::value_float() const
{
    return 0.0;
}
