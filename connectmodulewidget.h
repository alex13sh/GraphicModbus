#ifndef CONNECTMODULEWIDGET_H
#define CONNECTMODULEWIDGET_H

#include <QWidget>

//#include "structs.h"
#include "defines.h"
class QModbusClient;
class QModbusReply;
class QTimer;

namespace Ui {
class ConnectModuleWidget;
}

class ConnectModuleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectModuleWidget(QWidget *parent = nullptr);
    ~ConnectModuleWidget();

    void setDevices(MapDevices devices){mapDevices=devices;}

private slots:
    void on_pbConnect_clicked();
    void on_pbAdd_clicked();

private:
    Ui::ConnectModuleWidget *ui;

    using ValuesType = QVector<quint16>;

    QModbusReply *lastRequest = nullptr;
    QModbusClient *modbusDevice = nullptr;
    QTimer *tim_value=nullptr;
    MapDevices mapDevices;

    void updateList();
};

#endif // CONNECTMODULEWIDGET_H
