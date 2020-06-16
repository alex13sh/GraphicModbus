#ifndef CONNECTMODULEWIDGET_H
#define CONNECTMODULEWIDGET_H

#include <QWidget>

namespace Ui {
class ConnectModuleWidget;
}

class ConnectModuleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectModuleWidget(QWidget *parent = nullptr);
    ~ConnectModuleWidget();

private:
    Ui::ConnectModuleWidget *ui;
};

#endif // CONNECTMODULEWIDGET_H
