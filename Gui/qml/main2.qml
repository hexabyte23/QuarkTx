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
import QtMultimedia 5.0

ApplicationWindow {
    id: osrc_app
    visible: true
    width: 640
    height: 480
    title: qsTr("QuarkTx")

    Header {
        modelname: "Supra pro competition"
    }

    Item {
        x: 60
        y: 88
        //width: 500
        //height: 400
        Loader {
            id: load
        }

    }

    MainContent {
        id: main_window
    }

    TrimStatus {

    }

    Footer {

    }

    Component.onCompleted: {
        console.log("ready")
    }
}
