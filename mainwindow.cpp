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
}

MainWindow::~MainWindow()
{
    delete ui;
}

#include "modbusdevice.h"
#include "modbusdevice_analog.h"
#include "modbussensor.h"
void MainWindow::init_test_device() {
    auto dev_analog = new ModbusDevice_Analog("Input Analog", this);
    mapDevices->insert(dev_analog->name(), dev_analog);
    auto sens_resister = new ModbusSensor("Test Resister", 1, dev_analog);
}

