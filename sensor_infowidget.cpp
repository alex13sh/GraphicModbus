#include "sensor_infowidget.h"
#include "ui_sensor_infowidget.h"
#include "modbussensor.h"

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
}
