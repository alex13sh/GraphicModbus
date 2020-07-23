#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QMainWindow>
//#include "structs.h"
#include "core/defines.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TestWindow; }
QT_END_NAMESPACE

class Logger;

class TestWindow : public QMainWindow
{
    Q_OBJECT

public:
    TestWindow(QWidget *parent = nullptr);
    ~TestWindow();

private:
    Ui::TestWindow *ui;

    MapDevices mapDevices;
    Logger *m_logger;

    void init_test_device();
    void init_test_device_2();
};
#endif // MAINWINDOW_H
