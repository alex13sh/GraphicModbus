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

#include "modbusdevice.h"
#include "modbusdevice_analog.h"
#include "modbusdevice_iodigital.h"
#include "modbussensor.h"
void MainWindow::init_test_device() {
    auto dev_analog = new ModbusDevice_Analog("Input Analog", this);
    mapDevices->insert(dev_analog->name(), dev_analog);
    auto sens_resister = dev_analog->createSensor(1, "Test Resister");

    auto dev_digital = new ModbusDevice_IODigital("Input\Output Digital", this);
    mapDevices->insert(dev_digital->name(), dev_digital);
    auto sens_counter = dev_digital->createSensor(2, "Test Counter");
}

