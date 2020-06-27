#include "sensoranalog_infowidget.h"
#include "ui_sensoranalog_infowidget.h"
#include "modbussensor.h"

SensorAnalog_InfoWidget::SensorAnalog_InfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SensorAnalog_InfoWidget)
{
    ui->setupUi(this);
}

SensorAnalog_InfoWidget::~SensorAnalog_InfoWidget()
{
    delete ui;
}

void SensorAnalog_InfoWidget::updateForm() {
    if(!m_sens) return;
    ui->txtName->setText(m_sens->name());
    ui->txtPin->setText(QString::number(m_sens->pin()));
}
