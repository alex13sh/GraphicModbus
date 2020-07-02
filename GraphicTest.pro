QT       += core gui serialport serialbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    widgets/connectmodulewidget.cpp \
    widgets/graphsensorform.cpp \
    main.cpp \
    mainwindow.cpp \
    core\modbusdevice.cpp \
    core\modbusdevice_analog.cpp \
    core\modbusdevice_iodigital.cpp \
    core\modbussensor.cpp \
    core\modbusvalue.cpp \
    widgets/sensor_infowidget.cpp \
    widgets/testmodulewidget.cpp

HEADERS += \
    widgets/connectmodulewidget.h \
    core/defines.h \
    widgets/graphsensorform.h \
    mainwindow.h \
    core/modbusdevice.h \
    core/modbusdevice_analog.h \
    core/modbusdevice_iodigital.h \
    core/modbussensor.h \
    core/modbusvalue.h \
    widgets/sensor_infowidget.h \
    structs.h \
    widgets/testmodulewidget.h

FORMS += \
    widgets/TestModuleWidget.ui \
    widgets/connectmodulewidget.ui \
    widgets/graphsensorform.ui \
    mainwindow.ui \
    widgets/sensor_infowidget.ui \
    widgets/testmodulewidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
