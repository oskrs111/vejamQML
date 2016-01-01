//Original code by:
//http://www.lothlorien.com/kf6gpe/?p=234

#include "qtkImageProvider.h"
#include <QImage>
#include <QPixmap>
#include <QDebug>

qtkImageProvider::qtkImageProvider(QQuickImageProvider::ImageType type) :
     QQuickImageProvider(type)
{
    //type -> QQuickImageProvider::Pixmap
    this->m_currentImage = QImage(":/res/vejam_toolbar_h48.png");
}

qtkImageProvider::~qtkImageProvider()
{
    // This space intentionally left blank.
}

void qtkImageProvider::updateImage(QImage newImage)
{
    this->m_mutexA.lock();
    this->m_currentImage = newImage;
    this->m_mutexA.unlock();
}

QImage qtkImageProvider::requestImage(const QString& id, QSize* size, const QSize& requestedSize)
{   
    static QImage result;
    this->m_mutexA.lock();
    if (requestedSize.isValid())
    {
        result = this->m_currentImage.scaled(requestedSize, Qt::KeepAspectRatio);
    }
    else
    {
        result = this->m_currentImage;
    }
    *size = result.size();
    this->m_mutexA.unlock();
    qDebug() << "requestImage: " << id;
    return result;
}

QPixmap qtkImageProvider::requestPixmap(const QString& id, QSize* size, const QSize& requestedSize)
{
    QImage result;
    qDebug() << "requestPixmap: " << id;

    if (requestedSize.isValid()) {
        result = this->m_currentImage.scaled(requestedSize, Qt::KeepAspectRatio);
    } else {
        result = this->m_currentImage;
    }
    *size = result.size();
    return QPixmap::fromImage(result);
}
