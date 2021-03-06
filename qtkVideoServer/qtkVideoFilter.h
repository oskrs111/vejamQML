#ifndef QTK_VIDEO_FILTER_H
#define QTK_VIDEO_FILTER_H
#include "QMutex"
#include "QVideoFrame"
#include "QVideoFilterRunnable"
#include "QAbstractVideoFilter"

class qtkVideoFilter : public QAbstractVideoFilter 
{
	Q_OBJECT
public:    
    QVideoFilterRunnable *createFilterRunnable();

    void frameUpdated(QImage frame);
    void setRotationAngle(int angle);
    void setScaleX(int sx);
    void setScaleY(int sy);    
    void setFrameDropper(quint8 frames);

    qreal getRotationAngle();
    qreal getScaleX();    
    qreal getScaleY();
    quint8 getFrameDropper();

private:
    qreal m_rotationAngle;
    qreal m_scaleX;
    qreal m_scaleY;
    quint8 m_frameDropper;
    QMutex m_mutexA;

signals:
    void finished(QObject *result);
    void frameReady(QImage frame);
    void filterReady();
};


class qtkVideoFilterRunable : public QVideoFilterRunnable
{
    public:
    qtkVideoFilterRunable(qtkVideoFilter* parent);
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags);
    QImage::Format qPixel2QImageFormat(QVideoFrame::PixelFormat format);

    private:
    QImage m_currentFrame;
    QMutex m_mutexA;
    qtkVideoFilter* p_parent;
};

#endif
