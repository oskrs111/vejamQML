import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Controls 1.3
import QtMultimedia 5.5
import qtkvideofilter.uri 1.0

Rectangle {
    id: liveForm   
    width: 480
    height: 800

     Grid {
         id: gridTop
         x: 0
         height: 50
         clip: false
         visible: true
         spacing: 0
         anchors.topMargin: 0
         columns: 2
         anchors.top: parent.top
         width: parent.width

         Text {
             id: textTitle
             text: qsTr("VejamQML")
             font.family: "Verdana"
             anchors.top: parent.top
             anchors.topMargin: 18
             anchors.left: parent.left
             anchors.leftMargin: 19
             font.pixelSize: 12
         }

         Text {
             id: textStat
             objectName: "text.stat"
             text: qsTr("|")
             anchors.leftMargin: 13
             anchors.left: textTitle.right
             anchors.top: parent.top
             anchors.topMargin: 18
             font.family: "Verdana"
             font.pixelSize: 12
         }


         Image {
             id: image1
             objectName: "live.image"
             width: 48
             height: 48
             anchors.right: parent.right
             anchors.rightMargin: 0
             fillMode: Image.Stretch
             source: "image://imageProvider/frame.255"
         }
     }

    Grid {
        id: grid
        x: 0
        width: parent.width
        height: 60
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: vout.bottom
        anchors.topMargin: 10
        spacing: 0
        rows: 1
        columns: 2



        Column {
            id: column1
            width: (parent.width /2)
            height: minimizeButton.height + 5

            Button {
                id: minimizeButton
                width: parent.width - 10
                height: 43
                text: qsTr("Hide")
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                //anchors.horizontalCenter: grid2.columns.
                onClicked:
                {
                    qmlInterface.onMinimizeButton();
                }
            }
        }

        Column {
            id: column2
            width: column1.width
            height: column1.height

            Button {
                id: setupButton
                width: minimizeButton.width
                height: minimizeButton.height
                text: qsTr("Setup")
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                onClicked:
                {
                    qmlInterface.onSetupButton();
                }
            }
        }
    }

    Camera {
        id: camera
        objectName: "camera.device"
    }

    VideoOutput{
        id: vout
        y: 49
        source: camera
        filters: [qtkFilter]
        width: parent.width
        height: parent.width / 1.3
        anchors.horizontalCenter: parent.horizontalCenter
    }

    VideoFilter{
    id: qtkFilter
    objectName: "videoFilter.device"
    }

}
