TEMPLATE = app

VERSION = 0.0.0.1
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += APP_NAME=\\\"QUARTTX\\\"


QT += qml quick widgets bluetooth

SOURCES +=  src/main.cpp \
    src/MainModel.cpp \
    src/BatteryModel.cpp \
    src/RadioLink.cpp
HEADERS +=  src/global.h \
            src/main.h \
    src/MainModel.h \
    src/BatteryModel.h \
    qml/shared/shared.h \
    src/RadioLink.h

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
