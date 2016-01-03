import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Controls 1.3

ApplicationWindow {
    id: mainWindow
    readonly property real _dip: 1//Screen.pixelDensity / (96 / 25.4) // DPI norm in mm
    width: Screen.desktopAvailableWidth*_dip
    height: Screen.desktopAvailableHeight*_dip
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
