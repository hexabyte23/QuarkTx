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
    height: 480
    width: 320

    LauncherList
    {
        id: ll
        anchors.fill: parent

        Component.onCompleted:
        {
            addPage("Load from EEPROM", "",  Qt.resolvedUrl("xx.qml"));
            addPage("Save to EEPROM", "",  Qt.resolvedUrl("xx.qml"));
            addPage("Dump", "Sensor, Model, EEPROM",  Qt.resolvedUrl("xx.qml"));
            addPage("Display free memory", "",  Qt.resolvedUrl("xx.qml"));
            addPage("Display input", "Sensors",  Qt.resolvedUrl("xx.qml"));
            addPage("Display output", "Channels",  Qt.resolvedUrl("xx.qml"));
            addPage("Toggle Tx mode", "Transmit, Debug",  Qt.resolvedUrl("xx.qml"));
            addPage("Reset", "Clean calibration, Tx mode & models ...",  Qt.resolvedUrl("xx.qml"));
            addPage("Terminal", "Display RS 232 link content",  Qt.resolvedUrl("xx.qml"));
        }
    }
}
