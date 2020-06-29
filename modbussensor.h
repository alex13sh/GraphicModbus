#ifndef MODBUSSENSOR_H
#define MODBUSSENSOR_H

#include <QObject>
#include "defines.h"

enum SensorType_{
    None=0,
    Volt_1=14,
    Resister=39
};
enum SensorType{
    OtherSensor=0,
    Температура,
    Давление
};

class ModbusDevice;
class ModbusSensor : public QObject
{
    Q_OBJECT
public:
    explicit ModbusSensor(QObject *parent = nullptr);

    ModbusSensor(const QString &name, quint8 pin, ModbusDevice *module);

    void setModule(ModbusDevice *m_module, int pin);
    void setType_(SensorType_ m_type);

    void addValue(quint16 address, ModbusValue* value);
    ListValues values() const;

    QString name() const {return m_name;}
    quint8 pin() const {return m_pin;}

    void updateValue();
    void setValueF(float value);

signals:
    void fvalueChanged(float value);
private:

    QString m_name = "None";
    SensorType_ m_type_ = None;
    SensorType m_type = OtherSensor;
    const ModbusDevice *m_module=nullptr;

    quint8 m_pin;

    float m_fvalue;
    int m_ivalue;

    ListValues m_values;

    friend class ModbusDevice;
};

#endif // MODBUSSENSOR_H
