
import QtQuick 2.0

Item
{
    id: container
    width: 320
    height: 480

    Component.onCompleted:
    {
        var map = txCommandModel.getSensorData(0);
        s0.init = map['trim'];
        map = txCommandModel.getSensorData(1);
        s1.init = map['trim'];
        map = txCommandModel.getSensorData(2);
        s2.init = map['trim'];
        map = txCommandModel.getSensorData(3);
        s3.init = map['trim'];
    }

    Column
    {
        spacing: 10
        anchors.fill: parent
        anchors.topMargin: 12

        Text
        {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Calibrate Trim"
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

        Slider {id:s0; min:0; max:200; name:"Elevator";minLabelWidth:50}
        Slider {id:s1; min:0; max:200; name:"Aileron";minLabelWidth:50}
        Slider {id:s2; min:0; max:200; name:"Rudder";minLabelWidth:50}
        Slider {id:s3; min:0; max:200; name:"Throttle";minLabelWidth:50}
    }
}
