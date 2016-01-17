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
            addPage("Calibrate", "Setup",  Qt.resolvedUrl("xx.qml"));
            addPage("Trim", "Setup",  Qt.resolvedUrl("xx.qml"));
            addPage("Min", "Setup",  Qt.resolvedUrl("xx.qml"));
            addPage("Max", "Setup",  Qt.resolvedUrl("xx.qml"));
            addPage("Simulate", "Setup",  Qt.resolvedUrl("xx.qml"));
        }
    }
}
