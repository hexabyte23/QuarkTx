import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtMultimedia 5.0

ApplicationWindow
{
    id: quarkTxApp
    visible: true
    title: qsTr("QuarkTx")
    height: 480
    width: 320


    LauncherList
    {
        id: ll
        anchors.fill: parent

        Component.onCompleted:
        {
            addExample("System", "EEPROM, Dump, free memory ...",  Qt.resolvedUrl("qrc:/qml/System.qml"));
            addExample("Tx sensors", "Setup",  Qt.resolvedUrl("qrc:/qml/Sensors.qml"));
            addExample("Models", "Setup",  Qt.resolvedUrl("qrc:/qml/Models.qml"));
            addExample("Radio Control Language", "Setup",  Qt.resolvedUrl("qrc:/qml/rcl.qml"));
        }
    }
}
