#ifndef TESTMODULEWIDGET_H
#define TESTMODULEWIDGET_H

#include <QWidget>

//#include "structs.h"
#include "defines.h"

namespace Ui {
class TestModuleWidget;
}

class TestModuleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TestModuleWidget(QWidget *parent = nullptr);
    ~TestModuleWidget();

    void setDevices(MapDevices devices){mapDevices=devices; updateListDevice();}

private slots:
    void on_pbAdd_clicked();

    void on_cmbDevice_currentIndexChanged(int index);

private:
    Ui::TestModuleWidget *ui;

    MapDevices mapDevices = nullptr;
    QStringList cmbListName;

    void updateListDevice();
    void updateListSensor(const ModbusDevice *dev);
};

#endif // TESTMODULEWIDGET_H
