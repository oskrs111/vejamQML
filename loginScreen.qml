import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.4

Rectangle {
    id: loginForm
    property real dip: 1//Screen.pixelDensity / (96 / 25.4) // DPI norm in mm
    Component.onCompleted:{
    qmlInterface._TRACE("Device pixel width: " + Screen.width);
    qmlInterface._TRACE("Device pixel height: " + Screen.height);
    qmlInterface._TRACE("Device pixel ratio: " + Screen.devicePixelRatio);
    qmlInterface._TRACE("Device pixel density: " + Screen.pixelDensity);
    qmlInterface._TRACE("Device pixel logical density: " + Screen.logicalPixelDensity);

    if(Qt.platform.os === "android"){
        loginForm.width = Screen.desktopAvailableWidth;
        loginForm.height = Screen.desktopAvailableHeight;
        loginForm.dip =  Screen.pixelDensity / (96 / 25.4);
        loginForm.update();

    }
    else{
        loginForm.width = 460;
        loginForm.height = 800;
    }

    }
//image1
    Image {
        id: image1
        x: 50*dip
        width: 100*dip
        height: 48*dip
        anchors.top: parent.top
        anchors.topMargin: 50*dip
        fillMode: Image.PreserveAspectFit
        source: "res/vejam_h48_gqfa_1.0.png"
    }
//image1 - END

//grid1
    Grid {
        id: grid1
        x: 0
        width: 200*dip
        height: 150*dip
        anchors.topMargin: 50*dip
        anchors.top: image1.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 20*dip
        rows: 3
        columns: 1

        TextField {
            id: username
            objectName: "login.user"
            width: 200*dip
            height: 32*dip
            font.family: "Verdana"
            font.pixelSize: 12*dip
            placeholderText: qsTr("Username")
            style: TextFieldStyle {
                   textColor: "black"
                   background: Rectangle {
                       //radius: 2
                       border.color: "#333"
                       border.width: 1
                   }
               }
        }

        TextField {
            id: password
            objectName: "login.pass"
            width: 200*dip
            height: 32*dip
            font.family: "Verdana"
            font.pixelSize: 12*dip
            placeholderText: qsTr("Password")
            style: TextFieldStyle {
                   textColor: "black"
                   background: Rectangle {
                       //radius: 2
                       border.color: "#333"
                       border.width: 1
                   }
               }
        }

        Button {
            id: loginButton
            width: 200*dip
            height: 32*dip
            text: qsTr("Login")            
            onClicked:
            {
                qmlInterface.onLoginButton();
            }
            style: ButtonStyle {
               background: Rectangle {
               border.width: control.activeFocus ? 2 : 1
               border.color: "#888"
               radius: 4
               gradient: Gradient {
                   GradientStop { position: 0 ; color: control.pressed ? "#ccc" : "#eee" }
                   GradientStop { position: 1 ; color: control.pressed ? "#aaa" : "#ccc" }
                    }
                }
            }
        }
    }
//grid1 - END
//grid2

        Grid {
            id: grid2
            x: 0*dip
            width: 200*dip
            height: 50*dip
            anchors.top: grid1.bottom
            anchors.topMargin: 25*dip
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 20*dip
            rows: 3
            columns: 1

            Button {
                id: runButton
                width: 200*dip
                height: 32*dip
                text: qsTr("...or use on LAN")
                onClicked:
                {
                    qmlInterface.onRunButton();
                }
                style: ButtonStyle {
                   background: Rectangle {
                   border.width: control.activeFocus ? 2 : 1
                   border.color: "#888"
                   radius: 4
                   gradient: Gradient {
                       GradientStop { position: 0 ; color: control.pressed ? "#ccc" : "#eee" }
                       GradientStop { position: 1 ; color: control.pressed ? "#aaa" : "#ccc" }
                        }
                    }
                }
            }
        }

//grid2 - END

}
