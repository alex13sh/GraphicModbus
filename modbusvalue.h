#ifndef MODBUSVALUE_H
#define MODBUSVALUE_H

#include <QObject>
#include "defines.h"

class QTimer;

enum ValueType{
    ValueType_None,
    ValueType_INT8,
    ValueType_INT16,
    ValueType_INT32,
    ValueType_FLOAT,
    Датчик,
    Ошибка,
    Найстройка //Запись
};

class ModbusValue : public QObject
{
    Q_OBJECT
public:
    explicit ModbusValue(ModbusDevice *module, const QString &name, quint16 address, quint8 size);

    void setType(ValueType type);

    void setValues(const ValuesType &values); // Установить новые значения
    void updateValues(const ValuesType &values); // Обновить
    void updateValues();
    ValuesType values() const;

    float value_float() const;
    int value_int() const;
signals:
    void valuesChanged();

protected:
    ModbusDevice *m_module = nullptr;
    QString m_name;
    quint16 m_address;
    quint8 m_size;
    ValueType m_type;

    QTimer *m_updateValue;
    ValuesType m_values;
    quint32 m_ivalue;
    float m_fvalue; // может быть эти значения не будут хранится
};

class ModbusValueFloat : public ModbusValue
{

public:
    explicit ModbusValueFloat(ModbusDevice *module, const QString &name, quint16 address, quint8 size);

    void setValue(float value);
    float value() const;
};

#endif // MODBUSVALUE_H
