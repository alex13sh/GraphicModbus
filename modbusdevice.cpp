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

void ModbusDevice::setSensor(quint8 pin, ModbusSensor *sens) {
    if(m_sensors.contains(pin))
        return;
    m_sensors[pin] = sens;
    emit updatedListSensors();
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

        if(unit.startAddress()>=4000
            && unit.startAddress()<=4071)
            updateValueSensor(unit.startAddress(), unit.values());
    }else qDebug()<<" - Error:"<<reply->errorString()<<tr("(%1) ").arg(reply->error())<<"; device:"<<m_device->errorString();

}

bool ModbusDevice::sendRead(quint16 addr, quint16 cnt) const {
    if(m_device->state() != QModbusDevice::ConnectedState) return false;

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

#include "modbussensor.h"
void ModbusDevice::updateValueSensor(quint16 adr, ModbusDevice::ValuesType value) {
    if(adr>=4000 && adr<4064){
        qint32 ivalue = (((qint32)value[1]<<16)|(qint32)value[0]);
        float fvalue=(float&)ivalue;
        quint8 pin = (adr-4000)/3+1;
        m_sensors[pin]->m_fvalue = fvalue;
    }else if(adr>=4064 && adr<=4071){
        qint16 ivalue=(quint16)(value[0]);
        quint8 pin = (adr-4064)/1+1;
        m_sensors[pin]->m_ivalue = ivalue;
    }
}

void ModbusDevice::getValueSensor(quint8 pin) const {
    auto sens = m_sensors[pin]; sens->m_fvalue = 0;
    sendRead(4000+(pin-1)*3, 2); // float value
    sendRead(4064+(pin-1), 1);  // int value
}

