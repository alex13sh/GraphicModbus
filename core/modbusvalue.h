#ifndef MODBUSVALUE_H
#define MODBUSVALUE_H

#include <QObject>
#include "defines.h"

class QTimer;

enum ValueType{
    ValueType_None,
    ValueType_INT8,
    ValueType_INT16,
    ValueType_UINT16,
    ValueType_INT32,
    ValueType_UINT32,
    ValueType_FLOAT
};

class ModbusValue : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool readOnly READ readOnly WRITE setReadOnly)
    Q_PROPERTY(int value READ value_int NOTIFY valuesChanged)
public:
    explicit ModbusValue(ModbusDevice *module, const QString &name, quint16 address, quint8 size);
    explicit ModbusValue(ModbusSensor *sensor, const QString &name, quint16 address, quint8 size);

    QString name() const {return m_name;}
    void setDescription(const QString &desc) {m_description=desc;}
    QString description() const {return m_description;}
//    void setType(ValueType type);
    void setReadOnly(bool readOnly){m_readOnly=readOnly;}
    bool readOnly() const {return m_readOnly;}
    void setType(ValueType type) {m_type=type;}
//    void setFloatType(bool floatType) {m_type = ValueType_FLOAT;}
    bool isFloatType() const {return m_type==ValueType_FLOAT;}


    const ModbusDevice *module() const {return m_module;}
    const ModbusSensor *sensor() const {return m_sensor;}

    void setValues(const ValuesType &values); // Установить новые значения
    void updateValues(const ValuesType &values); // Обновить
    void updateValues();
    ValuesType values() const;
    quint32 countUpdate() const {return m_countUpdate;}

    void setValue_int(int value);
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
    QString m_name, m_description;
    quint16 m_address;
    quint8 m_size;
    ValueType m_type;
    bool m_readOnly=false, m_floatType=false; // or ValueType::ValueType_FLOAT
    quint32 m_countUpdate = 0;

    QTimer *m_updateValue;
    ValuesType m_values;
};

#endif // MODBUSVALUE_H
