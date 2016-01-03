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
    this->m_mirrorSetting = this->loadParam(QString("video"),QString("mirror-setting")).toInt();
    this->m_widthScale = this->loadParam(QString("video"),QString("max-width")).toInt();
    this->m_scaleMode = this->loadParam(QString("video"),QString("scale-mode")).toInt();
    this->m_videoQuality = this->loadParam(QString("video"),QString("quality")).toInt();
    this->m_frameDrop = this->loadParam(QString("video"),QString("frame-drop")).toInt();
    this->m_streamAlias = this->loadParam(QString("aplicacion"),QString("streamming-alias"));
    this->m_streamAliasShowTitle = this->loadParam(QString("aplicacion"),QString("osd-show-title")).toInt();
    this->m_streamAliasShowTime = this->loadParam(QString("aplicacion"),QString("osd-show-time")).toInt();
    this->m_streamAliasXpos = this->loadParam(QString("aplicacion"),QString("osd-x-position")).toInt();
    this->m_streamAliasYpos = this->loadParam(QString("aplicacion"),QString("osd-y-position")).toInt();
    this->m_streamAliasFont = this->loadParam(QString("aplicacion"),QString("osd-text-font"));
    this->m_streamAliasFontSize = this->loadParam(QString("aplicacion"),QString("osd-text-font-size")).toInt();
    this->m_streamAliasFontWeight = this->loadParam(QString("aplicacion"),QString("osd-text-font-weight")).toInt();
    this->m_streamAliasFontColor = this->loadParam(QString("aplicacion"),QString("osd-text-font-color")).toInt(0,16);
}

void QtkVideoServer::setVideoFilter(qtkVideoFilter* videoFilter)
{
    this->m_videoFilter = videoFilter;
    connect(this->m_videoFilter, SIGNAL(frameReady(QImage)), this, SLOT(OnFilterCapturedImage(QImage)));
}

void QtkVideoServer::setCamera(QCamera* camera)
{
    this->m_camera = camera;
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

    m_mutexA.lock();
    if(this->m_widthScale)
    {
        this->m_currentFrame = frame.copy(QRect()).mirrored(mHor, mVer).scaledToWidth(this->m_widthScale, (Qt::TransformationMode)this->m_scaleMode);
    }
    else
    {
        this->m_currentFrame = frame.copy(QRect()).mirrored(mHor, mVer);
    }

    QDateTime time =  QDateTime::currentDateTime();
    QFont font = QFont(this->m_streamAliasFont, this->m_streamAliasFontSize, this->m_streamAliasFontWeight);
    QPen pen = QPen(this->m_streamAliasFontColor);
    if((this->m_streamAliasShowTime > 0) && (this->m_streamAliasShowTitle > 0))
    {
        if(this->m_streamAliasShowTime > 0)
        {
            this->m_streamAlias.append(time.toString(" dd.MM.yyyy - hh:mm:ss.zzz"));
        }

        this->osdTextWrite(&this->m_currentFrame,
                           this->m_streamAlias,
                           this->m_streamAliasXpos,
                           this->m_streamAliasYpos, font, pen);
    }
    m_mutexA.unlock();
    emit frameUpdated();
}

QImage QtkVideoServer::currentFrame2Image()
{
    QImage lastFrame;
    m_mutexA.lock();
    lastFrame = this->m_currentFrame.copy(QRect());
    m_mutexA.unlock();
    return lastFrame;
}

QByteArray QtkVideoServer::currentFrame2Base64Jpeg()
{
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QBuffer::WriteOnly);
    m_mutexA.lock();		
    this->m_currentFrame.save( &buffer, "JPG", this->m_videoQuality);
    m_mutexA.unlock();
    buffer.close();
	return ba.toBase64();
}

QByteArray QtkVideoServer::currentFrame2ByteArrayJpeg()
{
    QByteArray ba;
    QBuffer buffer(&ba);	
    buffer.open(QBuffer::WriteOnly);    
    m_mutexA.lock();	
    this->m_currentFrame.save( &buffer, "JPG", this->m_videoQuality);
    m_mutexA.unlock();
    buffer.close();
    return ba;
}

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
