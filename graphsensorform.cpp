#include "graphsensorform.h"
#include "ui_graphsensorform.h"

GraphSensorForm::GraphSensorForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphSensorForm)
{
    ui->setupUi(this);
}

GraphSensorForm::~GraphSensorForm()
{
    delete ui;
}

#include "modbusdevice.h"
#include "modbussensor.h"
#include "modbusvalue.h"
void GraphSensorForm::setDevices(MapDevices devices){
    mapDevices=devices;
    for(auto d:*mapDevices)
        for(auto s:d->getListSensors()){
            m_lstSensor.append(s);
            for(auto v:s->values())
                if(v->readOnly())
                    m_lstValue.append(v);
        }
    updateListSensors();
}

void GraphSensorForm::updateListSensors() {
    ui->lstSensor->clear();
    for(auto v:m_lstValue){
        auto txt = QString("%1/%2 - %3/%4 = %5")
                .arg(v->module()->name())
                .arg(v->sensor()->pin()).arg(v->sensor()->name())
                .arg(v->name());
        if(v->isFloatType()) txt = txt.arg(v->value_float());
        else txt = txt.arg(v->value_int());
        ui->lstSensor->addItem(txt);
    }
}
