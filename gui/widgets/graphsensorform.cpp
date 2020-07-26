#include "graphsensorform.h"
#include "ui_graphsensorform.h"
#include <QTimer>

GraphSensorForm::GraphSensorForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphSensorForm)
{
    ui->setupUi(this);
    m_timUpdate = new QTimer(this);
    m_timUpdate->setInterval(500);
    connect(m_timUpdate, &QTimer::timeout, this, &GraphSensorForm::updateListSensors);
    m_graph = static_cast<GraphicsItem *>(ui->widGraph->chart());
}

GraphSensorForm::~GraphSensorForm()
{
    delete ui;
}

#include "core/modbusdevice.h"
#include "core/modbussensor.h"
#include "core/modbusvalue.h"
void GraphSensorForm::setDevices(MapDevices devices){
    mapDevices=devices;
    m_lstSensor.clear();
    m_lstValue.clear();
    for(auto d:*mapDevices)
        for(auto s:d->getListSensors()){
            m_lstSensor.append(s);
            for(auto v:s->values())
                if(v->readOnly())
                    m_lstValue.append(v);
        }
//    m_graph->setValues(m_lstValue);
    m_graph->setSensors(m_lstSensor);
    updateListSensors();
}

//void GraphSensorForm::updateListSensors() {
//    ui->lstSensor->clear();
//    for(auto v:m_lstValue){
//        auto txt = QString("%1) %2/%3 - %4/%5 = %6")
//                .arg(v->countUpdate()).arg(v->module()->name())
//                .arg(v->sensor()->pin()).arg(v->sensor()->name())
//                .arg(v->name());
//        if(v->isFloatType()) txt = txt.arg(v->value_float());
//        else txt = txt.arg(v->value_int());
//        ui->lstSensor->addItem(txt);
//        v->updateValues();
//    }
//    m_graph->updateValues();
//}

void GraphSensorForm::updateListSensors() {
    ui->lstSensor->clear();
    for(auto s:m_lstSensor){
        auto txt = QString("%1) %2 = %3")
                .arg(s->pin()).arg(s->name())
                .arg(s->value_float());
        ui->lstSensor->addItem(txt);
        s->updateValue();
    }
    m_graph->updateSensors();
}

void GraphSensorForm::on_pbStart_clicked()
{
    if(m_timUpdate->isActive()) {
        m_timUpdate->stop();
        ui->pbStart->setText("Start");
    } else {
        m_timUpdate->start();
        ui->pbStart->setText("Stop");
    }
}

void GraphSensorForm::on_pbUpdateListSensor_clicked() {
    setDevices(mapDevices);
}
