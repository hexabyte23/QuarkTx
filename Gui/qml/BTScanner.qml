/*
Command.cpp - QuarkTx
Copyright (c) 2015-2016 Thierry WILMOT.  All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

import QtQuick 2.0
import QtBluetooth 5.2

Item {
    id: top

    property BluetoothService currentService

    BluetoothDiscoveryModel {
        id: btModel
        running: true
        discoveryMode: BluetoothDiscoveryModel.DeviceDiscovery
        onDiscoveryModeChanged: console.log("Discovery mode: " + discoveryMode)
        onServiceDiscovered: console.log("Found new service " + service.deviceAddress + " " + service.deviceName + " " + service.serviceName);
        onDeviceDiscovered: console.log("New device: " + device)
        onErrorChanged: {
            switch (btModel.error)
            {
            case BluetoothDiscoveryModel.PoweredOffError:
                console.log("Error: Bluetooth device not turned on");
                break;
            case BluetoothDiscoveryModel.InputOutputError:
                console.log("Error: Bluetooth I/O Error");
                break;
            case BluetoothDiscoveryModel.InvalidBluetoothAdapterError:
                console.log("Error: Invalid Bluetooth Adapter Error");
                break;
            case BluetoothDiscoveryModel.NoError:
                break;
            default:
                console.log("Error: Unknown Error");
                break;
            }
        }
    }

    Column {
        id: mainColumn
        spacing: 5

        Rectangle {
            id: busy
            height: text.height*1.2
            width: text.width*1.2
            radius: 5
            color: "#60A000"
            visible: btModel.running

            Text {
                id: text
                text: qsTr("Scanning")
                font.bold: true
                font.pointSize: 20
                anchors.centerIn: parent
            }

            SequentialAnimation on color {
                id: busyThrobber
                ColorAnimation { easing.type: Easing.InOutSine; from: "#60A000"; to: "white"; duration: 1000; }
                ColorAnimation { easing.type: Easing.InOutSine; to: "#60A000"; from: "white"; duration: 1000 }
                loops: Animation.Infinite
            }
        }

        ListView {
            id: btList
            width: 200
            height: 20
            orientation: ListView.Horizontal
            contentHeight: 30
            contentWidth: 10
            clip: true

            model: btModel
            delegate: Rectangle {
                id: btDelegate
                width: parent.width
                height: column.height + 10

                property bool expended: false;
                clip: true
                Image {
                    id: bticon
                    source: "qrc:/bluetooth";
                    width: bttext.height;
                    height: bttext.height;
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.margins: 5
                }

                Column {
                    id: column
                    anchors.left: bticon.right
                    anchors.leftMargin: 5
                    Text {
                        id: bttext
                        text: deviceName ? deviceName : name
                        font.family: "FreeSerif"
                        font.pointSize: 16
                    }

                    Text {
                        id: details

                        function get_details(s)
                        {
                            if (btModel.discoveryMode == BluetoothDiscoveryModel.DeviceDiscovery)
                            {
                                // We are doing a device discovery
                                var str = "Address: " + remoteAddress;
                                return str;
                            }
                            else
                            {
                                var str = "Address: " + s.deviceAddress;
                                if (s.serviceName) { str += "<br>Service: " + s.serviceName; }
                                if (s.serviceDescription) { str += "<br>Description: " + s.serviceDescription; }
                                if (s.serviceProtocol) { str += "<br>Protocol: " + s.serviceProtocol; }
                                return str;
                            }
                        }

                        visible: opacity !== 0
                        opacity: btDelegate.expended ? 1 : 0.0
                        text: get_details(service)
                        font.family: "FreeSerif"
                        font.pointSize: 14
                        Behavior on opacity {
                            NumberAnimation { duration: 200}
                        }
                    }
                }
                Behavior on height { NumberAnimation { duration: 200} }

                MouseArea {
                    anchors.fill: parent
                    onClicked: btDelegate.expended = !btDelegate.expended
                }
            }
            focus: true
        }

        Row {
            id: buttonGroup
            spacing: 10

            MyButton {
                id: fdButton
                width: top.width/3*0.9
                height: mdButton.height
                text: qsTr("Full Discovery")
                onClicked: btModel.discoveryMode = BluetoothDiscoveryModel.FullServiceDiscovery
            }
            MyButton {
                id: mdButton
                width: top.width/3*0.9
                text: qsTr("Minimal Discovery")
                onClicked: btModel.discoveryMode = BluetoothDiscoveryModel.MinimalServiceDiscovery
            }
            MyButton {
                id: devButton
                width: top.width/3*0.9
                height: mdButton.height
                text: qsTr("Device Discovery")
                onClicked: btModel.discoveryMode = BluetoothDiscoveryModel.DeviceDiscovery
            }
        }
    }

}

