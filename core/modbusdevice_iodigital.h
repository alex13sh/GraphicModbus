#ifndef MODBUSDEVICE_IODIGITAL_H
#define MODBUSDEVICE_IODIGITAL_H

#include "modbusdevice.h"

class ModbusDevice_IODigital : public ModbusDevice
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit ModbusDevice_IODigital(const QString &name, QObject *parent = nullptr);

    ModbusSensor *createSensor(quint8 pin, const QString &name) override;
    void initSensor(ModbusSensor *sens) override;

    void updateValues() override;
signals:

private:
    ModbusValue *v_bit_setup=nullptr;
//    void getValues(quint16 adr, ValuesType values) override;
//    void updateValueSensor(quint16 adr, ValuesType value) override;
//    void getValueSensor(quint8 pin) const override;

};

#include "modbussensor.h"

class ModbusSensor_IDigital : public ModbusSensor
{
    Q_OBJECT
    Q_PROPERTY(int value READ value NOTIFY valueChanged)
public:
    ModbusSensor_IDigital(const QString &name, quint8 pin, ModbusDevice *module);
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
    virtual float value_float() const;

signals:
    void valueChanged();
private:
    SensorInputType m_type_input;
    ModbusValue *v_type_input=nullptr;
    //  *v_value=nullptr;
};

class ModbusSensor_ODigital : public ModbusSensor
{
    Q_OBJECT
    Q_PROPERTY(int value READ value NOTIFY valueChanged)
public:
    ModbusSensor_ODigital(const QString &name, quint8 pin, ModbusDevice *module);
//    void setType(SensorType typ);
    void addValue(quint16 address, ModbusValue *value) override;

    enum SensorOutputType{
        LogicSignal,
        SHIM
    };
    void setTypeOutput(SensorOutputType type);
    SensorOutputType typeOutput() const {return m_type_output;}

    void setLogicLevel(bool lvl);
    int value() const;
    virtual float value_float() const;

signals:
    void valueChanged();
private:
    SensorOutputType m_type_output;
    ModbusValue *v_type_output=nullptr,
      *v_value=nullptr,
      *v_bit_state=nullptr, *v_bit_setup=nullptr;
    ModbusValue *v_logic = nullptr;
};

#endif // MODBUSDEVICE_IODIGITAL_H
