import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Dialogs 1.1

MessageDialog {
    id: messageDialog
    objectName: "messageBox"
    title: "Atención!"
    text: ""
    visible: false

    onAccepted: {
     messageDialog.visible = false
    }
}
