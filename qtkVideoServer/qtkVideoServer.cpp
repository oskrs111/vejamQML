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
    this->m_mirrorSetting = this->loadParam(QString("video"),QString("mirror-setting")).toInt();
    this->m_widthScale = this->loadParam(QString("video"),QString("resolucion-x")).toInt();
    this->m_scaleMode = this->loadParam(QString("video"),QString("scale-mode")).toInt();
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

//void QtkVideoServer::OnFilterCapturedImage(QImage frame)
//{
//    m_mutexA.lock();
//    QVideoFrame tFrame = frame;
//    if(tFrame.map(QAbstractVideoBuffer::ReadOnly))
//    {
//        if(this->m_widthScale == 0) this->m_widthScale = tFrame.width();
//        switch(this->m_mirrorSetting)
//        {
//            case mirrorVertical:
//                this->m_currentFrame = QImage(tFrame.bits(), tFrame.width(), tFrame.height(), tFrame.bytesPerLine(), getQImageFormat(tFrame.pixelFormat())).mirrored(0, 1).scaledToWidth(this->m_widthScale, (Qt::TransformationMode)this->m_scaleMode);
//                break;

//            case mirrorHorizontal:
//                this->m_currentFrame = QImage(tFrame.bits(), tFrame.width(), tFrame.height(), tFrame.bytesPerLine(), getQImageFormat(tFrame.pixelFormat())).mirrored(1, 0).scaledToWidth(this->m_widthScale, (Qt::TransformationMode)this->m_scaleMode);
//                break;

//            case mirrorAll:
//                this->m_currentFrame = QImage(tFrame.bits(), tFrame.width(), tFrame.height(), tFrame.bytesPerLine(), getQImageFormat(tFrame.pixelFormat())).mirrored(1, 1).scaledToWidth(this->m_widthScale, (Qt::TransformationMode)this->m_scaleMode);
//                break;

//            case mirrorNone:
//            default:
//            this->m_currentFrame = QImage(tFrame.bits(), tFrame.width(), tFrame.height(), tFrame.bytesPerLine(), getQImageFormat(tFrame.pixelFormat())).scaledToWidth(this->m_widthScale, (Qt::TransformationMode)this->m_scaleMode);
//            break;
//        }
//        tFrame.unmap();
//    }
//    m_mutexA.unlock();
//    emit frameUpdated();
//    //qDebug() << "QtkVideoServer::OnFilterCapturedImage";
//}

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
    this->m_currentFrame = frame.copy(QRect()).mirrored(mHor, mVer).scaledToWidth(this->m_widthScale, (Qt::TransformationMode)this->m_scaleMode);
    QDateTime time =  QDateTime::currentDateTime();
    this->osdTextWrite(&this->m_currentFrame,  this->loadParam(QString("aplicacion"),QString("streamming-alias")), 25, 25);
    this->osdTextWrite(&this->m_currentFrame,  time.toString("dd.MM.yyyy - hh:mm:ss.zzz"), 25, 50);
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
    this->m_currentFrame.save( &buffer, "JPG", this->loadParam(QString("video"),QString("calidad")).toInt());    
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
	this->m_currentFrame.save( &buffer, "JPG", this->loadParam(QString("video"),QString("calidad")).toInt());
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

void QtkVideoServer::osdTextWrite(QImage* img, QString osdText, int xPos, int yPos)
{	
    QPainter p(img);
	p.setPen(QPen(Qt::blue));
	p.setFont(QFont("Times", 14, QFont::Bold));	
	p.drawText(QPoint(xPos, yPos), osdText); 
}

QImage::Format QtkVideoServer::getQImageFormat(QVideoFrame::PixelFormat format)
{
    switch(format)
    {
        case QVideoFrame::Format_RGB24: return QImage::Format_RGB888;
        case QVideoFrame::Format_RGB32: return QImage::Format_RGB32;
        default: break;
    }

    return QImage::Format_RGB888;
}
