
import QtQuick 2.0

Item
{
    id: container
    width: 320
    height: 480

    Component.onCompleted:
    {
        freeMemoryStr.text = txCommandModel.getFreeMemoryStr();
    }

    Column
    {
        anchors.fill: parent
        anchors.topMargin: 12
        spacing: 10

        Text
        {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Free memory"
            font.pointSize: 24
            font.bold: true
            color: "#777"
        }
    }

    Column
    {
        anchors.left: parent.left
        anchors.leftMargin: 21
        anchors.top: parent.top
        anchors.topMargin: 60
        spacing: 25


        Row
        {
            Text {text: "Free memory: "}
            Text {id:freeMemoryStr}
        }
    }
}
