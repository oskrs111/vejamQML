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
    this->m_mutexA.lock();
    QVideoFrame tFrame = *input;
    if(tFrame.map(QAbstractVideoBuffer::ReadOnly))
    {

        this->m_currentFrame = QImage(tFrame.bits(),
                                      tFrame.width(),
                                      tFrame.height(),
                                      tFrame.bytesPerLine(),
                                      this->qPixel2QImageFormat(tFrame.pixelFormat()));
        tFrame.unmap();
    }
    this->m_mutexA.unlock();
    this->p_parent->frameUpdated(this->m_currentFrame);
    return *input;
}

QImage::Format qtkVideoFilterRunable::qPixel2QImageFormat(QVideoFrame::PixelFormat format)
{
    switch(format)
    {
        case QVideoFrame::Format_RGB24: return QImage::Format_RGB888;
        case QVideoFrame::Format_RGB32: return QImage::Format_RGB32;
        default: break;
    }

    return QImage::Format_RGB888;
}





