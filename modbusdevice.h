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
    QModbusClient *device() const {return m_device;}
signals:

private:
    QString name = "None";
    enum DeviceType{
        Other,
        OVEN_InpoutAnalog,
        OVEN_IODiget
    } m_type = Other;
    QModbusClient *m_device=nullptr;

    void onReadReady();
    bool sendRead(quint16 addr, quint16 cnt);
};
typedef QMap<QString, ModbusDevice>* MapDevices;
typedef QMap<QString, ModbusDevice> MapDevices_;

#endif // MODBUSDEVICE_H
