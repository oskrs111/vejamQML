#ifndef QTK_VIDEO_FILTER_H
#define QTK_VIDEO_FILTER_H
#include "QVideoFilterRunnable"
#include "QAbstractVideoFilter"

class qtkVideoFilterRunable : public QVideoFilterRunnable 
{

public:
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags)
	{		
		emit frameReady(*input);
		return *input;
	}
	
signals:
    void frameReady(QVideoFrame frame);
	
};

class qtkVideoFilter : public QAbstractVideoFilter 
{
	Q_OBJECT

public:
    QVideoFilterRunnable *createFilterRunnable() { return new qtkVideoFilterRunable; }

signals:
    void finished(QObject *result);
};

#endif
