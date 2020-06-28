#include "modbusvalue.h"
#include "modbusdevice.h"

#include <QTimer>

ModbusValue::ModbusValue(ModbusDevice *module, const QString &name, quint16 address, quint8 size)
    : QObject(module)
    , m_module(module)
    , m_name(name), m_address(address), m_size(size)
{
    m_updateValue = new QTimer(this);

}

void ModbusValue::setValues(const ValuesType &values) {
    m_values = values;
}

ValuesType ModbusValue::values() const {
    return m_values;
}
