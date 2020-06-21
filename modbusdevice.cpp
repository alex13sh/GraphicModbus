#include "modbusdevice.h"

ModbusDevice::ModbusDevice(QObject *parent) : QObject(parent)
{

}

QString ModbusDevice::typeStr() const{
    switch (type) {
    case OVEN_InpoutAnalog: return "Input Analog";
    case OVEN_IODiget: return "IO Digital";
    default: return "Other";
    }
}
