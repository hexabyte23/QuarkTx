import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Flow {
    x: 0
    y: parent.height - 40
    width: parent.width
    height: 20

    Rectangle {
        width: 70
        height: 40
        color: "black"

        Text {
            x: 10
            y: 10
            text: qsTr("Exit")
            color: "white"
            font.bold: true
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                Qt.quit();
            }
        }
    }
}
