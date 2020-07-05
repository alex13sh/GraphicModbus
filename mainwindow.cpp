#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "modbusdevice.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mapDevices = new MapDevices_;
    init_test_device();
    ui->ConnectModule->setDevices(mapDevices);
    ui->TestModule->setDevices(mapDevices);
    ui->GraphSensor->setDevices(mapDevices);
}

MainWindow::~MainWindow()
{
    delete ui;
}

#include "core/modbusdevice.h"
#include "core/modbusdevice_analog.h"
#include "core/modbusdevice_iodigital.h"
#include "core/modbussensor.h"
#include "core/modbusvalue.h"
#include <QTimer>
#include <QDebug>

void MainWindow::init_test_device() {
    auto dev_analog = new ModbusDevice_Analog("Input Analog", this);
    mapDevices->insert(dev_analog->name(), dev_analog);
    dev_analog->connectRTU("/dev/ttyACM0");

    if(dev_analog->isConnected()){
        dev_analog->setADC(false);
        auto sens_analog = static_cast<ModbusSensor_Analog *>(dev_analog->createSensor(1, "Термо 1"));
        sens_analog->setInterval(800);
        sens_analog->setType(ModbusSensor_Analog::Pt_100);

        auto tim = new QTimer(this);
        connect(tim, &QTimer::timeout, this, [sens_analog](){
            auto *v = sens_analog->getValueFloat();
            auto txt = QString("%1) %2/%3 - %4/%5 = %6")
                    .arg(v->countUpdate()).arg(v->module()->name())
                    .arg(v->sensor()->pin()).arg(v->sensor()->name())
                    .arg(v->name());
            if(v->isFloatType()) txt = txt.arg(v->value_float());
            else txt = txt.arg(v->value_int());
                qDebug()<<"txt:"<<txt;
            v->updateValues();
        });
        tim->start(10);

        sens_analog = static_cast<ModbusSensor_Analog *>(dev_analog->createSensor(2, "Термо 2"));
        sens_analog->setInterval(600);
        sens_analog->setType(ModbusSensor_Analog::Pt_100);
//        sens_analog = static_cast<ModbusSensor_Analog *>(dev_analog->createSensor(5, "Пирометр"));
//        sens_analog->setInterval(1000);
//        sens_analog->setType(ModbusSensor_Analog::Amper_4_20);
        sens_analog = static_cast<ModbusSensor_Analog *>(dev_analog->createSensor(3, "Термо 3"));
        sens_analog->setInterval(600);
        sens_analog->setType(ModbusSensor_Analog::Volt_1);
        sens_analog = static_cast<ModbusSensor_Analog *>(dev_analog->createSensor(4, "Термо 4"));
        sens_analog->setInterval(600);
        sens_analog->setType(ModbusSensor_Analog::Volt_1);
        sens_analog = static_cast<ModbusSensor_Analog *>(dev_analog->createSensor(5, "Термо 5"));
        sens_analog->setInterval(600);
        sens_analog->setType(ModbusSensor_Analog::Volt_1);
        sens_analog = static_cast<ModbusSensor_Analog *>(dev_analog->createSensor(6, "Термо 6"));
        sens_analog->setInterval(600);
        sens_analog->setType(ModbusSensor_Analog::Volt_1);
        sens_analog = static_cast<ModbusSensor_Analog *>(dev_analog->createSensor(7, "Термо 7"));
        sens_analog->setInterval(600);
        sens_analog->setType(ModbusSensor_Analog::Volt_1);
    }

//    auto dev_digital = new ModbusDevice_IODigital("Input\Output Digital", this);
//    mapDevices->insert(dev_digital->name(), dev_digital);
//    auto sens_counter = dev_digital->createSensor(2, "Test Counter");
}

