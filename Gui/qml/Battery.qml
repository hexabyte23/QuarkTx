import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Layouts 1.2

Row {
    spacing: 0

    Text {
        y: 3
        text: batteryModel.getRemainingTime()
        font.pointSize: 12
    }

    Item {
        id: battery_item
        x: 40
        Rectangle {
            height: 21
            width: 60
            color: "black"
        }

        Text {
            x: 21
            y: 3
            text: "%1 %".arg(batteryModel.getRemainingLevel())
            font.pointSize: 12
            font.bold: true
            color: "white"
        }

        Rectangle {
            x: 3
            y: 3
            height: 15
            width: 50 * (1-batteryModel.getRemainingLevel()/100)
            color: "white"
            clip: true

            Text {
                x: 19
                y: 0
                text: "%1 %".arg(batteryModel.getRemainingLevel())
                font.pointSize: 12
                font.bold: true
                color: "black"
            }
        }

        Rectangle {
            x: 60
            y: 4
            height: 11
            width: 3
            color: "black"
        }
    }

}
