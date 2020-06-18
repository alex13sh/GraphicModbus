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

#endif // STRUCTS_H
