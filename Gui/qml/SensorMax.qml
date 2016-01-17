
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
        spacing: 10
        anchors.fill: parent
        anchors.topMargin: 12

        Text
        {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Calibrate Max"
            font.pointSize: 24
            font.bold: true
            color: "#777"
        }
    }

    Column
    {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 60
        spacing: 25

        Slider {id:elevatorStick; min:0; max:200; name:"Elevator";minLabelWidth:50}
        Slider {id:aileronStick; min:0; max:200; name:"Aileron";minLabelWidth:50}
        Slider {id:rudderStick; min:0; max:200; name:"Rudder";minLabelWidth:50}
        Slider {id:throttleStick; min:0; max:200; name:"Throttle";minLabelWidth:50}
    }
}
