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
    if(!modbusDevice_) return;
//    modbusDevice = modbusDevice_->device();
//    if(modbusDevice) delete modbusDevice;
    if(ui->cmbType->currentText() == "RTU"){
//        modbusDevice = new QModbusRtuSerialMaster(this);
        if(modbusDevice_->connectRTU(ui->txtDeviceAddress->text())) qDebug()<<"Device is connected";
        else qDebug()<<"Device is not connected";
    }else modbusDevice = new QModbusTcpClient(this);
}

void ConnectModuleWidget::on_pbAdd_clicked() {
    if(!mapDevices) return;
    auto name = ui->txtName->text();
    if(mapDevices->contains(name))
        qDebug()<<"Такое имя устройства уже существует";
    else{
        DeviceType type = DeviceType::OtherDevice;
        type = ui->cmbType_Device->currentText()=="InputAnalog"?DeviceType::OVEN_InpoutAnalog : DeviceType::OVEN_IODiget;
//        (*mapDevices)[name] = ModbusDevice(name, modbusDevice, type);
        modbusDevice_ = new ModbusDevice(name, type);
        mapDevices->insert(name, modbusDevice_);
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


