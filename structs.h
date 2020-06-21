#ifndef STRUCTS_H
#define STRUCTS_H

#include <QMap>

class QModbusClient;
struct ModbusDevice{
    QString name = "None";
    enum DeviceType{
        Other,
        OVEN_InpoutAnalog,
        OVEN_IODiget
    } type = Other;
    QModbusClient *device=nullptr;
    ModbusDevice()
        : name("None"), device(nullptr), type(Other)
    {}
    ModbusDevice(const QString &name, QModbusClient *device, DeviceType type)
        : name(name), device(device), type(type)
    {}
    QString typeStr() const{
        switch (type) {
        case OVEN_InpoutAnalog: return "Input Analog";
        case OVEN_IODiget: return "IO Digital";
        default: return "Other";
        }
    }
};
typedef QMap<QString, ModbusDevice>* MapDevices;
typedef QMap<QString, ModbusDevice> MapDevices_;

struct Sensor{
    QString name = "None";
    enum SensorType_{
        None=0,
        Volt_1=14,
        Resister=39
    } type_ = None;
    enum SensorType{
        Other=0,
        Температура,
        Давление
    } type = Other;
    const ModbusDevice *module=nullptr;
    quint16 address;

    float fvalue;
    int ivalue;

    Sensor()
        : name("None")
    {}
    Sensor(const QString &name, quint16 address)
        : name(name), address(address)
    {}

    void setModule(const ModbusDevice *module){this->module=module;}
    void setType_(SensorType_ type){type_=type;}
};
typedef QMap<QString, Sensor>* MapSensors;

#endif // STRUCTS_H
