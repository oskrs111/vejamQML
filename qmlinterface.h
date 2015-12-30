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

    QObject* getQmlCamera();

private:
    QQmlApplicationEngine* p_engine;
    QtkVideoServer* p_videoSource;
    qtkImageProvider* p_imageProvider;
    QTimer* p_timer;
    quint8 alert(QString message);


signals:

public slots:
    void onLoginButton();
    void onFrameUpdated();
    void onTimer();
};

#endif // QMLINTERFACE_H