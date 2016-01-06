import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Controls 1.3

ApplicationWindow {
    id: mainWindow
    readonly property real _dip: 1//Screen.pixelDensity / (96 / 25.4) // DPI norm in mm
    //width: (platform.os === "android" ? Screen.desktopAvailableWidth*_dip : 460)
    //height: (platform.os === "android" ? Screen.desktopAvailableWidth*_dip : 800)

    Component.onCompleted:{
        if(Qt.platform.os === "android"){
            mainWindow.width = Screen.desktopAvailableWidth*_dip;
            mainWindow.height = Screen.desktopAvailableHeight*_dip;
            console.log("android...")
        }
        else{
            mainWindow.width = 460*_dip;
            mainWindow.height = 800*_dip;
            console.log("non-android...")
        }
     }

    title: qsTr("VejamQML")
    //maximumWidth: 480*_dip
    //maximumHeight: 800*_dip
    visible: true

    Loader {
        id: myLoader
        anchors.horizontalCenter: parent.horizontalCenter
          objectName: "qml.Loader"
          //source: "qrc:/loginScreen.qml"
          source: "qrc:/liveScreen.qml"
      }
}
