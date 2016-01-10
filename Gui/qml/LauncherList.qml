

import QtQuick 2.0

Rectangle
{
    color: "#eee"

    function addPage(name, desc, url)
    {
        myModel.append({"name":name, "description":desc, "url":url})
    }

    function hidePage()
    {
        ei.visible = false;
    }

    ListView
    {
        clip: true
        delegate: SimpleLauncherDelegate {pageItem: ei}
        model: ListModel {id:myModel}
        anchors.fill: parent
    }

    Item
    {
        id: ei
        visible: false
        clip: true
        property url pageUrl
        anchors.fill: parent
        anchors.bottomMargin: 40

        onPageUrlChanged: visible = (pageUrl == '' ? false : true);

        Rectangle
        {
            id: bg
            anchors.fill: parent
            color: "white"
        }

        MouseArea
        {
            anchors.fill: parent
            enabled: ei.visible
            //Eats mouse events
        }

        Loader
        {
            focus: true
            source: ei.pageUrl
            anchors.fill: parent
        }
    }

    Rectangle
    {
        id: bar
        visible: ei.visible
        anchors.bottom: parent.bottom
        width: parent.width
        height: 40

        Rectangle
        {
            height: 1
            color: "#ccc"
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
        }

        Rectangle
        {
            height: 1
            color: "#fff"
            anchors.top: parent.top
            anchors.topMargin: 1
            anchors.left: parent.left
            anchors.right: parent.right
        }

        gradient: Gradient
        {
            GradientStop { position: 0 ; color: "#eee" }
            GradientStop { position: 1 ; color: "#ccc" }
        }

        MouseArea
        {
            anchors.fill: parent
            enabled: ei.visible
            //Eats mouse events
        }

        Image
        {
            id: back
            source: "qrc:/artwork/back.png"
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 2
            anchors.left: parent.left
            anchors.leftMargin: 16

            MouseArea
            {
                id: mouse
                hoverEnabled: true
                anchors.centerIn: parent
                width: 38
                height: 31
                anchors.verticalCenterOffset: -1
                onClicked: ei.pageUrl = ""

                Rectangle
                {
                    anchors.fill: parent
                    opacity: mouse.pressed ? 1 : 0
                    Behavior on opacity { NumberAnimation{ duration: 100 }}
                    border.color: "darkgray"
                    antialiasing: true
                    radius: 4

                    gradient: Gradient
                    {
                        GradientStop { position: 0 ; color: "#22000000" }
                        GradientStop { position: 0.2 ; color: "#11000000" }
                    }
                }
            }
        }
    }
}
