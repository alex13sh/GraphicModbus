#ifndef MODBUSDEVICE_IODIGITAL_H
#define MODBUSDEVICE_IODIGITAL_H

#include "modbusdevice.h"

class ModbusDevice_IODigital : public ModbusDevice
{
    Q_OBJECT
public:
    explicit ModbusDevice_IODigital(const QString &name, QObject *parent = nullptr);

    ModbusSensor *createSensor(quint8 pin, const QString &name) override;

signals:

private:

//    void getValues(quint16 adr, ValuesType values) override;
//    void updateValueSensor(quint16 adr, ValuesType value) override;
//    void getValueSensor(quint8 pin) const override;

};

#endif // MODBUSDEVICE_IODIGITAL_H
