QT += core
QT -= gui

TARGET = Test
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ../../Radio/Command.cpp \
    ../../Radio/MemoryFree.cpp \
    ../../Radio/Mesure.cpp \
    ../../Radio/Model.cpp \
    ../../Radio/RCLEval.cpp \
    ../../Radio/Sensor.cpp \
    ../../Radio/SerialLink.cpp \
    ../../Radio/Tx.cpp \
    lib/arduino.cpp

HEADERS += \
    ../../Radio/Command.h \
    ../../Radio/config.h \
    ../../Radio/MemoryFree.h \
    ../../Radio/Mesure.h \
    ../../Radio/Model.h \
    ../../Radio/Radio.ino \
    ../../Radio/RCLEval.h \
    ../../Radio/Sensor.h \
    ../../Radio/SerialLink.h \
    ../../Radio/Streaming.h \
    ../../Radio/Tx.h

INCLUDEPATH += lib

DEFINES += "ARDUINO=\"200\""
