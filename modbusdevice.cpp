#include "modbusdevice.h"
#include "modbusvalue.h"

#include <QModbusRtuSerialMaster>
#include <QModbusTcpClient>
#include <QVariant>
#include <QDebug>

ModbusDevice::ModbusDevice(QObject *parent) : QObject(parent)
{

}

bool ModbusDevice::connectRTU(const QString &adr) {
    m_device = new QModbusRtuSerialMaster(this);
    m_device->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
        QVariant(adr));

    m_device->setTimeout(1000);
    m_device->setNumberOfRetries(3);
    if (!m_device->connectDevice())
//        qDebug()<<tr("Connect failed: ") + m_device->errorString();
        return false;
    else return true; //qDebug()<<"Connected!";
}

bool ModbusDevice::connectTCP(const QString &adr)
{
    m_device = new QModbusTcpClient(this);
    m_device->setConnectionParameter(QModbusDevice::NetworkAddressParameter, QVariant(adr));
    m_device->setTimeout(1000);
    m_device->setNumberOfRetries(3);
    if (!m_device->connectDevice())
//        qDebug()<<tr("Connect failed: ") + m_device->errorString();
        return false;
    else return true; //qDebug()<<"Connected!";
}

bool ModbusDevice::isConnected() const {
    return m_device->state() == QModbusDevice::ConnectedState;
}

ModbusSensor *ModbusDevice::createSensor(quint8 pin, const QString &name) {
    return nullptr;
}

void ModbusDevice::setSensor(quint8 pin, ModbusSensor *sens) {
    if(m_sensors.contains(pin))
        return;
    m_sensors[pin] = sens;
    emit updatedListSensors();
}

void ModbusDevice::setValue(quint32 address, ModbusValue *value) {
    m_values.insert(address, value);
}

QString ModbusDevice::typeStr() const {
    switch (m_type) {
    case OVEN_InpoutAnalog: return "Input Analog";
    case OVEN_IODiget: return "IO Digital";
    default: return "Other";
    }
}

void ModbusDevice::onReadReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;
    qDebug()<<"Get Result";
    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();
        getValues(unit.startAddress(), unit.values());
    }else qDebug()<<" - Error:"<<reply->errorString()<<tr("(%1) ").arg(reply->error())<<"; device:"<<m_device->errorString();

}

void ModbusDevice::getValues(quint16 adr, ValuesType value) {
    if(!m_values.contains(adr)) return;
    m_values[adr]->updateValues(value);
}

bool ModbusDevice::sendRead(quint16 addr, quint16 cnt) const {
    if(not isConnected()) return false;

    QModbusDataUnit du(QModbusDataUnit::InputRegisters, addr, cnt);
    if (auto *reply = m_device->sendReadRequest(du, 1)) {
        if (!reply->isFinished()){
            connect(reply, &QModbusReply::finished, this, &ModbusDevice::onReadReady);
            return true;
        }else
            delete reply; // broadcast replies return immediately
    }
    return false;
}

