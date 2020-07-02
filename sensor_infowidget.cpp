#include "sensor_infowidget.h"
#include "ui_sensor_infowidget.h"
#include "modbussensor.h"
#include "modbusvalue.h"

Sensor_InfoWidget::Sensor_InfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Sensor_InfoWidget)
{
    ui->setupUi(this);
}

Sensor_InfoWidget::~Sensor_InfoWidget()
{
    delete ui;
}

void Sensor_InfoWidget::updateForm() {
    if(!m_sens) return;
    ui->txtName->setText(m_sens->name());
    ui->txtPin->setText(QString::number(m_sens->pin()));

    for(QWidget *w : vec_label) {ui->gridLayout->removeWidget(w); w->deleteLater();}
    for(QWidget *w : vec_text) {ui->gridLayout->removeWidget(w); w->deleteLater();}
    ui->gridLayout->removeItem(ui->verticalSpacer);
    vec_label.clear();
    vec_text.clear();
    for(auto val : m_sens->values()){
        auto lb = new QLabel(val->name(), this);
        auto txt = new QLineEdit(QString::number(val->value_int()), this);
        txt->setEnabled(!val->readOnly());
        vec_label.append(lb);
        vec_text.append(txt);
        ui->gridLayout->addWidget(lb, vec_label.size()+1, 0);
        ui->gridLayout->addWidget(txt, vec_text.size()+1, 1);
    }
    ui->gridLayout->addItem(ui->verticalSpacer,vec_text.size()+2, 1);
}
