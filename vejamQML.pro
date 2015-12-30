TEMPLATE = app

QT += qml quick multimedia

SOURCES += main.cpp \
    qmlinterface.cpp \
    qtkApplicationParameters/qtkapplicationparameters.cpp \
    qtkVideoServer/qtkCaptureBuffer.cpp \
    qtkVideoServer/qtkVideoServer.cpp \
    qtkImageProvider/qtkImageProvider.cpp

RESOURCES += qml.qrc
DEFINES += VEJAM_NO_GUI

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
            qtkApplicationParameters \
            qtkImageProvider


HEADERS += \
    qmlinterface.h \
    qtkVideoServer/qtkCaptureBuffer.h \
    qtkVideoServer/qtkVideoServer.h \
    qtkApplicationParameters/qtkapplicationparameters.h \
    main.h \
    qtkImageProvider/qtkImageProvider.h

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
