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

    Column
    {
        anchors.fill: parent
        anchors.topMargin: 12
        spacing: 10

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
        anchors.topMargin:80
        spacing: 25

        Slider
        {
            min: 0
            max: 200
            name: "Elevator"
            minLabelWidth: 50
            init: txCommandModel.getSensorData(0)["trim"]
            onValueChanged:txCommandModel.setSensorData(0, {"trim": value})
        }

        Slider
        {
            min: 0
            max: 200
            name: "Aileron"
            minLabelWidth: 50
            init: txCommandModel.getSensorData(1)["trim"]
            onValueChanged:txCommandModel.setSensorData(1, {"trim": value})
        }


        Slider
        {
            min: 0
            max: 200
            name: "Rudder"
            minLabelWidth: 50
            init: txCommandModel.getSensorData(2)["trim"]
            onValueChanged:txCommandModel.setSensorData(2, {"trim": value})
        }

        Slider
        {
            min: 0
            max: 200
            name: "Throttle"
            minLabelWidth: 50
            init: txCommandModel.getSensorData(3)["trim"]
            onValueChanged:txCommandModel.setSensorData(3, {"trim": value})
        }
    }
}
