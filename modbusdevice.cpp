#include "modbusdevice.h"

#include <QModbusRtuSerialMaster>
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
//    m_device = new QModbusTcpClient(this);
    return false;
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

//        updateValues(unit.startAddress(), unit.values());
    }else qDebug()<<" - Error:"<<reply->errorString()<<tr("(%1) ").arg(reply->error())<<"; device:"<<m_device->errorString();

}

bool ModbusDevice::sendRead(quint16 addr, quint16 cnt) {
    if(m_device->state() != QModbusDevice::ConnectedState) return false;

    QModbusDataUnit du(QModbusDataUnit::InputRegisters, addr, cnt);
//    du.setRegisterType(du.InputRegisters);

//    modbusDevice->processResponse(QModbusResponse(QModbusResponse::ReadInputRegisters, QByteArray::fromHex("02cd01")));

    if (auto *reply = m_device->sendReadRequest(du, 1)) {
        if (!reply->isFinished()){
            connect(reply, &QModbusReply::finished, this, &ModbusDevice::onReadReady);
            return true;
        }else
            delete reply; // broadcast replies return immediately
    }
    return false;
}

