#include "modbussensor.h"

ModbusSensor::ModbusSensor(QObject *parent)
    : QObject(parent)
{

}

ModbusSensor::ModbusSensor(const QString &name, quint16 address, QObject *parent)
    : QObject(parent), name(name), address(address)
{}
