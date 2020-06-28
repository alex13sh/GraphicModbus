#include "modbusdevice_analog.h"

ModbusDevice_Analog::ModbusDevice_Analog(const QString &name, QObject *parent) :
    ModbusDevice(name, DeviceType::OVEN_InpoutAnalog, parent)
{
}

void ModbusDevice_Analog::getValues(quint16 adr, ValuesType values) {
    if(adr>=4000 && adr<=4071)
        updateValueSensor(adr, values);
}

#include "modbussensor.h"
void ModbusDevice_Analog::updateValueSensor(quint16 adr, ValuesType value) {
    if(adr>=4000 && adr<4064){
        qint32 ivalue = (((qint32)value[1]<<16)|(qint32)value[0]);
        float fvalue=(float&)ivalue;
        quint8 pin = (adr-4000)/3+1;
//        m_sensors[pin]->m_fvalue = fvalue;
        m_sensors[pin]->setValueF(fvalue);
    }else if(adr>=4064 && adr<=4071){
        qint16 ivalue=(quint16)(value[0]);
        quint8 pin = (adr-4064)/1+1;
//        m_sensors[pin]->m_ivalue = ivalue;
    }
}

void ModbusDevice_Analog::getValueSensor(quint8 pin) const {
    sendRead(4000+(pin-1)*3, 2); // float value
    sendRead(4064+(pin-1), 1);  // int value
}
