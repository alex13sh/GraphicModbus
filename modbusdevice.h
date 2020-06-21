#ifndef MODBUSDEVICE_H
#define MODBUSDEVICE_H

#include <QObject>

class QModbusClient;
class ModbusDevice : public QObject
{
    Q_OBJECT
public:
    explicit ModbusDevice(QObject *parent = nullptr);

    QString typeStr() const;

signals:

private:
    QString name = "None";
    enum DeviceType{
        Other,
        OVEN_InpoutAnalog,
        OVEN_IODiget
    } type = Other;
    QModbusClient *device=nullptr;
};
typedef QMap<QString, ModbusDevice>* MapDevices;
typedef QMap<QString, ModbusDevice> MapDevices_;

#endif // MODBUSDEVICE_H
