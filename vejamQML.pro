TEMPLATE = app

QT += qml quick multimedia

SOURCES += main.cpp \
    qmlinterface.cpp \
    ./../qtkApplicationParameters/qtkapplicationparameters.cpp \
    qtkVideoServer/qtkVideoServer.cpp \
    qtkImageProvider/qtkImageProvider.cpp \
    qtkVideoServer/qtkVideoFilter.cpp \
    ../qtkHttpServer/qtkHttpServer.cpp \
    ../qtkHttpServer/qtkMjpgStreamer.cpp

RESOURCES += qml.qrc
DEFINES += VEJAM_NO_GUI

android {
#QT += androidextras
#DEFINES += ANDROID_PLATFORM
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
            ./../config \
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
    ../qtkHttpServer/qtkMjpgStreamer.h

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
