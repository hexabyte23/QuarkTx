
import QtQuick 2.0


Item {
    height: 480
    width: 320
    LauncherList {
        id: ll
        anchors.fill: parent
        Component.onCompleted: {
            addExample("Load from EEPROM", "",  Qt.resolvedUrl("bezierCurve/bezierCurve.qml"));
            addExample("Save to EEPROM", "",  Qt.resolvedUrl("quadraticCurveTo/quadraticCurveTo.qml"));
            addExample("Dump", "Sensor, Model, EEPROM",  Qt.resolvedUrl("squircle/squircle.qml"));
            addExample("Display free memory", "",  Qt.resolvedUrl("squircle/squircle.qml"));
            addExample("Display input sensor", "",  Qt.resolvedUrl("squircle/squircle.qml"));
            addExample("Display output channels", "",  Qt.resolvedUrl("squircle/squircle.qml"));
            addExample("Toggle Tx mode", "Transmit, Debug",  Qt.resolvedUrl("squircle/squircle.qml"));
            addExample("Reset", "Clean calibration, Tx mode & models ...",  Qt.resolvedUrl("squircle/squircle.qml"));
        }
    }
}
