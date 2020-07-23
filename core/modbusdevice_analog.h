#ifndef MODBUSDEVICE_ANALOG_H
#define MODBUSDEVICE_ANALOG_H

#include "modbusdevice.h"

class ModbusDevice_Analog : public ModbusDevice
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit ModbusDevice_Analog(const QString &name, QObject *parent = nullptr);

    void setADC(bool enable);

    ModbusSensor *createSensor(quint8 pin, const QString &name) override;
    void initSensor(ModbusSensor *sens) override;
signals:

private:
     ModbusValue *v_adc=nullptr;
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

        Amper_4_20=11,
        Amper_0_20,
        Amper_0_5,

        Volt_1=14,
        Resister_0_2=38,
        Resister_0_5=39,

        Pt_50=8,
        Pt_100=3,
        Pt_500=30,
        Pt_1000=35,

        Cu_50=2,
        Cu_100=1,
        Cu_500=28,
        Cu_1000=33
    };
    ModbusSensor_Analog(const QString &name, quint8 pin, ModbusDevice *module);
    void setType(SensorType typ);
    void addValue(quint16 address, ModbusValue *value) override;

    void setRange(float l, float h);

    virtual qint16 value_int16() const;
    virtual float value_float() const;

    ModbusValue *getValueFloat()const{return v_value_float;}

signals:
    void value_float_changed();
private:
    SensorType m_type;
    ModbusValue *v_type=nullptr,
      *v_value_float=nullptr, *v_value_int=nullptr,
      *v_ain_l=nullptr, *v_ain_h=nullptr;
};

class ModbusSensor_Analog_Perometr : public ModbusSensor_Analog
{
public:
    ModbusSensor_Analog_Perometr(const QString &name, quint8 pin, ModbusDevice *module)
        : ModbusSensor_Analog(name, pin, module)
    {
        setType(Pt_100);
    }
    virtual float value_float() const
    {
        // от 0 до 250 градусов
        // 4 - 20 мА
        float v = ModbusSensor_Analog::value_float();
//        v = v
        return v;
    }
};
class ModbusSensor_Analog_Vibra : public ModbusSensor_Analog
{
public:
    ModbusSensor_Analog_Vibra(const QString &name, quint8 pin, ModbusDevice *module);
    virtual float value_float() const
    {
        // от 0 до 25 мм/с
        // 4 - 20 мА
        float v = ModbusSensor_Analog::value_float();

        return v;
    }
};
#include <math.h>
class ModbusSensor_Analog_Davl : public ModbusSensor_Analog
{
public:
    ModbusSensor_Analog_Davl(const QString &name, quint8 pin, ModbusDevice *module)
        : ModbusSensor_Analog(name, pin, module)
    {
//        setType(Amper_4_20);
//        setRange(-100.0, 100.0);
    }
    virtual float value_float() const
    {
        // от 0 до 25 мм/с
        // 4 - 20 мА
        float v = ModbusSensor_Analog::value_float();
        v = pow(10, v*10-5.5);
        return v;
    }
};

#endif // MODBUSDEVICE_ANALOG_H
