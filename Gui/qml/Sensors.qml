
import QtQuick 2.0

Item
{
    height: 480
    width: 320
    LauncherList
    {
        id: ll
        anchors.fill: parent
        Component.onCompleted: {
            addExample("Calibrate", "Setup",  Qt.resolvedUrl("bezierCurve/bezierCurve.qml"));
            addExample("Trim", "Setup",  Qt.resolvedUrl("quadraticCurveTo/quadraticCurveTo.qml"));
            addExample("Simulate", "Setup",  Qt.resolvedUrl("squircle/squircle.qml"));
        }
    }
}
