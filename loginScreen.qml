import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Controls 1.3

Rectangle {
    id: loginForm
    //anchors.horizontalCenter: parent.horizontalCenter

    Grid {
        id: grid1
        x: 0
        width: 200
        height: 400
        anchors.top: parent.top
        anchors.topMargin: 245
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 20
        rows: 3
        columns: 1

        TextField {
            id: username
            objectName: "login.user"
            width: 200
            height: 32

            placeholderText: qsTr("Username")
        }

        TextField {
            id: password
            objectName: "login.pass"
            width: 200
            height: 32

            placeholderText: qsTr("Password")
        }

        Button {
            id: loginButton
            width: 200
            height: 32
            text: qsTr("Login")
            onClicked:
            {
                qmlInterface.onLoginButton();
            }
        }
    }
}
