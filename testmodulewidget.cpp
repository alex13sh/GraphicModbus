#include "testmodulewidget.h"
#include "ui_testmodulewidget.h"

#include "modbusdevice.h"
#include <QTimer>
#include <QDebug>

TestModuleWidget::TestModuleWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestModuleWidget)
{
    ui->setupUi(this);

    auto tim1 = new QTimer(this);
    connect(tim1, &QTimer::timeout, this, [this](){
        if(ui->cmbDevice->count()!=mapDevices->size()) this->updateListDevice();
    });
    tim1->start(5000);
}

TestModuleWidget::~TestModuleWidget()
{
    delete ui;
}

void TestModuleWidget::on_pbAdd_clicked() {

}

void TestModuleWidget::updateListDevice() {
    if(!mapDevices) return;
    ui->cmbDevice->clear(); cmbListName.clear();
    for(const auto *itm : *mapDevices){
        cmbListName.append(itm->name());
        ui->cmbDevice->addItem(QString("%1 (%2)").arg(itm->name()).arg(itm->typeStr()));
    }
}

#include "modbussensor.h"
void TestModuleWidget::updateListSensor(const ModbusDevice *dev) {
    if(!dev) return;
    ui->listWidget->clear();
    for(auto sens:dev->getListSensors()){
        ui->listWidget->addItem(QString("Name: %1").arg(sens->name()));
    }
}

void TestModuleWidget::on_cmbDevice_currentIndexChanged(int index) {
    if(!mapDevices) return;
    if(index==-1) return;
    if(index>=cmbListName.count()) return;
    auto name = cmbListName[index];
    updateListSensor(mapDevices->value(name, nullptr));
}
