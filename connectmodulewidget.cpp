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
    if(!modbusDevice) return;
    if(ui->cmbType->currentText() == "RTU") modbusDevice->connectRTU(ui->txtDeviceAddress->text());
    else modbusDevice->connectTCP(ui->txtDeviceAddress->text());

    if(modbusDevice->isConnected()) qDebug()<<"Device is connected";
    else qDebug()<<"Device is not connected";
}

void ConnectModuleWidget::on_pbAdd_clicked() {
    if(!mapDevices) return;
    auto name = ui->txtName->text();
    if(mapDevices->contains(name))
        qDebug()<<"Такое имя устройства уже существует";
    else{
        DeviceType type = DeviceType::OtherDevice;
        type = ui->cmbType_Device->currentText()=="InputAnalog"?DeviceType::OVEN_InpoutAnalog : DeviceType::OVEN_IODiget;
        modbusDevice = new ModbusDevice(name, type);
        mapDevices->insert(name, modbusDevice);
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


