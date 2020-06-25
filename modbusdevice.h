#ifndef MODBUSDEVICE_H
#define MODBUSDEVICE_H

#include <QObject>

enum DeviceType{
    Other,
    OVEN_InpoutAnalog,
    OVEN_IODiget
};
class QModbusClient;
class ModbusDevice : public QObject
{
    Q_OBJECT
public:
    explicit ModbusDevice(QObject *parent = nullptr);
    ModbusDevice(const QString &name, QModbusClient *device, DeviceType type)
        : m_name(name), m_device(device), m_type(type)
    {}

    QString name() const {return this->m_name;}
    QString typeStr() const;
    QModbusClient *device() const {return m_device;}
signals:

private:
    QString m_name = "None";
    DeviceType m_type = DeviceType::Other;
    QModbusClient *m_device=nullptr;

    void onReadReady();
    bool sendRead(quint16 addr, quint16 cnt);
};
typedef QMap<QString, ModbusDevice>* MapDevices;
typedef QMap<QString, ModbusDevice> MapDevices_;

#endif // MODBUSDEVICE_H
