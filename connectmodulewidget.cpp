#include "connectmodulewidget.h"
#include "ui_connectmodulewidget.h"

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
    if(!modbusDevice) return;
    auto name = ui->txtName->text();
    if(mapDevices.contains(name))
        qDebug()<<"Такое имя устройства уже существует";
    else{
        mapDevices[name] = modbusDevice;
        modbusDevice = nullptr;
        qDebug()<<"Устройство"<<name<<"добавленно";
    }
}
