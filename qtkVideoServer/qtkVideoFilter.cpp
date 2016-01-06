#include "QDebug"
#include "qtkVideoFilter.h"

QVideoFilterRunnable *qtkVideoFilter::createFilterRunnable() 
{ 
    return new qtkVideoFilterRunable(this);
}

void qtkVideoFilter::frameUpdated(QImage frame)
{
    //static quint16 fn = 0;
    //qDebug() << "qtkVideoFilter::frameUpdated= " << fn++;   
    emit frameReady(frame);
}

qtkVideoFilterRunable::qtkVideoFilterRunable(qtkVideoFilter* parent)
{
    this->p_parent = parent;
}

QVideoFrame qtkVideoFilterRunable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags)
{    
    Q_UNUSED(surfaceFormat);
    Q_UNUSED(flags); 
    static quint8 frameDropper = 0;

    if(frameDropper == 6)
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
//https://forum.qt.io/topic/46912/how-to-get-a-frame-image-from-qcamera/2
        if(tFrame.pixelFormat() ==  QVideoFrame::Format_BGR32){
        this->m_currentFrame = QImage(tFrame.bits(),
                                      tFrame.width(),
                                      tFrame.height(),
                                      tFrame.bytesPerLine(),
                                      this->qPixel2QImageFormat(tFrame.pixelFormat())).rgbSwapped().copy(QRect());

        }
        else{
        this->m_currentFrame = QImage(tFrame.bits(),
                                      tFrame.width(),
                                      tFrame.height(),
                                      tFrame.bytesPerLine(),                                      
                                      this->qPixel2QImageFormat(tFrame.pixelFormat())).copy(QRect());
        }
        tFrame.unmap();
        this->p_parent->frameUpdated(this->m_currentFrame.copy(QRect()));
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





