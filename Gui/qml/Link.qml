/*
Command.cpp - QuarkTx
Copyright (c) 2015-2016 Thierry & Betrand WILMOT.  All rights reserved.

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
        anchors.fill: parent
        anchors.topMargin: 12
        spacing: 10

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
        //spacing: 25

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

        Rectangle // spacer
        {
            height: 20
            width: 10
        }

        Row
        {
            anchors.topMargin: 10
            Text {text: "Cnx status: "}
            Text {id:cnxStatus ;font.bold: true}
        }

        Row
        {
            Text {text: "Transport status: "}
            Text {id:transportStatus ;font.bold: true}
        }

        Row
        {
            Text {text: "Tx version: "}
            Text {id:versionTx ;font.bold: true}
        }
    }
}
