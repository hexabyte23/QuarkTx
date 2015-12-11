import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

ToolButton {

    property int textSize: 16

    Image {
        id: icon
        x: parent.width * 0.25
        y: parent.height * 0.25 - parent.textSize / 2
        width: parent.width * 0.5
        height: parent.height * 0.5
        source: parent.iconSource
    }

    style: ButtonStyle {
        label: Text {
            id: labelText
            color: control.pressed ? "white" : "black"
            font.pointSize: control.textSize
            text: control.text
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignBottom
        }

        background: Rectangle {
            implicitWidth: 10
            implicitHeight: 25
            border.width: control.activeFocus ? 2 : 1
            border.color: "#888"
            radius: 4
            gradient: Gradient {
                GradientStop { position: 0 ; color: control.pressed ? "#1D1" : "#eee" }
                GradientStop { position: 1 ; color: control.pressed ? "#171" : "#ccc" }
            }
        }
    }
}
