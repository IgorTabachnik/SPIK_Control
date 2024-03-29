#-------------------------------------------------
#
# Project created by QtCreator 2019-07-09T14:09:33
#
#-------------------------------------------------

QT     += core gui axcontainer
CONFIG += c++11 console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SPiK_control
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
        modbusConnection.cpp \
        dmodbussettings.cpp \
        addressrange.cpp \
        commonThread.cpp \
        searchsettings.cpp \
        devicemodbus.cpp \
        thsearchdevices.cpp \
        ThPollDevices.cpp \
        excelconfig.cpp \
        functiontest.cpp \
        help_func.cpp \
        thcontrolfunctest.cpp \
        settings_spik.cpp \
        pwr_device.cpp \
        3rdparty/libmodbus/src/modbus.c \
        3rdparty/libmodbus/src/modbus-data.c \
        3rdparty/libmodbus/src/modbus-rtu.c \
        3rdparty/libmodbus/src/modbus-tcp.c \
        3rdparty/libmodbus/src/modbus-ascii.c \
        3rdparty/qextserialport/qextserialport.cpp




HEADERS += \
        mainwindow.h \
        modbusConnection.h \
        dmodbussettings.h\
        commonThread.h \
        addressrange.h \
        searchsettings.h \
        devicemodbus.h \
        thsearchdevices.h \
        ThPollDevices.h \
        excelconfig.h \
        functiontest.h \
        fpga_defines.h \
        help_func.h \
        thcontrolfunctest.h \
        modbusregs.h \
        settings_spik.h \
        pwr_device.h \
        3rdparty/qextserialport/qextserialport.h \
        3rdparty/qextserialport/qextserialenumerator.h \
        3rdparty/libmodbus/src/modbus.h \


FORMS += \
        settings_spik.ui \
        addressrange.ui \
        mainwindow.ui \
        dmodbussettings.ui \

INCLUDEPATH += 3rdparty/libmodbus \
               3rdparty/libmodbus/src \
               3rdparty/qextserialport \
               src

win32
{
    SOURCES += 3rdparty/qextserialport/win_qextserialport.cpp \
               3rdparty/qextserialport/qextserialenumerator_win.cpp
    DEFINES += _TTY_WIN_ WINVER=0x0501
    LIBS += -lsetupapi -lws2_32
}



win32: LIBS += -L'C:/Program Files (x86)/IVI Foundation/VISA/WinNT/lib/msc/' -lvisa32
INCLUDEPATH += 'C:/Program Files (x86)/IVI Foundation/VISA/WinNT/Include'
DEPENDPATH += 'C:/Program Files (x86)/IVI Foundation/VISA/WinNT/Include'
