//Original code by:
//http://www.lothlorien.com/kf6gpe/?p=234
#ifndef QTKIMAGEPROVIDER_H
#define QTKIMAGEPROVIDER_H
#include <QMutex>
#include <QQuickImageProvider>

class qtkImageProvider : public QQuickImageProvider
{

private:	
	QImage m_currentImage;
    QMutex m_mutexA;
	
public:
    qtkImageProvider(QQuickImageProvider::ImageType type);
    ~qtkImageProvider();
    QImage requestImage(const QString& id, QSize* size, const QSize& requestedSize);
    QPixmap requestPixmap(const QString& id, QSize* size, const QSize& requestedSize);
	void updateImage(QImage newImage);
};

#endif // QTKIMAGEPROVIDER_H

