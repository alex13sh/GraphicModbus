#ifndef DEFINES_H
#define DEFINES_H

#include <QMap>

class ModbusDevice;
class ModbusSensor;
typedef QMap<QString, ModbusDevice*>* MapDevices;
typedef QMap<QString, ModbusDevice*> MapDevices_;

typedef QVector<quint16> ValuesType;
typedef QMap<int, ModbusSensor*> ListSensors;

#endif // DEFINES_H
