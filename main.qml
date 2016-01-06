import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Controls 1.3

ApplicationWindow {
    id: mainWindow
    readonly property real _dip: 1//Screen.pixelDensity / (96 / 25.4) // DPI norm in mm

    Component.onCompleted:{
        if(Qt.platform.os === "android"){
            mainWindow.width = Screen.desktopAvailableWidth*_dip;
            mainWindow.height = Screen.desktopAvailableHeight*_dip;
            console.log("android...")
        }
        else{
            mainWindow.width = 460*_dip;
            mainWindow.maximumWidth = 460*_dip;
            mainWindow.minimumWidth = 460*_dip;
            mainWindow.height = 800*_dip;
            mainWindow.maximumHeight = 800*_dip;
            mainWindow.minimumHeight = 800*_dip;
            console.log("non-android...")
        }
     }

    title: qsTr("VejamQML")
    visible: true

    Loader {
        id: myLoader
        anchors.horizontalCenter: parent.horizontalCenter
          objectName: "qml.Loader"
          //source: "qrc:/loginScreen.qml"
          source: "qrc:/liveScreen.qml"
      }
}
