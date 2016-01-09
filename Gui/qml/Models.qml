
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
            addExample("Change current model", "",  Qt.resolvedUrl("bezierCurve/bezierCurve.qml"));
            addExample("Neutral", "Setup",  Qt.resolvedUrl("bezierCurve/bezierCurve.qml"));
            addExample("Reverse", "Setup",  Qt.resolvedUrl("quadraticCurveTo/quadraticCurveTo.qml"));
        }
    }
}
