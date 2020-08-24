#ifndef MODBUSSENSOR_H
#define MODBUSSENSOR_H

#include <QObject>
#include <QtQml>
#include "defines.h"

enum SensorType{
    OtherSensor=0,
    Temper,
    Davl
};

class ModbusDevice;
class ModbusSensor : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(float value READ value_float NOTIFY fvalueChanged)
    Q_PROPERTY(QString hash READ hash CONSTANT)

    Q_PROPERTY(float value_warn MEMBER m_value_warn CONSTANT)
    Q_PROPERTY(float value_err MEMBER m_value_err CONSTANT)
    Q_PROPERTY(float value_max MEMBER m_value_max NOTIFY value_maxChanged)
public:
    explicit ModbusSensor(QObject *parent = nullptr);

    ModbusSensor(const QString &name, quint8 pin, ModbusDevice *module);

    void setModule(ModbusDevice *m_module, int pin);
    const ModbusDevice *module() const {return m_module;}
//    void setType_(SensorType_ m_type);

    virtual void addValue(quint16 address, ModbusValue* value);
    ListValues values() const;
    void setInterval(quint16 interval);
    void setValueErr(float warn, float err);

    QString name() const {return m_name;}
    quint8 pin() const {return m_pin;}
    QString hash() const;

    void updateValue();
    float get_value_float();
    virtual float value_float() const = 0;
    float get_value_float_from_int(quint32 ivalue);
    virtual float value_float_from_int(quint32 ivalue);
    void setValueMax(float value);
//    QList<float> convert_value_list(const QList<quint32> &values);
signals:
    void fvalueChanged(/*float value*/);
    void value_maxChanged();
protected:

    QString m_name = "None";
    SensorType m_type = OtherSensor;
    const ModbusDevice *m_module=nullptr;

    quint8 m_pin;

    ListValues m_values;
    ModbusValue *v_value = nullptr, *v_interval=nullptr;
    QMap<QString, ModbusValue*> m_mapNameValue;
//    ListValues m_sensorValues, m_optionValues;
    float m_value_warn=-1, m_value_err=-1, m_value_max=0;

    friend class ModbusDevice;
};

#endif // MODBUSSENSOR_H
