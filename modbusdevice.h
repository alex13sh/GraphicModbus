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
typedef QMap<int, ModbusSensor*> ListSensors;

class ModbusDevice : public QObject
{
    Q_OBJECT
public:
    explicit ModbusDevice(QObject *parent = nullptr);
    ModbusDevice(const QString &name, DeviceType type, QObject *parent = nullptr)
        : QObject(parent), m_name(name), m_device(nullptr), m_type(type)
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
    ListSensors getListSensors() const {return m_sensors;}

signals:
    void updatedListSensors();

protected:
    QString m_name = "None";
    DeviceType m_type = DeviceType::OtherDevice;
    QModbusClient *m_device=nullptr;

    ListSensors m_sensors; // датчики этого модуля
    using ValuesType = QVector<quint16>;
//    typedef void CallBackFunc (QModbusReply *);

    virtual void onReadReady();
    virtual void getValues(quint16 adr, ValuesType value) {};
    bool sendRead(quint16 addr, quint16 cnt) const;
    virtual void updateValueSensor(quint16 adr, ValuesType value) {};

    friend class ModbusSensor;
    virtual void getValueSensor(quint8 pin) const {};
};

#endif // MODBUSDEVICE_H
