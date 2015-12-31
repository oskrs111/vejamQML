import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Controls 1.3

ApplicationWindow {
    id: window1
    visible: true
    width: 360
    height: 480

    Loader {
        id: myLoader
        anchors.horizontalCenter: parent.horizontalCenter
          objectName: "qml.Loader"
          //source: "qrc:/loginScreen.qml"
          source: "qrc:/liveScreen.qml"
      }
}
