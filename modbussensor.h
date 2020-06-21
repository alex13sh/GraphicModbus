#ifndef MODBUSSENSOR_H
#define MODBUSSENSOR_H

#include <QObject>

class ModbusDevice;
class ModbusSensor : public QObject
{
    Q_OBJECT
public:
    explicit ModbusSensor(QObject *parent = nullptr);

    ModbusSensor(const QString &name, quint16 address, QObject *parent = nullptr);

    enum SensorType_{
        None=0,
        Volt_1=14,
        Resister=39
    };
    enum SensorType{
        Other=0,
        Температура,
        Давление
    };

    void setModule(const ModbusDevice *module){this->module=module;}
    void setType_(SensorType_ type){type_=type;}

    void updateValue();

signals:

private:

    QString name = "None";
    SensorType_ type_ = None;
    SensorType type = Other;
    const ModbusDevice *module=nullptr;
    quint16 address;

    float fvalue;
    int ivalue;

};

#endif // MODBUSSENSOR_H
