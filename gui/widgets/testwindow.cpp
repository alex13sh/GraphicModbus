#include "testwindow.h"
#include "ui_testwindow.h"
//#include "modbusdevice.h"
#include "core/logger.h"

#include <QDebug>

TestWindow::TestWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TestWindow)
{
    ui->setupUi(this);
    mapDevices = new MapDevices_;
    init_test_device_2();
    qDebug()<<"_bp__1";
    ui->ConnectModule->setDevices(mapDevices);
    qDebug()<<"_bp__2";
    ui->TestModule->setDevices(mapDevices);
    qDebug()<<"_bp__3";
    ui->GraphSensor->setDevices(mapDevices);
    qDebug()<<"_bp__4";
//    ui->tabGraph->hide();

    m_logger = new Logger(this);
    m_logger->setValues(ui->GraphSensor->getValuesSensor());
//    logger->pushValues();
//    delete logger;
    qDebug()<<"_bp__5";
}

TestWindow::~TestWindow()
{
    delete ui;
    delete m_logger;
}

#include "core/modbusdevice.h"
#include "core/modbusdevice_analog.h"
#include "core/modbusdevice_iodigital.h"
#include "core/modbussensor.h"
#include "core/modbusvalue.h"
#include <QTimer>

void TestWindow::init_test_device() {
    qDebug()<<"init_test_device";
    int bp_cnt=1;
    auto dev_analog = new ModbusDevice_Analog("Input Analog", this);
    qDebug()<<"bp_"<<bp_cnt++;
    mapDevices->insert(dev_analog->name(), dev_analog);
    dev_analog->connectRTU("/dev/ttyACM0");
    qDebug()<<"bp_"<<bp_cnt++;
//    if(dev_analog->isConnected()){
        dev_analog->setADC(false);
        auto sens_analog = static_cast<ModbusSensor_Analog *>(dev_analog->createSensor(1, "Термо 1"));
        sens_analog->setInterval(800);
        sens_analog->setType(ModbusSensor_Analog::Pt_100);
        sens_analog = static_cast<ModbusSensor_Analog *>(dev_analog->createSensor(2, "Термо 2"));
        sens_analog->setInterval(800);
        sens_analog->setType(ModbusSensor_Analog::Pt_100);
qDebug()<<"bp_"<<bp_cnt++;
//        sens_analog = static_cast<ModbusSensor_Analog *>(dev_analog->createSensor(5, "Пирометр"));
//        sens_analog->setInterval(1000);
//        sens_analog->setType(ModbusSensor_Analog::Amper_4_20);
//        for(int i=3; i<8; ++i){
//            sens_analog = static_cast<ModbusSensor_Analog *>(dev_analog->createSensor(i, "Пирометр"));
//            sens_analog->setType(ModbusSensor_Analog::None);
//        }
//    }

//    auto dev_digital = new ModbusDevice_IODigital("Input\Output Digital", this);
//    mapDevices->insert(dev_digital->name(), dev_digital);
//    auto sens_counter = dev_digital->createSensor(2, "Test Counter");

}

void TestWindow::init_test_device_2()
{
//    auto dev_analog = new ModbusDevice_Analog("Input Analog", this);
//    mapDevices->insert(dev_analog->name(), dev_analog);
//    dev_analog->connectRTU("/dev/ttyACM0");
//    auto sens_analog = static_cast<ModbusSensor_Analog *>(dev_analog->createSensor(1, "Перометр 4_20"));
//    sens_analog->setInterval(800);
//    sens_analog->setType(ModbusSensor_Analog::Amper_4_20);
//    sens_analog->setRange(0, 250.0);

//    sens_analog = static_cast<ModbusSensor_Analog *>(new ModbusSensor_Analog_Davl("Давление -1_1 V", 2, dev_analog));
//    dev_analog->initSensor(sens_analog); // 0.1 В
//    dev_analog->setSensor(sens_analog->pin(), sens_analog);
//    sens_analog->setInterval(800);
//    sens_analog->setType(ModbusSensor_Analog::Volt_1);
//    sens_analog->setRange(-1.0, 1.0);
////    dev_analog->setSensor(2, dynamic_cast<ModbusSensor_Analog_Davl*>(sens_analog));

//    sens_analog  = static_cast<ModbusSensor_Analog *>(dev_analog->createSensor(3, "Вибрация 4_20 A"));
//    sens_analog->setInterval(600);
//    sens_analog->setType(ModbusSensor_Analog::Amper_4_20);
//    sens_analog->setRange(0, 25.0);

//    sens_analog = static_cast<ModbusSensor_Analog *>(dev_analog->createSensor(4, "Термосопротивление 1"));
//    sens_analog->setInterval(800);
//    sens_analog->setType(ModbusSensor_Analog::Pt_100);
//    sens_analog = static_cast<ModbusSensor_Analog *>(dev_analog->createSensor(5, "Термосопротивление 2"));
//    sens_analog->setInterval(800);
//    sens_analog->setType(ModbusSensor_Analog::Pt_100);
//    sens_analog = static_cast<ModbusSensor_Analog *>(dev_analog->createSensor(6, "Термосопротивление 3"));
//    sens_analog->setInterval(800);
//    sens_analog->setType(ModbusSensor_Analog::Pt_100);

    auto dev_iod = new ModbusDevice_IODigital("Input/Output Digit", this);
    mapDevices->insert(dev_iod->name(), dev_iod);
    dev_iod->connectRTU("/dev/ttyACM0");

//    auto sens_in = static_cast<ModbusSensor_IDigital *>(dev_iod->createSensor(1, "Скоростной счётчик импульсов")); // DI-1 - Скоростной счётчик импульсов
    qDebug()<< "_bp_1";
    auto sens_in = new ModbusSensor_IDigital("Скоростной счётчик импульсов", 1, dev_iod);
    qDebug()<< "_bp_2";
    dev_iod->initSensor(sens_in);
    qDebug()<< "_bp_3";
    dev_iod->setSensor(0, sens_in);
    qDebug()<< "_bp_4";
    sens_in->setInterval(2);
    qDebug()<< "_bp_5";
    sens_in->setTypeInput(ModbusSensor_IDigital::InputHZ);

//    sens_in = static_cast<ModbusSensor_IDigital *>(dev_iod->createSensor(9, "Состояние клапана 1")); // клапан 1 DI-9
//    sens_in = static_cast<ModbusSensor_IDigital *>(dev_iod->createSensor(10, "Состояние клапана 2")); // клапан 2  DI-10
//    sens_in = static_cast<ModbusSensor_IDigital *>(dev_iod->createSensor(11, "Состояние клапана 3")); // клапан 3 DI-11

    auto sens_out = new ModbusSensor_ODigital("Клапан 24В", 1, dev_iod);
    qDebug()<< "_bp_6";
    dev_iod->initSensor(sens_out);
    qDebug()<< "_bp_7";
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

