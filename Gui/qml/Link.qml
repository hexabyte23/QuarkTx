/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0

Item
{
    id: container
    width: 320
    height: 480

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
        id: column1
        anchors.left: parent.left
        anchors.leftMargin: 21
        anchors.top: parent.top
        anchors.topMargin: 60
        spacing: 25

        Button
        {
            id: connectButton
            text: "Connect to Tx"

            onClicked:
            {
                if(radioLinkModel.findTxAndConnect())
                {
                    versionTx.text = radioLinkModel.getTxVersion()
                }
            }
        }

        Row
        {
            Text {text: "Tx version: "}
            Text {id:versionTx;text: "NA"}
        }

        /*
        Slider {id:elevatorStick; min:-100; max:100; name:"Elevator";minLabelWidth:50}
        Slider {id:aileronStick; min:0; max:200; name:"Aileron";minLabelWidth:50}
        Slider {id:rudderStick; min:0; max:200; name:"Rudder";minLabelWidth:50}
        Slider {id:throttleStick; min:0; max:200; name:"Throttle";minLabelWidth:50}
        */
    }

}