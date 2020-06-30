#include "modbusdevice_iodigital.h"
#include "modbussensor.h"
#include "modbusvalue.h"

ModbusDevice_IODigital::ModbusDevice_IODigital(const QString &name, QObject *parent)
    : ModbusDevice(name, DeviceType::OVEN_IODiget, parent)
{

}

ModbusSensor *ModbusDevice_IODigital::createSensor(quint8 pin, const QString &name) {
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

    addValue("Дополнительный режим", 64 +(pin-1), 1); // 1 - 8
    addValue("Фильтр", 96 +(pin-1), 1); // 1 - 12
    addValue("Период измерения частоты", 128 +(pin-1), 1); // 1 - 8
    sens->value_int = addValue("Значение входа в дополнительном режиме", 160 +(pin-1)*2, 2, true); // 1 - 12

    addValue("Сброс значения дополнительного режима", 224 +(pin-1)*1, 1); // 1 - 8
    addValue("Сброс значения счётчика импульсв", 232 +(pin-1)*1, 1); // 9 - 12

    addValue("Режим работы выхода", 272 +(pin-1)*1, 1); // 1 - 4
    addValue("Период ШИМ", 308 +(pin-1)*1, 1); // 1 - 4
    addValue("Коэффициент заполнения ШИМ", 340 +(pin-1)*1, 1); // 1 - 4

    addValue("Безопасное состояние выхода", 474 +(pin-1)*1, 1); // 1 - 4
    return sens;
}
