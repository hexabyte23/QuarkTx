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

Rectangle
{
    id: container
    property Item pageItem
    width: ListView.view.width
    height: button.implicitHeight + 23

    gradient: Gradient
    {
        GradientStop {position: 0; color: "#eee"}
        GradientStop {position: 1; color: "#ddd"}
    }

    Image
    {
        id: image
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 16
        visible: (url != "") ? true : false
        opacity: 0.7
        source: "qrc:/artwork/next.png"
    }

    Item
    {
        id: button
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.right:image.left
        implicitHeight: col.height
        height: implicitHeight
        width: buttonLabel.width + 20

        MouseArea
        {
            id: mouseArea
            anchors.fill: parent
            onClicked: pageItem.pageUrl = url
            hoverEnabled: true
        }

        Column
        {
            id: col
            spacing: 2
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width

            Text
            {
                id: buttonLabel
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.right: parent.right
                anchors.rightMargin: 10
                text: name
                color: "black"
                font.pixelSize: 22
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                styleColor: "white"
                style: Text.Raised

            }

            Text
            {
                id: buttonLabel2
                anchors.left: parent.left
                anchors.leftMargin: 10
                text: description
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                color: "#666"
                font.pixelSize: 12
            }
        }
    }

    Rectangle
    {
        id:justLine
        height: 1
        color: "#ccc"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }
}

