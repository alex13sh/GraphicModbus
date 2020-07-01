#ifndef MODBUSDEVICE_ANALOG_H
#define MODBUSDEVICE_ANALOG_H

#include "modbusdevice.h"

class ModbusDevice_Analog : public ModbusDevice
{
    Q_OBJECT
public:
    explicit ModbusDevice_Analog(const QString &name, QObject *parent = nullptr);

    ModbusSensor *createSensor(quint8 pin, const QString &name) override;
signals:

private:

//    void getValues(quint16 adr, ValuesType values) override;
//    void updateValueSensor(quint16 adr, ValuesType value) override;
//    void getValueSensor(quint8 pin) const override;
};

#include "modbussensor.h"

class ModbusSensor_Analog : public ModbusSensor
{
    Q_OBJECT
    Q_PROPERTY(float value_float READ value_float NOTIFY value_float_changed)
public:
    enum SensorType{
        None=0,
        Volt_1=14,
        Resister=39
    };
    ModbusSensor_Analog(const QString &name, quint8 pin, ModbusDevice *module);
    void setType(SensorType typ);
    void addValue(quint16 address, ModbusValue *value) override;

    qint16 value_int16() const;
    float value_float() const;

signals:
    void value_float_changed();
private:
    SensorType m_type;
    ModbusValue *v_type=nullptr,
      *v_value_float=nullptr, *v_value_int=nullptr;
};

#endif // MODBUSDEVICE_ANALOG_H
