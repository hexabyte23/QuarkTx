TEMPLATE = app

VERSION = 0.0.0.2
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += APP_NAME=\\\"QUARTTX\\\"


QT += qml quick widgets bluetooth printsupport

SOURCES +=  src/main.cpp \
            src/MainModel.cpp \
            src/BatteryModel.cpp \
            src/RadioLink.cpp \
            src/qcustomplot.cpp \
            src/TxCommand.cpp \
            src/RclMainWindow.cpp \
            ../Test/radio/lib/arduino.cpp \
            ../Radio/RCLEval.cpp \
            ../Radio/Model.cpp \
            ../Radio/Sensor.cpp


HEADERS +=  src/global.h \
            src/main.h \
            src/MainModel.h \
            src/BatteryModel.h \
            src/RadioLink.h \
            src/TxCommand.h \
            src/qcustomplot.h \
            src/RclMainWindow.h \
            ../Test/radio/lib/arduino.h \
            ../Test/radio/lib/EEPROM.h \
            ../Test/radio/lib/stream.h \
            ../Radio/RCLEval.h \
            ../Radio/Mesure.h \
            ../Radio/Sensor.h \
            ../Radio/config.h

RESOURCES += qml.qrc

macx {
    QT += serialport
    #QMAKE_MAC_SDK = macosx10.11
}

win32 {
    QT += serialport
}

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = qml/

# Default rules for deployment.
include(deployment.pri)

TRANSLATIONS = QuarkTx_fr_FR.ts

lupdate_only{
SOURCES = qml/*.qml \
          qml/*.js \
          src/*.cpp
}

FORMS += \
    src/RclMainWindow.ui

INCLUDEPATH += ../Test/radio/lib

DEFINES += "ARDUINO=\"200\""
