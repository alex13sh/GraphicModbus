#include "modbusdevice_iodigital.h"
#include "modbussensor.h"
#include "modbusvalue.h"

ModbusDevice_IODigital::ModbusDevice_IODigital(const QString &name, QObject *parent)
    : ModbusDevice(name, DeviceType::OVEN_IODiget, parent)
{

}

ModbusSensor *ModbusDevice_IODigital::createSensor(quint8 pin, const QString &name) {
    if(not (pin>=1 && pin<=12)) return nullptr;

    ModbusSensor *sens = new ModbusSensor_IODigital(name, pin, this);

    auto addValue = [this, sens](const QString &name, quint16 address, quint8 size, bool readOnly = false, const QString &desc = ""){
        ModbusValue *value;
        value = new ModbusValue(sens, name, address, size);
        value->setReadOnly(readOnly);
        value->setDescription(desc);
        sens->addValue(address, value);
        this->m_values.insert(address, value);
        return value;
    };

    if(pin>=1 && pin<=8)  addValue("type_input" , 64 +(pin-1), 1, false, "Дополнительный режим"); // 1 - 8
    if(pin>=1 && pin<=12) addValue("filter", 96 +(pin-1), 1, false, "Фильтр"); // 1 - 12
    if(pin>=1 && pin<=8)  addValue("interval", 128 +(pin-1), 1, false, "Период измерения частоты"); // 1 - 8
    if(pin>=1 && pin<=12) addValue("value", 160 +(pin-1)*2, 2, true, "Значение входа в дополнительном режиме"); // 1 - 12

    if(pin>=1 && pin<=8)  addValue("reset_value", 224 +(pin-1)*1, 1, false, "Сброс значения дополнительного режима"); // 1 - 8
    if(pin>=9 && pin<=12) addValue("reset_counter", 232 +(pin-1)*1, 1, false, "Сброс значения счётчика импульсв"); // 9 - 12

    if(pin>=1 && pin<=4)  addValue("type_output", 272 +(pin-1)*1, 1, false, "Режим работы выхода"); // 1 - 4
    if(pin>=1 && pin<=4)  addValue("Период ШИМ", 308 +(pin-1)*1, 1); // 1 - 4
    if(pin>=1 && pin<=4)  addValue("Коэффициент заполнения ШИМ", 340 +(pin-1)*1, 1); // 1 - 4

    if(pin>=1 && pin<=4)  addValue("Безопасное состояние выхода", 474 +(pin-1)*1, 1); // 1 - 4
    return sens;
}

ModbusSensor_IODigital::ModbusSensor_IODigital(const QString &name, quint8 pin, ModbusDevice *module)
    : ModbusSensor(name, pin, module)
{
}

void ModbusSensor_IODigital::addValue(quint16 address, ModbusValue *value) {
    ModbusSensor::addValue(address, value);
    auto name = value->name();
    if(name == "type_input") v_type_input = value;
    else if(name == "type_output") v_type_output = value;
    else if(name == "value") {
        v_value = value;
        connect(v_value, &ModbusValue::valuesChanged, this, &ModbusSensor_IODigital::valueChanged);
    }
}

void ModbusSensor_IODigital::setTypeInput(ModbusSensor_IODigital::SensorInputType type) {
    if(!v_type_input) return;
    v_type_input->setValue_int16(qint16(type));
    m_type_input = type;
}

int ModbusSensor_IODigital::value() const {
    if(v_value) return v_value->value_int();
    return 0;
}
