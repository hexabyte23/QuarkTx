
import QtQuick 2.0

Item
{
    id: container
    width: 320
    height: 480

    Component.onCompleted:
    {
        cnxStatus.text = radioLinkModel.getConnexionStatusStr();
        transportStatus.text = radioLinkModel.getTransportStatusStr();
        versionTx.text = radioLinkModel.getTxVersionStr();
    }

    Column
    {
        spacing: 10
        anchors.fill: parent
        anchors.topMargin: 12

        Text
        {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Tx link"
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

        Button
        {
            id: connectButton

            Component.onCompleted:
            {
                if(radioLinkModel.isTxConnected())
                    text = "Close cnx";
                else
                    text = "Connect";
            }

            onClicked:
            {
                if(radioLinkModel.isTxConnected())
                    radioLinkModel.txDisconnect();
                else
                    radioLinkModel.findTxAndConnect();

                if(radioLinkModel.isTxConnected())
                    text = "Close cnx";
                else
                    text = "Connect";

                cnxStatus.text = radioLinkModel.getConnexionStatusStr();
                transportStatus.text = radioLinkModel.getTransportStatusStr();
                versionTx.text = radioLinkModel.getTxVersionStr();
            }
        }

        Row
        {
            Text {text: "Cnx status: "}
            Text {id:cnxStatus}
        }

        Row
        {
            Text {text: "Transport status: "}
            Text {id:transportStatus}
        }

        Row
        {
            Text {text: "Tx version: "}
            Text {id:versionTx}
        }

        /*
        Slider {id:elevatorStick; min:-100; max:100; name:"Elevator";minLabelWidth:50}
        Slider {id:aileronStick; min:0; max:200; name:"Aileron";minLabelWidth:50}
        Slider {id:rudderStick; min:0; max:200; name:"Rudder";minLabelWidth:50}
        Slider {id:throttleStick; min:0; max:200; name:"Throttle";minLabelWidth:50}
        */
    }

}
