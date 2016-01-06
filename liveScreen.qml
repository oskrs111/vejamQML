import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Controls 1.3
import QtMultimedia 5.5
import QtQuick.Controls.Styles 1.4
import qtkvideofilter.uri 1.0

Rectangle {
    id: liveForm
    readonly property real dip: 1//Screen.pixelDensity / (96 / 25.4) // DPI norm in mm
    //width: (platform.os === "android" ? Screen.desktopAvailableWidth*_dip : 460)
    //height: (platform.os === "android" ? Screen.desktopAvailableWidth*_dip : 800)

    Component.onCompleted:{
        if(Qt.platform.os === "android"){
            liveForm.width = Screen.desktopAvailableWidth*_dip;
            liveForm.height = Screen.desktopAvailableHeight*_dip;
        }
        else{
            liveForm.width = 460*_dip;
            liveForm.height = 800*_dip;
        }
    }

    Grid {
         id: gridTop
         x: 0
         height: 50*dip
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
             anchors.topMargin: 18*dip
             anchors.left: parent.left
             anchors.leftMargin: 19*dip
             font.pixelSize: 12*dip
         }

         Text {
             id: textStat
             objectName: "text.stat"
             text: qsTr("|")
             anchors.leftMargin: 13*dip
             anchors.left: textTitle.right
             anchors.top: parent.top
             anchors.topMargin: 18*dip
             font.family: "Verdana"
             font.pixelSize: 12*dip
         }


         Image {
             id: image1
             objectName: "live.image"
             width: 48*dip
             height: 48*dip
             anchors.right: parent.right
             anchors.rightMargin: 0
             fillMode: Image.Pad
             source: "image://imageProvider/frame.255"
         }
     }

    Camera {
        id: camera
        objectName: "camera.device"
        Component.onCompleted:{
            if(Qt.platform.os === "android"){
                camera.Rotation = 90
            }
        }
    }

    VideoOutput{
        id: vout
        y: 49*dip
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

    TextArea {
        id: textArea1
        objectName: "text.log"
        width: parent.width
        height: (liveForm.height - vout.height) / 2
        text: "vejamQML"
        frameVisible: true
        textColor: "#2c2121"
        anchors.top: vout.bottom
        anchors.topMargin: 0
        readOnly: true
        activeFocusOnPress: false
        font.pixelSize: 10
        anchors.horizontalCenter: parent.horizontalCenter

        style: TextAreaStyle {
            backgroundColor: "#ffffff"
        }
    }

}
