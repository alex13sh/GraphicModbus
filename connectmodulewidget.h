#ifndef CONNECTMODULEWIDGET_H
#define CONNECTMODULEWIDGET_H

#include <QWidget>
#include <QMap>

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

private slots:
    void on_pbConnect_clicked();

private:
    Ui::ConnectModuleWidget *ui;

    using ValuesType = QVector<quint16>;

    QModbusReply *lastRequest = nullptr;
    QModbusClient *modbusDevice = nullptr;
    QTimer *tim_value=nullptr;
    QMap<QString, QModbusClient*> mapDevices;
};

#endif // CONNECTMODULEWIDGET_H
