import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Flow {
    x: 120
    y: 88
    anchors.verticalCenter: parent.verticalCenter
    anchors.horizontalCenter: parent.horizontalCenter
    spacing: 43


    MyButton {
        id: modelButton
        width: 102
        height: 90
        text: qsTr("Model")
        iconSource: "qrc:/model2"
        onClicked: {

        }
    }

    MyButton {
        id: setupButton
        width: 102
        height: 90
        text: qsTr("Setup")
        iconSource: "qrc:/bluetooth"
        onClicked: {
            load.source = "qrc:/qml/BTScanner.qml"
        }
    }
}
