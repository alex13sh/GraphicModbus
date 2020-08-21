#include "modbusdevice.h"
#include "modbusvalue.h"

#include <QModbusRtuSerialMaster>
#include <QModbusTcpClient>
#include <QVariant>
#include <QDebug>

ModbusDevice::ModbusDevice(QObject *parent) : QObject(parent)
{

    v_ip_address = new ModbusValue(this, "ip_adrdess", 20,2);
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
    m_device->setTimeout(2000);
    m_device->setNumberOfRetries(3);
    if (!m_device->connectDevice()) {
        qDebug()<<tr("Connect failed: ") + m_device->errorString();
        return false;
    } else {
         qDebug()<<"Connected!";
        return true;
    }
}

bool ModbusDevice::isConnected() const {
    if(!m_device) return false;
    return m_device->state() == QModbusDevice::ConnectedState;
}

void ModbusDevice::set_ipaddress(const QString &ip) {
    auto ip_part = ip.split('.');
    if(ip_part.size()!=4) return;
    quint32 ip_int=0;
    for(int i=0; i<4; ++i){
        quint8 v = ip_part.at(i).toUInt();
        ip_int|=v<<(8*i);
    }
//    ValuesType res(2);
//    res[0] = ip_int & 0xffff;
//    res[1] = ip_int >> 16;
//    sendWrite(20, res);
    v_ip_address->setValue_uint32(ip_int);
}

ModbusSensor *ModbusDevice::createSensor(quint8 pin, const QString &name) {
    return nullptr;
}

void ModbusDevice::setSensor(quint8 pin, ModbusSensor *sens) {
//    if(m_sensors.contains(pin))
//        return;
    m_sensors.insert(pin, sens);
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
//    qDebug()<<"Get Result";
    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();
        getValues(unit.startAddress(), unit.values());
    }else {
        qDebug()<<" - Error:"<<reply->errorString()<<tr("(%1) ").arg(reply->error())<<"; device:"<<this->name();
        if (reply->error() == 7) { // Error: "Reply aborted due to connection closure." "(7) " ;
            m_device->connectDevice();
        }
    }
}

void ModbusDevice::getValues(quint16 adr, ValuesType value) {
    int adr_end = adr+value.size();
    qDebug()<<"ModbusDevice::getValues adr_start:"<<adr<<"; adr_end:"<<adr_end;
    for (int a=adr; a<adr_end; a++) {
        if(m_values.contains(a)) {
            int cnt = m_values[a]->size();
            m_values[a]->updateValues(value.mid(a-adr, cnt));
        }
    }
}

bool ModbusDevice::sendRead(quint16 addr, quint16 cnt) const {
    if(not isConnected()) return false;
    qDebug()<<"ModbusDevice::sendRead adr:"<<addr<<"; cnt:"<<cnt;
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

bool ModbusDevice::sendWrite(quint16 addr, ValuesType value) const {
    if(! isConnected()) return false;
    qDebug()<<"ModbusDevice::sendWrite:"<<addr<<value;
    QModbusDataUnit du(QModbusDataUnit::HoldingRegisters, addr, value);
    if (auto *reply = m_device->sendWriteRequest(du, 1)) {
        if (!reply->isFinished()){
            connect(reply, &QModbusReply::finished, this, &ModbusDevice::onReadReady);
            return true;
        }else{
            qDebug()<<"Error: "<<reply->errorString();
            delete reply; // broadcast replies return immediately
        }
    }
    return false;
}

