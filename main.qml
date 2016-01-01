import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Controls 1.3

ApplicationWindow {
    id: mainWindow
    width: 480
    height: 800
    title: qsTr("VejamQML")
    maximumWidth: 480
    maximumHeight: 800
    visible: true

    Loader {
        id: myLoader
        anchors.horizontalCenter: parent.horizontalCenter
          objectName: "qml.Loader"
          //source: "qrc:/loginScreen.qml"
          source: "qrc:/liveScreen.qml"
      }
}
