#include "connectmodulewidget.h"
#include "ui_connectmodulewidget.h"

#include "modbusdevice.h"

#include <QModbusTcpClient>
#include <QModbusRtuSerialMaster>
#include <QTimer>
#include <QDebug>

ConnectModuleWidget::ConnectModuleWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectModuleWidget)
{
    ui->setupUi(this);
    ui->txtDeviceAddress->setText("/dev/ttyACM0");
}

ConnectModuleWidget::~ConnectModuleWidget()
{
    delete ui;
}

void ConnectModuleWidget::on_pbConnect_clicked() {
    if(modbusDevice) delete modbusDevice;
    if(ui->cmbType->currentText() == "RTU")
        modbusDevice = new QModbusRtuSerialMaster(this);
    else modbusDevice = new QModbusTcpClient(this);

    modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
        ui->txtDeviceAddress->text());

    modbusDevice->setTimeout(1000);
    modbusDevice->setNumberOfRetries(3);
    if (!modbusDevice->connectDevice())
        qDebug()<<tr("Connect failed: ") + modbusDevice->errorString();
    else qDebug()<<"Connected!";
}

void ConnectModuleWidget::on_pbAdd_clicked() {
    if(!mapDevices) return;
    if(!modbusDevice) return;
    auto name = ui->txtName->text();
    if(mapDevices->contains(name))
        qDebug()<<"Такое имя устройства уже существует";
    else{
        DeviceType type = DeviceType::OtherDevice;
        type = ui->cmbType_Device->currentText()=="InputAnalog"?DeviceType::OVEN_InpoutAnalog : DeviceType::OVEN_IODiget;
//        (*mapDevices)[name] = ModbusDevice(name, modbusDevice, type);
        mapDevices->insert(name, new ModbusDevice(name, modbusDevice, type));
        modbusDevice = nullptr;
        qDebug()<<"Устройство"<<name<<"добавленно";
        updateList();
    }
}

void ConnectModuleWidget::updateList() {
    ui->lstDevice->clear();
    for(const auto *itm : *mapDevices){
        ui->lstDevice->addItem(QString("%1 (%2)").arg(itm->name()).arg(itm->typeStr()));
    }
}
