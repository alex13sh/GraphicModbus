#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mapDevices = new MapDevices_;
    ui->ConnectModule->setDevices(mapDevices);
}

MainWindow::~MainWindow()
{
    delete ui;
}

