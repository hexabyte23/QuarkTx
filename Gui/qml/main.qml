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

import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtMultimedia 5.0

ApplicationWindow
{
    id: quarkTxApp
    visible: true
    title: qsTr("QuarkTx")
    height: 480
    width: 320

    LauncherList
    {
        id: ll
        anchors.fill: parent

        Component.onCompleted:
        {
            addPage("About", "QuarkTx v 0.0.0.3",  "");
            addPage("Link", "Connect to Tx",  Qt.resolvedUrl("qrc:/qml/Link.qml"));
            addPage("System", "EEPROM, Dump, free memory ...",  Qt.resolvedUrl("qrc:/qml/System.qml"));
            addPage("Sensors", "Calibrate, trim, simulate ...",  Qt.resolvedUrl("qrc:/qml/Sensors.qml"));
            addPage("Models", "Change current, neutral, reverse",  Qt.resolvedUrl("qrc:/qml/Models.qml"));
            addPage("Radio Control Language", "Load/save EEPROM",  Qt.resolvedUrl("qrc:/qml/Rcl.qml"));
        }
    }
}
