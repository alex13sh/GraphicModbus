#include "modbusdeviceinit.h"
#include <QDebug>

ModbusDevicesInit::ModbusDevicesInit(QObject *parent)
    : QObject(parent)
{
    m_mapDevices = new MapDevices_;
    init_devices();
}

void ModbusDevicesInit::init_devices()
{
    init_devices_analog();
    init_devices_digital();
    emit mapDevicesChanged();
}

#include "core/modbusdevice_analog.h"
void ModbusDevicesInit::init_devices_analog()
{
    auto dev_analog = new ModbusDevice_Analog("Input Analog", nullptr);
    m_mapDevices->insert(dev_analog->name(), dev_analog);
//    dev_analog->connectRTU("/dev/ttyACM0");
    dev_analog->connectTCP("192.168.8.3");
    auto sens_analog = static_cast<ModbusSensor_Analog *>(dev_analog->createSensor(1, "Температура Ротора"));
    sens_analog->setInterval(800);
    sens_analog->setType(ModbusSensor_Analog::Amper_4_20);
    sens_analog->setRange(0, 250.0);
    sens_analog->setValueErr(60, 90);

    sens_analog = static_cast<ModbusSensor_Analog *>(new ModbusSensor_Analog_Davl("Давление -1_1 V", 2, dev_analog));
    dev_analog->initSensor(sens_analog); // 0.1 В
    dev_analog->setSensor(sens_analog->pin(), sens_analog);
    sens_analog->setInterval(800);
    sens_analog->setType(ModbusSensor_Analog::Volt_1);
    sens_analog->setRange(-1.0, 1.0);
    sens_analog->setValueErr(0.1, 0.5);
//    dev_analog->setSensor(2, dynamic_cast<ModbusSensor_Analog_Davl*>(sens_analog));

    sens_analog = static_cast<ModbusSensor_Analog *>(new ModbusSensor_Analog_Vibra("Вибрация 4_20 A", 3, dev_analog));
    dev_analog->initSensor(sens_analog);
//    sens_analog  = static_cast<ModbusSensor_Analog *>(dev_analog->createSensor(3, "Вибрация 4_20 A"));
    sens_analog->setInterval(600);
    sens_analog->setType(ModbusSensor_Analog::Amper_4_20);
    sens_analog->setRange(0, 25.0);
    sens_analog->setValueErr(3, 5);

    sens_analog = static_cast<ModbusSensor_Analog *>(dev_analog->createSensor(4, "Температура Статора"));
    sens_analog->setInterval(800);
    sens_analog->setType(ModbusSensor_Analog::Pt_100);
    sens_analog->setValueErr(60, 85);
    sens_analog = static_cast<ModbusSensor_Analog *>(dev_analog->createSensor(5, "Температура Пер.Под."));
    sens_analog->setInterval(800);
    sens_analog->setType(ModbusSensor_Analog::Pt_100);
    sens_analog->setValueErr(60, 80);
    sens_analog = static_cast<ModbusSensor_Analog *>(dev_analog->createSensor(6, "Температура Зад.Под."));
    sens_analog->setInterval(800);
    sens_analog->setType(ModbusSensor_Analog::Pt_100);
    sens_analog->setValueErr(60, 80);
}

#include "core/modbusdevice_iodigital.h"
void ModbusDevicesInit::init_devices_digital()
{
    auto dev_iod = new ModbusDevice_IODigital("Input/Output Digit", nullptr);
    m_mapDevices->insert(dev_iod->name(), dev_iod);
//    dev_iod->connectRTU("/dev/ttyACM0");
    dev_iod->connectTCP("192.168.8.2");

//    auto sens_in = static_cast<ModbusSensor_IDigital *>(dev_iod->createSensor(1, "Скоростной счётчик импульсов")); // DI-1 - Скоростной счётчик импульсов
    auto sens_in = new ModbusSensor_IDigital("Скоростной счётчик импульсов", 1, dev_iod);
    dev_iod->initSensor(sens_in);
    dev_iod->setSensor(0, sens_in);
    sens_in->setInterval(2);
    sens_in->setValueErr(333, 433);
//    qDebug()<< "_bp_5";
//    sens_in->setTypeInput(ModbusSensor_IDigital::InputHZ);

//    sens_in = static_cast<ModbusSensor_IDigital *>(dev_iod->createSensor(9, "Состояние клапана 1")); // клапан 1 DI-9
//    sens_in = static_cast<ModbusSensor_IDigital *>(dev_iod->createSensor(10, "Состояние клапана 2")); // клапан 2  DI-10
//    sens_in = static_cast<ModbusSensor_IDigital *>(dev_iod->createSensor(11, "Состояние клапана 3")); // клапан 3 DI-11

    auto sens_out = new ModbusSensor_ODigital("Клапан 24В", 1, dev_iod);
//    qDebug()<< "_bp_6";
    dev_iod->initSensor(sens_out);
//    qDebug()<< "_bp_7";
    dev_iod->setSensor(1, sens_out);
    sens_out->setTypeOutput(ModbusSensor_ODigital::LogicSignal);
    sens_out = new ModbusSensor_ODigital("Клапан 2", 2, dev_iod);  // клапан 2  D0-2
    dev_iod->initSensor(sens_out);
    dev_iod->setSensor(2, sens_out);
    sens_out->setTypeOutput(ModbusSensor_ODigital::LogicSignal);
    sens_out = new ModbusSensor_ODigital("Насос", 3, dev_iod);  // клапан 3 D0-3
    dev_iod->initSensor(sens_out);
    sens_out->setTypeOutput(ModbusSensor_ODigital::LogicSignal);
}

QVector<ModbusSensor *> ModbusDevicesInit::getListSensors()
{
    QVector<ModbusSensor *> res;
    for (auto d : *m_mapDevices)
        for (auto s : d->getListSensors()) {
            res.append(s);
            m_mapSensors[s->hash()] = s;
            qDebug()<<"ModbusDevicesInit::getListSensors name:"<<s->name()<<"; hash:"<<s->hash();
        }
    return res;
}

QVector<ModbusValue *> ModbusDevicesInit::getValues(bool readOnly)
{
    QVector<ModbusValue *> res;
    for (auto d : *m_mapDevices) {
        for (auto s : d->getListSensors()) {
//            if (s->get_Value()) {
//                res.append(s->get_Value());
//            } else {
                for (auto v : s->values()) {
                    if (!readOnly or (v->readOnly() and readOnly)) res.append(v);
                    if (v->hash_str().left(10) == "82dc5b4c30") res.append(v);
    //                qDebug()<<"ModbusDevicesInit::getValues:"<<v->sensor()->name()<<"; "<<v->name()<<": readOnly:"<<v->readOnly();
                }
//            }
        }
    }
    return res;
}

//#include <QLineSeries>
void ModbusDevicesInit::series_setPoints(const QString &hash, QList<QPointF> points, QLineSeries *series) {
//    series->clear();
    if(m_mapSensors.contains(hash)){
        qDebug()<<"mapSensor hash";
        auto s = m_mapSensors[hash];
        for(int i=0; i<points.size(); ++i){
             points[i].setY(s->value_float_from_int(points[i].y()));
             points[i].setX(i);
        }
    }
    qDebug()<<"Points:"<<points.mid(0, 10);
    series->replace(points);
    series->setPointsVisible(true);
    series->setVisible(true);
}

#include <QDateTime>
qint64 ModbusDevicesInit::getMSecsSinceEpoch() const
{
    return QDateTime::currentMSecsSinceEpoch();
}

void ModbusDevicesInit::updateValues()
{
    qDebug()<<"ModbusDevicesInit::updateValues";
    for (auto *d : *m_mapDevices){
        qDebug()<<" > "<<d->name();
        d->updateValues();
    }
}
