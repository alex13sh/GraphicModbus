#include "testmodulewidget.h"
#include "ui_testmodulewidget.h"

#include "core/modbusdevice.h"
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
    if(!m_curDevice) return;
    auto sens = m_curDevice->createSensor(ui->spin_pin->value(), ui->txtName->text());
    updateListSensor(m_curDevice);
}

void TestModuleWidget::updateListDevice() {
    if(!mapDevices) return;
    ui->cmbDevice->clear(); m_lstDevice.clear();
    for(auto *itm : *mapDevices){
        m_lstDevice.append(itm);
        ui->cmbDevice->addItem(QString("%1 (%2)").arg(itm->name()).arg(itm->typeStr()));
    }
}

#include "core/modbussensor.h"
void TestModuleWidget::updateListSensor(const ModbusDevice *dev) {
    if(!dev) return;
    ui->listWidget->clear(); m_lstSensor.clear();
    for(auto sens:dev->getListSensors()){
        m_lstSensor.append(sens);
        ui->listWidget->addItem(QString("Name: %1").arg(sens->name()));
    }
}

void TestModuleWidget::on_cmbDevice_currentIndexChanged(int index) {
    if(!mapDevices) return;
    if(index==-1) return;
    if(index>=m_lstDevice.count()) return;
//    auto name = m_lstDevice[index]->name();
//    updateListSensor(mapDevices->value(name, nullptr));
    m_curDevice = m_lstDevice.value(index, nullptr);
    updateListSensor(m_curDevice);
}

void TestModuleWidget::on_listWidget_currentRowChanged(int currentRow) {
    if(currentRow==-1) return;
    ui->InfoForm->setSensor(m_lstSensor.value(currentRow, nullptr));
}

#include "core/modbusdevice_analog.h"
void TestModuleWidget::on_pbTest_clicked() {
    auto dev_analog = static_cast<ModbusDevice_Analog *>(m_curDevice);
//    if(dev_analog->isConnected()){
//        auto sens_analog = static_cast<ModbusSensor_Analog *>(dev_analog->createSensor(1, "Термо 1"));
//        sens_analog->setInterval(6000);
//        sens_analog->setType(ModbusSensor_Analog::None);
////        sens_analog = static_cast<ModbusSensor_Analog *>(dev_analog->createSensor(2, "Термо 2"));
////        sens_analog->setInterval(1000);
////        sens_analog->setType(ModbusSensor_Analog::Pt_100);
////        sens_analog = static_cast<ModbusSensor_Analog *>(dev_analog->createSensor(5, "Пирометр"));
////        sens_analog->setInterval(1000);
////        sens_analog->setType(ModbusSensor_Analog::Amper_4_20);
//    }
//    for(auto s:dev_analog->getListSensors())
//        s->setTy
}
