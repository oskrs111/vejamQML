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
    void loadSettings();
    int getServerState();
    void osdTextWrite(QImage* img, QString osdText, int xPos, int yPos, QFont font, QPen pen);

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
//OSLL: Variables for settings
    int m_mirrorSetting;
    int m_widthScale;
    int m_scaleMode;
    int m_videoQuality;    
    int m_streamAliasXpos;
    int m_streamAliasYpos;
    int m_streamAliasShowTitle;
    int m_streamAliasShowTime;
    int m_streamAliasFontSize;
    int m_streamAliasFontWeight;
    int m_streamAliasFontColor;
    QString m_streamAlias;
    QString m_streamAliasFont;    
    void updateVideoFilterSettings();
    void updateNativeCameraSettings();
//OSLL: Variables for settings - END

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
    void OnFilterReady();
};

struct vjCameraDevice
{
    QByteArray m_name;
    QString m_description;
};


#endif // QTKVIDEOSERVER_H
