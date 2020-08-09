#ifndef MODBUSSENSOR_H
#define MODBUSSENSOR_H

#include <QObject>
#include <QtQml>
#include "defines.h"

enum SensorType{
    OtherSensor=0,
    Температура,
    Давление
};

class ModbusDevice;
class ModbusSensor : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString name READ name FINAL)
    Q_PROPERTY(float value READ value_float NOTIFY fvalueChanged)
    Q_PROPERTY(QString hash READ hash CONSTANT)
public:
    explicit ModbusSensor(QObject *parent = nullptr);

    ModbusSensor(const QString &name, quint8 pin, ModbusDevice *module);

    void setModule(ModbusDevice *m_module, int pin);
    const ModbusDevice *module() const {return m_module;}
//    void setType_(SensorType_ m_type);

    virtual void addValue(quint16 address, ModbusValue* value);
    ListValues values() const;
    void setInterval(quint16 interval);

    QString name() const {return m_name;}
    quint8 pin() const {return m_pin;}
    QString hash() const;

    void updateValue();
    virtual float value_float() const = 0;
signals:
    void fvalueChanged(/*float value*/);

protected:

    QString m_name = "None";
    SensorType m_type = OtherSensor;
    const ModbusDevice *m_module=nullptr;

    quint8 m_pin;

    ListValues m_values;
    ModbusValue *v_value = nullptr, *v_interval=nullptr;
    QMap<QString, ModbusValue*> m_mapNameValue;
//    ListValues m_sensorValues, m_optionValues;

    friend class ModbusDevice;
};

#endif // MODBUSSENSOR_H
