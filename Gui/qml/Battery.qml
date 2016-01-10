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
import QtQuick.Layouts 1.2

Row {
    spacing: 0

    Text {
        y: 3
        text: batteryModel.getRemainingTime()
        font.pointSize: 12
    }

    Item {
        id: battery_item
        x: 40
        Rectangle {
            height: 21
            width: 60
            color: "black"
        }

        Text {
            x: 21
            y: 3
            text: "%1 %".arg(batteryModel.getRemainingLevel())
            font.pointSize: 12
            font.bold: true
            color: "white"
        }

        Rectangle {
            x: 3
            y: 3
            height: 15
            width: 50 * (1-batteryModel.getRemainingLevel()/100)
            color: "white"
            clip: true

            Text {
                x: 19
                y: 0
                text: "%1 %".arg(batteryModel.getRemainingLevel())
                font.pointSize: 12
                font.bold: true
                color: "black"
            }
        }

        Rectangle {
            x: 60
            y: 4
            height: 11
            width: 3
            color: "black"
        }
    }

}
