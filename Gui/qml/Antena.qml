import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Item {
    property int level: 0

    Column {
        spacing: 3

        Rectangle {
            width: 5
            height: 4
            border.width: 1
            color: parent.parent.level < 25 ? "white" : "black"
        }

        Rectangle {
            width: 15
            height: 4
            border.width: 1
            color: parent.parent.level < 49 ? "white" : "black"
        }

        Rectangle {
            width: 25
            height: 4
            border.width: 1
            color: parent.parent.level < 74 ? "white" : "black"
        }

        Rectangle {
            width: 35
            height: 4
            border.width: 1
            color: parent.parent.level < 99 ? "white" : "black"
        }
    }

    Text {
        x: 30
        y: 4
        text: qsTr("Tx")
        font.family: "Courier"
        font.bold: true
    }
}
