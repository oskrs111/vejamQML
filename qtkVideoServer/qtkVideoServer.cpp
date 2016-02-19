#include <QBuffer>
#include <QPainter>
#include <QDateTime>
#include <QImageEncoderControl>
#include "qtkvideoserver.h"

//http://blog.csdn.net/kl222/article/details/23187693

QtkVideoServer::QtkVideoServer(QtKApplicationParameters *params, QObject *parent) :
    QObject(parent)
{
    this->m_camera = 0;
    this->m_videoFilter = 0;
    this->m_appParameters = params;
    this->loadSettings();
}

void QtkVideoServer::loadSettings()
{      
    this->m_streamAlias = this->loadParam(QString("aplicacion"),QString("streamming-alias"));
    this->m_scaleMode = this->loadParam(QString("video"),QString("scale-mode")).toInt();
    this->m_videoQuality = this->loadParam(QString("video"),QString("quality")).toInt();
    this->m_mirrorSetting = this->loadParam(QString("video"),QString("mirror-setting")).toInt();
    this->m_streamAliasShowTitle = this->loadParam(QString("video"),QString("osd-show-title")).toInt();
    this->m_streamAliasShowTime = this->loadParam(QString("video"),QString("osd-show-time")).toInt();
    this->m_streamAliasXpos = this->loadParam(QString("video"),QString("osd-x-position")).toInt();
    this->m_streamAliasYpos = this->loadParam(QString("video"),QString("osd-y-position")).toInt();
    this->m_streamAliasFont = this->loadParam(QString("video"),QString("osd-text-font"));
    this->m_streamAliasFontSize = this->loadParam(QString("video"),QString("osd-text-font-size")).toInt();
    this->m_streamAliasFontWeight = this->loadParam(QString("video"),QString("osd-text-font-weight")).toInt();
    this->m_streamAliasFontColor = this->loadParam(QString("video"),QString("osd-text-font-color")).toInt(0,16);
    this->updateVideoFilterSettings();
    this->updateNativeCameraSettings();
}

void QtkVideoServer::updateVideoFilterSettings()
{
    if(this->m_videoFilter == 0) return;
    this->m_videoFilter->setRotationAngle(this->loadParam(QString("video"),QString("rotation-angle")).toInt());
    this->m_videoFilter->setFrameDropper(this->loadParam(QString("video"),QString("frame-drop")).toInt());
    this->m_videoFilter->setScaleX(this->loadParam(QString("video"),QString("scale-width")).toInt());
    this->m_videoFilter->setScaleY(this->loadParam(QString("video"),QString("scale-height")).toInt());
}

void QtkVideoServer::updateNativeCameraSettings()
{
    if(this->m_camera == 0) return;
    #ifdef ANDROID_PLATFORM
    int torch = QCameraExposure::FlashOff;
    if(this->loadParam(QString("video-extras"),QString("torch")).toInt() > 0)
    {
        torch = QCameraExposure::FlashTorch;
    }
    this->m_camera->exposure()->setFlashMode((QCameraExposure::FlashMode)torch);
    #endif
}

void QtkVideoServer::setVideoFilter(qtkVideoFilter* videoFilter)
{
    this->m_videoFilter = videoFilter;
    connect(this->m_videoFilter, SIGNAL(frameReady(QImage)), this, SLOT(OnFilterCapturedImage(QImage)));
    connect(this->m_videoFilter, SIGNAL(filterReady()), this, SLOT(OnFilterReady()));
}

void QtkVideoServer::setCamera(QCamera* camera)
{
    this->m_camera = camera;
    this->updateNativeCameraSettings();
}

int QtkVideoServer::getServerState()
{
    if(this->m_camera)
    {
        return this->m_camera->state();
    }
    else return 0;
}

void QtkVideoServer::OnFilterCapturedImage(QImage frame)
{
    bool mHor = false;
    bool mVer = false;

    switch(this->m_mirrorSetting)
    {
       case mirrorVertical:
            mVer = true;
            break;

       case mirrorHorizontal:
           mHor = true;
           break;

       case mirrorAll:
            mVer = true;
            mHor = true;
           break;

       case mirrorNone:
       default:
       break;
    }

    this->m_mutexA.lock();
    this->m_currentFrame = frame.copy(QRect()).mirrored(mHor, mVer);

    QDateTime time =  QDateTime::currentDateTime();
    QFont font = QFont(this->m_streamAliasFont, this->m_streamAliasFontSize, this->m_streamAliasFontWeight);
    QPen pen = QPen(this->m_streamAliasFontColor);
    if((this->m_streamAliasShowTime > 0) || (this->m_streamAliasShowTitle > 0))
    {
        QString frameText;

        if(this->m_streamAliasShowTitle > 0)
        {
            frameText.append(this->m_streamAlias);
        }

        if(this->m_streamAliasShowTime > 0)
        {            
            frameText.append(time.toString(" dd.MM.yyyy - hh:mm:ss.zzz"));
        }

        if(frameText.size() > 0)
        {
            this->osdTextWrite(&this->m_currentFrame,
                               frameText,
                               this->m_streamAliasXpos,
                               this->m_streamAliasYpos, font, pen);
        }
    }
    this->m_mutexA.unlock();
    emit frameUpdated();
}

QImage QtkVideoServer::currentFrame2Image()
{
    QImage lastFrame;
    this->m_mutexA.lock();
    lastFrame = this->m_currentFrame.copy(QRect());
    this->m_mutexA.unlock();
    return lastFrame;
}

QByteArray QtkVideoServer::currentFrame2Base64Jpeg()
{
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QBuffer::WriteOnly);
    this->m_mutexA.lock();
    this->m_currentFrame.save( &buffer, "JPG", this->m_videoQuality);
    this-> m_mutexA.unlock();
    buffer.close();
	return ba.toBase64();
}

QByteArray QtkVideoServer::currentFrame2ByteArrayJpeg()
{
    QByteArray ba;
    QBuffer buffer(&ba);	
    buffer.open(QBuffer::WriteOnly);    
    this->m_mutexA.lock();
    this->m_currentFrame.save( &buffer, "JPG", this->m_videoQuality);
    this->m_mutexA.unlock();
    buffer.close();
    return ba;
}

void QtkVideoServer::saveParam(QString groupName, QString paramName, QString paramValue, quint16 order)
{
    if(this->m_appParameters)
    {
        this->m_appParameters->saveParam(groupName, paramName,  paramValue, order);
    }
}

QString QtkVideoServer::loadParam(QString groupName, QString paramName, quint16 order)
{
    if(this->m_appParameters)
    {
        return this->m_appParameters->loadParam(groupName, paramName, order);
    }
    return 0;
}

void QtkVideoServer::osdTextWrite(QImage* img, QString osdText, int xPos, int yPos, QFont font, QPen pen)
{
    QPainter p(img);
    p.setPen(pen);
    p.setFont(font);
    p.drawText(QPoint(xPos, yPos), osdText);
}

//****************************************************************************

void QtkVideoServer::OnUpdateCameraState(QCamera::State state)
{
    qDebug() << "QtkVideoServer::OnUpdateCameraState( " << state << " )";
}

void QtkVideoServer::OnDisplayCameraError(QCamera::Error error)
{
    qDebug() << "QtkVideoServer::OnUpdateCameraState( " << error << " )";
}

void QtkVideoServer::OnDisplayCaptureError(int id,QCameraImageCapture::Error error, QString errorString)
{
    Q_UNUSED(id);
    Q_UNUSED(error);
    qDebug() << "QtkVideoServer::OnDisplayCaptureError( " << errorString << " )";
}

void QtkVideoServer::OnFilterReady()
{
    qDebug() << "[videoServer] VideoFilter ready!";
    this->updateVideoFilterSettings();
}
