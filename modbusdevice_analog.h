#ifndef MODBUSDEVICE_ANALOG_H
#define MODBUSDEVICE_ANALOG_H

#include "modbusdevice.h"

class ModbusDevice_Analog : public ModbusDevice
{
    Q_OBJECT
public:
    explicit ModbusDevice_Analog(const QString &name, QObject *parent = nullptr);

signals:

private:

    void getValues(quint16 adr, ValuesType values) override;
    void updateValueSensor(quint16 adr, ValuesType value) override;
    void getValueSensor(quint8 pin) const override;
};

#endif // MODBUSDEVICE_ANALOG_H