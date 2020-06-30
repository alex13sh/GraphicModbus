#ifndef MODBUSVALUE_H
#define MODBUSVALUE_H

#include <QObject>
#include "defines.h"

class QTimer;

enum ValueType_{
    ValueType_None,
    ValueType_INT8,
    ValueType_INT16,
    ValueType_INT32,
    ValueType_FLOAT
};
enum ValueType{
    Sensor, Option, Critical
};

class ModbusValue : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool readOnly READ readOnly WRITE setReadOnly)
public:
    explicit ModbusValue(ModbusDevice *module, const QString &name, quint16 address, quint8 size);
    explicit ModbusValue(ModbusSensor *sensor, const QString &name, quint16 address, quint8 size);

    QString name() const {return m_name;}
//    void setType(ValueType type);
    void setReadOnly(bool readOnly){m_readOnly=readOnly;}
    bool readOnly() const {return m_readOnly;}

    void setValues(const ValuesType &values); // Установить новые значения
    void updateValues(const ValuesType &values); // Обновить
    void updateValues();
    ValuesType values() const;

    void setValue_int16(qint16 value);
    void setValue_uint32(quint32 value);
    void setValue_float(float value);
    int value_int() const;
    float value_float() const;
    qint8 value_int8() const;
    qint16 value_int16() const;
    qint32 value_int32() const;

signals:
    void valuesChanged();

protected:
    const ModbusDevice *m_module = nullptr;
    ModbusSensor *m_sensor = nullptr;
    QString m_name;
    quint16 m_address;
    quint8 m_size;
    ValueType m_type;
    bool m_readOnly;

    QTimer *m_updateValue;
    ValuesType m_values;
};

#endif // MODBUSVALUE_H
