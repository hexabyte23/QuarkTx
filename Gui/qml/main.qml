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
