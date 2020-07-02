#ifndef GRAPHSENSORFORM_H
#define GRAPHSENSORFORM_H

#include <QWidget>

#include "defines.h"
namespace Ui {
class GraphSensorForm;
}

class GraphSensorForm : public QWidget
{
    Q_OBJECT

public:
    explicit GraphSensorForm(QWidget *parent = nullptr);
    ~GraphSensorForm();

    void setDevices(MapDevices devices);

private slots:
    void on_pbStart_clicked();

    void on_pbUpdateListSensor_clicked();

private:
    Ui::GraphSensorForm *ui;

    MapDevices mapDevices = nullptr;
    QVector<ModbusSensor*> m_lstSensor;
    QVector<ModbusValue*> m_lstValue;

    QTimer *m_timUpdate = nullptr;

    void updateListSensors();
};

#endif // GRAPHSENSORFORM_H
