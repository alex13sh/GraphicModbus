#include "connectmodulewidget.h"
#include "ui_connectmodulewidget.h"

ConnectModuleWidget::ConnectModuleWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectModuleWidget)
{
    ui->setupUi(this);
}

ConnectModuleWidget::~ConnectModuleWidget()
{
    delete ui;
}
