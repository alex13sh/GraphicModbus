QT       += core gui serialport serialbus sql

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib
CONFIG += qt plugin qmltypes
QT += qml

QML_IMPORT_NAME = my.work.core
QML_IMPORT_MAJOR_VERSION = 1

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    logger.cpp \
    modbusdevice.cpp \
    modbusdevice_analog.cpp \
    modbusdevice_iodigital.cpp \
    modbussensor.cpp \
    modbusvalue.cpp

HEADERS += \
    structs.h \
    defines.h \
    logger.h \
    modbusdevice.h \
    modbusdevice_analog.h \
    modbusdevice_iodigital.h \
    modbussensor.h \
    modbusvalue.h
