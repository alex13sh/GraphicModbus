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
class QModbusReply;

class ModbusDevice : public QObject
{
    Q_OBJECT
public:
    explicit ModbusDevice(QObject *parent = nullptr);
    ModbusDevice(const QString &name, DeviceType type)
        : QObject(nullptr), m_name(name), m_device(nullptr), m_type(type)
    {}
    ModbusDevice(const QString &name, QModbusClient *device, DeviceType type)
        : QObject(nullptr), m_name(name), m_device(device), m_type(type)
    {}

    bool connectRTU(const QString &adr="/dev/ttyACM0");
    bool connectTCP(const QString &adr);
    QModbusClient *getDevice() const {return this->m_device;}

    void setSensor(quint8 pin, ModbusSensor* sens);

    QString name() const {return this->m_name;}
    QString typeStr() const;
    QModbusClient *device() const {return m_device;}
signals:
    void updatedListSensors();
private:
    QString m_name = "None";
    DeviceType m_type = DeviceType::OtherDevice;
    QModbusClient *m_device=nullptr;

    QMap<int, ModbusSensor*> m_sensors; // датчики этого модуля
    using ValuesType = QVector<quint16>;
//    typedef void CallBackFunc (QModbusReply *);

    void onReadReady();
    bool sendRead(quint16 addr, quint16 cnt) const;
    void updateValueSensor(quint16 adr, ValuesType value);

    friend class ModbusSensor;
    void getValueSensor(quint8 pin) const;
};

#endif // MODBUSDEVICE_H
