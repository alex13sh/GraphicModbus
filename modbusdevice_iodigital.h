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

#include "modbussensor.h"

class ModbusSensor_IODigital : public ModbusSensor
{
    Q_OBJECT
    Q_PROPERTY(int value READ value NOTIFY valueChanged)
public:
    ModbusSensor_IODigital(const QString &name, quint8 pin, ModbusDevice *module);
//    void setType(SensorType typ);
    void addValue(quint16 address, ModbusValue *value) override;

    enum SensorInputType{
        None=0,
        Period,
        CounterImpuls,
        InputHZ,
        Encoder
    };
    void setTypeInput(SensorInputType type);
    SensorInputType typeInput() const {return m_type_input;}
    int value() const;

signals:
    void valueChanged();
private:
    SensorInputType m_type_input;
    ModbusValue *v_type_input=nullptr, *v_type_output=nullptr,
      *v_value=nullptr;
};

#endif // MODBUSDEVICE_IODIGITAL_H
