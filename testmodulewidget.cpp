#include "testmodulewidget.h"
#include "ui_testmodulewidget.h"

#include <QTimer>

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
    ui->cmbDevice->clear();
    for(const auto &itm : *mapDevices){
        ui->cmbDevice->addItem(QString("%1 (%2)").arg(itm.name()).arg(itm.typeStr()));
    }
}
