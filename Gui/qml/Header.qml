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

import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Row {
    property string modelname: qsTr("noname")
    id: header
    anchors.horizontalCenter: parent.horizontalCenter
    spacing: -1

    Rectangle {
        id: antena_header
        width: 55
        height: 40
        border.width: 1

        Antena {
            x: 5
            anchors.verticalCenterOffset: -11
            anchors.verticalCenter: parent.verticalCenter

            //NumberAnimation on level { from: 0; to: 150; duration: 2000; loops: Animation.Infinite }
        }
    }

    Rectangle {
        id: main_header
        width: parent.parent.width - antena_header.width - battery_header.width - time_header.width
        height: 40
        border.width: 1

        Row {
            width: 200
            height: 40
            clip: false
            spacing: 3

            Text {
                id: label
                text: qsTr(" Model:")
                transformOrigin: Item.Left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }

            Text {
                id: value
                text: modelname
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }
        }
    }

    Rectangle {
        id: battery_header
        width: 114
        height: 40
        border.width: 1

        Battery {
            id: battery
            x: 6
            anchors.verticalCenterOffset: -3
            anchors.verticalCenter: parent.verticalCenter

            //NumberAnimation on level { from: 100; to: 0; duration: 20000; loops: Animation.Infinite }
        }
    }

    Rectangle {
        id: time_header
        width: 70
        height: 40
        border.width: 1

        Text {
            id: time
            anchors.centerIn: parent
            text: Qt.formatDateTime(new Date(), "hh:mm:ss")
            font.bold: true
            font.family: "Courier"
        }

        Timer {
            interval: 1000
            running: true
            repeat: true
            onTriggered: time.text = Qt.formatDateTime(new Date(), "hh:mm:ss")
        }
    }
}
