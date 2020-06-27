#ifndef SENSORANALOG_INFOWIDGET_H
#define SENSORANALOG_INFOWIDGET_H

#include <QWidget>

namespace Ui {
class SensorAnalog_InfoWidget;
}

class ModbusSensor;
class SensorAnalog_InfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SensorAnalog_InfoWidget(QWidget *parent = nullptr);
    ~SensorAnalog_InfoWidget();

    void setSensor(ModbusSensor *sens){m_sens=sens; updateForm();}

private:
    Ui::SensorAnalog_InfoWidget *ui;

    ModbusSensor *m_sens=nullptr;

    void updateForm();
};

#endif // SENSORANALOG_INFOWIDGET_H
