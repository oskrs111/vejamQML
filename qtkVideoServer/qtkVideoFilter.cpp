#include "QDebug"
#include "qtkVideoFilter.h"

QVideoFilterRunnable *qtkVideoFilter::createFilterRunnable() 
{ 
    //OSLL: Used as constructor call...
    this->m_rotationAngle = (qreal)180;
    this->m_scaleX = (qreal)0.5;
    this->m_scaleY = (qreal)0.5;
    this->m_frameDropper = 0;

    emit filterReady();
    return new qtkVideoFilterRunable(this);
}

void qtkVideoFilter::frameUpdated(QImage frame)
{    
    emit frameReady(frame);
}

void qtkVideoFilter::setRotationAngle(int angle)
{
    this->m_mutexA.lock();
    this->m_rotationAngle = (qreal)angle;
    this->m_mutexA.unlock();
}

void qtkVideoFilter::setScaleX(int sx)
{
    this->m_mutexA.lock();
    this->m_scaleX = (qreal)(((qreal)sx)/100);
    this->m_mutexA.unlock();
}

void qtkVideoFilter::setScaleY(int sy)
{
    this->m_mutexA.lock();
    this->m_scaleY = (qreal)(((qreal)sy)/100);
    this->m_mutexA.unlock();
}

void qtkVideoFilter::setFrameDropper(quint8 frames)
{
    this->m_mutexA.lock();
    this->m_frameDropper = frames;
    this->m_mutexA.unlock();
}

qreal qtkVideoFilter::getRotationAngle()
{
    this->m_mutexA.lock();
    qreal d = this->m_rotationAngle;
    this->m_mutexA.unlock();
    return d;
}

qreal qtkVideoFilter::getScaleX()
{
    this->m_mutexA.lock();
    qreal d = this->m_scaleX;
    this->m_mutexA.unlock();
    return d;
}

qreal qtkVideoFilter::getScaleY()
{
    this->m_mutexA.lock();
    qreal d = this->m_scaleY;
    this->m_mutexA.unlock();
    return d;
}

quint8 qtkVideoFilter::getFrameDropper()
{
    this->m_mutexA.lock();
    quint8 d = this->m_frameDropper;
    this->m_mutexA.unlock();
    return d;
}

qtkVideoFilterRunable::qtkVideoFilterRunable(qtkVideoFilter* parent)
{
    this->p_parent = parent;
}

QVideoFrame qtkVideoFilterRunable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags)
//http://www.bircd.org/annoyances/tft-aspect-ratio/modes.png
{    
    Q_UNUSED(surfaceFormat);
    Q_UNUSED(flags); 
    static quint8 frameDropper = 0;

    if(frameDropper == this->p_parent->getFrameDropper())
    {
        frameDropper = 0;
    }
    else
    {
        frameDropper++;
        return *input;
    }

    QVideoFrame tFrame = *input;
    if(tFrame.map(QAbstractVideoBuffer::ReadOnly))
    {
         QTransform transform;
         transform.rotate(this->p_parent->getRotationAngle())
                   .scale(this->p_parent->getScaleX(),this->p_parent->getScaleY());

//https://forum.qt.io/topic/46912/how-to-get-a-frame-image-from-qcamera/2
        if(tFrame.pixelFormat() ==  QVideoFrame::Format_BGR32){
        this->m_currentFrame = QImage(tFrame.bits(),
                                      tFrame.width(),
                                      tFrame.height(),
                                      tFrame.bytesPerLine(),
                                      this->qPixel2QImageFormat(tFrame.pixelFormat()))
                                      .transformed(transform)
                                      .rgbSwapped()
                                      .copy(QRect());

        }
        else{
        this->m_currentFrame = QImage(tFrame.bits(),
                                      tFrame.width(),
                                      tFrame.height(),
                                      tFrame.bytesPerLine(),                                      
                                      this->qPixel2QImageFormat(tFrame.pixelFormat()))
                                      .transformed(transform)
                                      .copy(QRect());
        }
        tFrame.unmap();
        this->p_parent->frameUpdated(this->m_currentFrame/*.copy(QRect())*/);
    }
    //this->m_mutexA.unlock();

    return *input;

}

QImage::Format qtkVideoFilterRunable::qPixel2QImageFormat(QVideoFrame::PixelFormat format)
{
    switch(format)
    {
        case QVideoFrame::Format_RGB24: return QImage::Format_RGB888;
        case QVideoFrame::Format_RGB32: return QImage::Format_RGB32;
        case QVideoFrame::Format_BGR32: return QImage::Format_RGB32;
        default:
        qDebug() << "qtkVideoFilterRunable::qPixel2QImageFormat-> Unknown format-> " << format;
        break;
    }

    return QImage::Format_RGB888;
}





