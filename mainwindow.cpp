#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "modbusdevice.h"
#include "core/logger.h"

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

    m_logger = new Logger(this);
    m_logger ->setValues(ui->GraphSensor->getValuesSensor());
//    logger->pushValues();
//    delete logger;
}

MainWindow::~MainWindow()
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
#include <QDebug>

void MainWindow::init_test_device() {
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

