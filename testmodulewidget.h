#ifndef TESTMODULEWIDGET_H
#define TESTMODULEWIDGET_H

#include <QWidget>

#include "structs.h"

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

private:
    Ui::TestModuleWidget *ui;

    MapDevices mapDevices;
    void updateListDevice();
};

#endif // TESTMODULEWIDGET_H
