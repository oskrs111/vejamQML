#ifndef QMLINTERFACE_H
#define QMLINTERFACE_H
#include <QObject>
#include <QQuickItem>
#include <QQmlContext>
#include <QQmlProperty>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QTimer>
#include "qtkvideoserver.h"
#include "qtkImageProvider.h"

//QQuickImageProvider
//http://www.lothlorien.com/kf6gpe/?p=234 -> ImageProvider
//http://stackoverflow.com/questions/30266881/what-is-the-right-way-to-use-qquickimageprovider
//http://wisol.ch/w/articles/2014-12-15-qt-signal-slots-qml-cpp/


class qmlInterface : public QObject
{
    Q_OBJECT
public:
    explicit qmlInterface(QObject *parent = 0);
    void setEngine(QQmlApplicationEngine* engine);
    void setVideoSource(QtkVideoServer* videoSource);
    void setImageProvider(qtkImageProvider* imageProvider);
#ifdef WINDOWS_PLATFORM
    void setApplication(QApplication* app);
#endif
    void updateTextState();
    void writeLog(QString log);

    QObject* getQmlCamera();
    QObject* getQmlVideoFilter();

private:
    QQmlApplicationEngine* p_engine;
    QtkVideoServer* p_videoSource;
    qtkImageProvider* p_imageProvider;
    QTimer* p_timer;
    QMutex m_mutexA;
    quint8 alert(QString message);


signals:

public slots:
    void _TRACE(QString log);
    void onLoginButton();
    void onRunButton();
    void onFrameUpdated();
    void onTimer();
    void onRemoteRequest(int type);
};

#endif // QMLINTERFACE_H
