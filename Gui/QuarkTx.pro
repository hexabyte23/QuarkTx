TEMPLATE = app

VERSION = 1.0.0.0
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += APP_NAME=\\\"QUARTTX\\\"


QT += qml quick widgets bluetooth serialport

SOURCES +=  src/main.cpp \
    src/mainmodel.cpp \
    src/batterymodel.cpp
HEADERS +=  src/global.h \
            src/main.h \
    src/mainmodel.h \
    src/batterymodel.h \
    qml/shared/shared.h

RESOURCES += qml.qrc

macx {
    QMAKE_MAC_SDK = macosx10.11
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
