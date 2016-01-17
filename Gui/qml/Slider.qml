
import QtQuick 2.0

Item
{
    id: slider
    height: 26
    width: 320

    property real min: 0
    property real max: 1
    property real value: min+(max-min)*mouseArea.value
    property real init: min+(max-min)/2
    property string name: "Slider"
    property color color: "#0066cc"
    property real minLabelWidth: 45

    Component.onCompleted: setValue(init)

    function setValue(v)
    {
       if (min < max)
          handle.x = Math.round(v/(max-min)*
                                (mouseArea.drag.maximumX - mouseArea.drag.minimumX)
                                + mouseArea.drag.minimumX);
    }

    Rectangle
    {
        id:sliderLabel
        anchors.left: parent.left
        anchors.leftMargin: 16
        height: childrenRect.height
        width: Math.max(minLabelWidth, childrenRect.width)
        anchors.verticalCenter: parent.verticalCenter
        Text
        {
            text: slider.name + ":"
            font.pointSize: 12
            color: "#333"
        }
    }

    Rectangle
    {
        id: rightSlide
        width: parent.width - 8 - sliderLabel.width
        height: 7
        color: "#eee"
        radius: 3
        antialiasing: true
        border.width: 1
        border.color: Qt.darker(color, 1.2)
        anchors.left: sliderLabel.right
        anchors.right: parent.right
        anchors.leftMargin: 10
        anchors.rightMargin: 44
        anchors.verticalCenter: parent.verticalCenter

        Rectangle
        {
            id: leftSlide
            height: parent.height
            color: slider.color
            radius: parent.radius
            antialiasing: true
            border.width: 1
            border.color: Qt.darker(color, 1.3)
            anchors.left: parent.left
            anchors.right: handle.horizontalCenter
            opacity: 0.5
        }

        Image
        {
            id: handle
            source: "qrc:/artwork/slider_handle.png"
            anchors.verticalCenter: parent.verticalCenter
            MouseArea
            {
                id: mouseArea
                anchors.fill: parent
                anchors.margins: -4
                drag.target: parent
                drag.axis: Drag.XAxis
                drag.minimumX: Math.round(-handle.width/2 + 3)
                drag.maximumX: Math.round(rightSlide.width - handle.width/2 - 3)
                property real value: (handle.x - drag.minimumX)/(drag.maximumX - drag.minimumX)
            }
        }
    }

    Text
    {
        id: valueLabel
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        text: value
        property real value: mouseArea.value

        onValueChanged:
        {
            valueLabel.text = Math.round(value*(max-min))
        }
    }
}
