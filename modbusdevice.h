#ifndef MODBUSDEVICE_H
#define MODBUSDEVICE_H

#include <QObject>
#include <QMap>

enum DeviceType{
    OtherDevice,
    OVEN_InpoutAnalog,
    OVEN_IODiget
};
class QModbusClient;
class ModbusSensor;
class ModbusDevice : public QObject
{
    Q_OBJECT
public:
    explicit ModbusDevice(QObject *parent = nullptr);
    ModbusDevice(const QString &name, DeviceType type)
        : m_name(name), m_device(nullptr), m_type(type)
    {}
    ModbusDevice(const QString &name, QModbusClient *device, DeviceType type)
        : m_name(name), m_device(device), m_type(type)
    {}

    bool connectRTU(const QString &adr="/dev/ttyACM0");
    bool connectTCP(const QString &adr);
    QModbusClient *getDevice()const{return this->m_device;}

    void setSensor(int adr, ModbusSensor* sens);

    QString name() const {return this->m_name;}
    QString typeStr() const;
    QModbusClient *device() const {return m_device;}
signals:

private:
    QString m_name = "None";
    DeviceType m_type = DeviceType::OtherDevice;
    QModbusClient *m_device=nullptr;

    QMap<int, ModbusSensor*> m_sensors; // датчики этого модуля

    void onReadReady();
    bool sendRead(quint16 addr, quint16 cnt);
    friend class ModbusSensor;
};

#endif // MODBUSDEVICE_H
