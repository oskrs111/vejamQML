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

signals:
    void finished(QObject *result);
    void frameReady(QImage frame);
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
