#ifndef SENSORANALOG_INFOWIDGET_H
#define SENSORANALOG_INFOWIDGET_H

#include <QWidget>

namespace Ui {
class Sensor_InfoWidget;
}

class ModbusSensor;
class Sensor_InfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit Sensor_InfoWidget(QWidget *parent = nullptr);
    ~Sensor_InfoWidget();

    void setSensor(ModbusSensor *sens){m_sens=sens; updateForm();}

private:
    Ui::Sensor_InfoWidget *ui;

    ModbusSensor *m_sens=nullptr;

    void updateForm();
};

#endif // SENSORANALOG_INFOWIDGET_H
