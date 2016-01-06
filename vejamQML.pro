TEMPLATE = app

QT += qml quick multimedia

SOURCES += main.cpp \
    qmlinterface.cpp \
    ./../qtkApplicationParameters/qtkapplicationparameters.cpp \
    qtkVideoServer/qtkVideoServer.cpp \
    qtkImageProvider/qtkImageProvider.cpp \
    qtkVideoServer/qtkVideoFilter.cpp \
    ../qtkHttpServer/qtkHttpServer.cpp \
    ../qtkHttpServer/qtkMjpgStreamer.cpp \
    ../qtkHttpServer/qtkJsRpcServer.cpp \
    ../qtkHttpServer/qtkRtpCommand_.cpp

RC_ICONS = res/vejam_toolbar_h48.ico
RESOURCES += qml.qrc
DEFINES += VEJAM_NO_GUI

android {
#QT += androidextras
DEFINES += ANDROID_PLATFORM
}

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

INCLUDEPATH += qtkVideoServer \
            ./../qtkApplicationParameters \
            ./../qtkHttpServer \
            config \
            rpcCommands \
            qtkImageProvider


HEADERS += \
    qmlinterface.h \
    qtkVideoServer/qtkVideoServer.h \
    ./../qtkApplicationParameters/qtkapplicationparameters.h \
    main.h \
    qtkImageProvider/qtkImageProvider.h \
    qtkVideoServer/qtkVideoFilter.h \
    ../config/qtkHttpServerConfig.h \
    ../qtkHttpServer/qtkHttpServer.h \
    ../qtkHttpServer/qtkMjpgStreamer.h \
    ../qtkHttpServer/qtkJsRpcServer.h \
    ../qtkHttpServer/qtkHttpCommon.h \
    ../rpcCommands/qtkRtpCommandsInit.h \
    ../qtkHttpServer/qtkRtpCommand_.h \
    rpcCommands/qtkRtpCommand_Test.h \
    rpcCommands/qtkRtpCommand_Settings.h \
    rpcCommands/qtkRtpCommand_headers.h

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
