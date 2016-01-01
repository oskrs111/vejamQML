#ifndef QTKVIDEOSERVER_H
#define QTKVIDEOSERVER_H
#include <QObject>
#include <QCamera>
#include <QVideoProbe>
#include <QCameraImageCapture>
#include <QImageEncoderSettings>
#include "qtkcapturebuffer.h"
#include "qtkVideoFilter.h"
#include "qtkapplicationparameters.h"   //OSLL: Add global include path...

class QtkVideoServer : public QObject
{
    Q_OBJECT
public:
    explicit QtkVideoServer(QtKApplicationParameters* params, QObject *parent = 0);
    void setVideoFilter(qtkVideoFilter* videoFilter);
    void setCamera(QCamera* camera);

    void startServer();	
    void Capture();
    int getServerState();
    void osdTextWrite(QImage* img, QString osdText, int xPos, int yPos);   

    QImage currentFrame2Image();
    QByteArray currentFrame2Base64Jpeg();
    QByteArray currentFrame2ByteArrayJpeg();

    enum mirrorConfig
    {
        mirrorNone = 0,
        mirrorVertical,
        mirrorHorizontal,
        mirrorAll
    };

private:
    int m_mirrorSetting;
    int m_widthScale;
    int m_scaleMode;
    QImage m_currentFrame;
    QMutex m_mutexA;    
    QCamera* m_camera;
    qtkVideoFilter* m_videoFilter;
    QtKApplicationParameters* m_appParameters;   
    void saveParam(QString groupName, QString paramName, QString paramValue, quint16 order = 0);
    QString loadParam(QString groupName, QString paramName, quint16 order = 0);

signals:
    void frameUpdated();

public slots:    
    void OnFilterCapturedImage(QImage frame);
    void OnDisplayCaptureError(int id,QCameraImageCapture::Error error, QString errorString);
    void OnUpdateCameraState(QCamera::State state);
    void OnDisplayCameraError(QCamera::Error error);
};

struct vjCameraDevice
{
    QByteArray m_name;
    QString m_description;
};


#endif // QTKVIDEOSERVER_H
