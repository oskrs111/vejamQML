import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Controls 1.3
import QtMultimedia 5.5

Rectangle {
    id: liveForm
    anchors.horizontalCenter: parent.horizontalCenter

    Grid {
        id: grid2
        x: 0
        width: 147
        height: 62
        anchors.top: parent.top
        anchors.topMargin: 388
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 20
        rows: 1
        columns: 2



        Button {
            id: minimizeButton
            width: 64
            height: 32
            text: qsTr("Hide")
            onClicked:
            {
                qmlInterface.onMinimizeButton();
            }
        }

        Button {
            id: setupButton
            width: 64
            height: 32
            text: qsTr("Setup")
            onClicked:
            {
                qmlInterface.onSetupButton();
            }
        }
    }

    Image {
        id: image1
        objectName: "live.image"
        x: 130
        y: 228
        width: 164
        height: 123
        anchors.horizontalCenterOffset: 5
        anchors.horizontalCenter: parent.horizontalCenter
        fillMode: Image.PreserveAspectFit
        source: "image://imageProvider/frame.255"
    }

    Camera {
        id: camera
        objectName: "camera.device"
    }

    VideoOutput{
        id: vout
        source: camera
        width: 200
        height: 200
        anchors.horizontalCenter: parent.horizontalCenter

    }

}
