#include <QBuffer>
#include <QPainter>
#include <QDateTime>
#include <QImageEncoderControl>
#include "qtkvideoserver.h"

//http://blog.csdn.net/kl222/article/details/23187693

QtkVideoServer::QtkVideoServer(QtKApplicationParameters *params, QObject *parent) :
    QObject(parent)
{
        this->m_appParameters = params;
		this->m_camera = 0;
}

bool QtkVideoServer::loadAvaliableCameras()
{
  qint16 order = 1;
  struct vjCameraDevice camDevice;

  foreach(const QByteArray &deviceName, QCamera::availableDevices())
  {
      camDevice.m_name = deviceName;
      camDevice.m_description =  QCamera::deviceDescription(deviceName);

      qDebug() << "Añade camara: " << deviceName;
      this->m_devices.append(camDevice);
      this->saveParam(QString("device"),QString("name"),QString(camDevice.m_name),order);
      this->saveParam(QString("device"),QString("description"),QString(camDevice.m_description),order);
	  order++;
  }

  if(this->m_devices.size())
  {
      this->saveParam(QString("device"),QString("qtty"),QString("%1").arg(this->m_devices.size()));      
      return 0;
  }
  return 1;
}

void QtkVideoServer::startServer()
{	
	this->setCamera(this->m_devices.at(0).m_name);
	this->m_camera->start();
	this->m_viewfinder->capture();
}

void QtkVideoServer::Capture()
{
	this->m_viewfinder->capture();
}

void QtkVideoServer::setCameraDevice(QCamera* cameraDevice)
{
    this->m_camera = cameraDevice;
}

void QtkVideoServer::setCamera(const QByteArray &cameraDeviceName)
{    
    if(this->m_camera == 0)
    {
        if (cameraDeviceName.isEmpty())
        this->m_camera = new QCamera;
        else
        this->m_camera = new QCamera(cameraDeviceName);
    }
    else
    {
       qDebug() << " QtkVideoServer::setCamera->Using external device pointer...";
    }
    connect(this->m_camera, SIGNAL(stateChanged(QCamera::State)), this, SLOT(OnUpdateCameraState(QCamera::State)));
    connect(this->m_camera, SIGNAL(error(QCamera::Error)), this, SLOT(OnDisplayCameraError(QCamera::Error)));
  	
	
    this->m_viewfinder = new QtKCaptureBuffer(this);
	this->m_viewfinder->setMirrorSetting(this->loadParam(QString("video"),QString("mirror-setting")).toInt());
	this->m_viewfinder->setWidthScale(this->loadParam(QString("video"),QString("resolucion-x")).toInt());
	this->m_viewfinder->setScaleMode(this->loadParam(QString("video"),QString("scale-mode")).toInt());
    connect(this->m_viewfinder, SIGNAL(imageCaptured(int,QImage)), this, SLOT(OnProcessCapturedImage(int,QImage)));

    this->m_camera->setViewfinder(this->m_viewfinder);
    this->m_camera->setCaptureMode(QCamera::CaptureStillImage);

    //OSLL: Traveling to amazing world of multiplatform support...
    this->m_videoProbe = new QVideoProbe(this);
    if (this->m_videoProbe->setSource(this->m_camera))
    {
        qDebug() << "QtkVideoServer::setCamera->videoProbe= sucess!";
        connect(this->m_videoProbe, SIGNAL(videoFrameProbed(QVideoFrame)),this, SLOT(OnProbeCapturedImage(QVideoFrame)));
    }
}

int QtkVideoServer::getServerState()
{
    if(this->m_camera)
    {
        return this->m_camera->state();
    }
    else return 0;
}

void QtkVideoServer::OnProcessCapturedImage(int id, QImage image)
{
    Q_UNUSED(id);
    m_mutexA.lock();
	this->m_currentFrame = image;
	m_mutexA.unlock();
    emit frameUpdated();
}

void QtkVideoServer::OnProbeCapturedImage(QVideoFrame frame)
{
    m_mutexA.lock();
    QVideoFrame tFrame = frame;
    if(tFrame.map(QAbstractVideoBuffer::ReadOnly))
    {
        this->m_currentFrame = QImage(frame.bits(),
                                      frame.width(),
                                      frame.height(),
                                      frame.bytesPerLine(),
                                      this->m_viewfinder->getQImageFormat(tFrame.pixelFormat())).mirrored(0, 1).scaledToWidth(0, (Qt::TransformationMode)0);
        /*
        if(this->m_widthScale == 0) this->m_widthScale = frame.width();
        switch(this->m_mirrorSetting)
        {
            case mirrorVertical:
                this->m_currentFrame = QImage(frame.bits(), frame.width(), frame.height(), frame.bytesPerLine(), getQImageFormat(tFrame.pixelFormat())).mirrored(0, 1).scaledToWidth(this->m_widthScale, (Qt::TransformationMode)this->m_scaleMode);
                break;

            case mirrorHorizontal:
                this->m_currentFrame = QImage(frame.bits(), frame.width(), frame.height(), frame.bytesPerLine(), getQImageFormat(tFrame.pixelFormat())).mirrored(1, 0).scaledToWidth(this->m_widthScale, (Qt::TransformationMode)this->m_scaleMode);
                break;

            case mirrorAll:
                this->m_currentFrame = QImage(frame.bits(), frame.width(), frame.height(), frame.bytesPerLine(), getQImageFormat(tFrame.pixelFormat())).mirrored(1, 1).scaledToWidth(this->m_widthScale, (Qt::TransformationMode)this->m_scaleMode);
                break;

            case mirrorNone:
            default:
            this->m_currentFrame = QImage(frame.bits(), frame.width(), frame.height(), frame.bytesPerLine(), getQImageFormat(tFrame.pixelFormat())).scaledToWidth(this->m_widthScale, (Qt::TransformationMode)this->m_scaleMode);
            break;
        }
*/
        tFrame.unmap();
    }
    m_mutexA.unlock();
    emit frameUpdated();
    qDebug() << "QtkVideoServer::OnProbeCapturedImage";
}

QImage QtkVideoServer::currentFrame2Image()
{
    QImage lastFrame;
    m_mutexA.lock();
    lastFrame = this->m_currentFrame;
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
	QDateTime time =  QDateTime::currentDateTime();
    buffer.open(QBuffer::WriteOnly);    
    m_mutexA.lock();
	this->osdTextWrite(&this->m_currentFrame,  this->loadParam(QString("aplicacion"),QString("streamming-alias")), 25, 25);
	this->osdTextWrite(&this->m_currentFrame,  time.toString("dd.MM.yyyy - hh:mm:ss.zzz"), 25, 50);
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
