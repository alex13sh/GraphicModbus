#include "modbusdevice.h"

#include <QModbusRtuSerialMaster>

ModbusDevice::ModbusDevice(QObject *parent) : QObject(parent)
{

}

QString ModbusDevice::typeStr() const{
    switch (m_type) {
    case OVEN_InpoutAnalog: return "Input Analog";
    case OVEN_IODiget: return "IO Digital";
    default: return "Other";
    }
}

void ModbusDevice::onReadReady()
{

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

