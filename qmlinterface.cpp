#include "qmlinterface.h"
#include "main.h"

qmlInterface::qmlInterface(QObject *parent) : QObject(parent)
{
    this->p_engine = 0;
    this->p_videoSource = 0;
    this->p_imageProvider = 0;
    this->p_timer = new QTimer(this);
    connect(this->p_timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    this->p_timer->start(50);
}

void qmlInterface::setEngine(QQmlApplicationEngine* engine)
{
    this->p_engine = engine;
    QQmlContext* ctx = this->p_engine->rootContext();
    ctx->setContextProperty("qmlInterface", this);
}

void qmlInterface::setVideoSource(QtkVideoServer* videoSource)
{
    this->p_videoSource = videoSource;
}

void qmlInterface::setImageProvider(qtkImageProvider* imageProvider)
{
    this->p_imageProvider = imageProvider;
}

void qmlInterface::updateTextState()
{
    static char sym[] = {'|','/','-','\\'};
    static quint8 index = 0;

    if(this->p_engine)
    {
        QList<QObject*> root = this->p_engine->rootObjects();
        QObject *text = root[0]->findChild<QObject*>(QString("text.stat"));
        QQmlProperty::write(text, QString("text"), QVariant(QString(sym[index++])), this->p_engine);
        if(index >= sizeof(sym)) index = 0;
    }
}

quint8 qmlInterface::alert(QString message)
{
    qDebug() << message;
    return 0;
}

QObject* qmlInterface::getQmlCamera()
//http://stackoverflow.com/questions/28041741/qt-qml-camera-to-c-qimage-on-android
{
    if(this->p_engine)
    {
        QList<QObject*> root = this->p_engine->rootObjects();
        QObject *camera = root[0]->findChild<QObject*>(QString("camera.device"));
        return camera;
    }

    return 0;
}

QObject* qmlInterface::getQmlVideoFilter()
//http://stackoverflow.com/questions/28041741/qt-qml-camera-to-c-qimage-on-android
{
    if(this->p_engine)
    {
        QList<QObject*> root = this->p_engine->rootObjects();
        QObject *camera = root[0]->findChild<QObject*>(QString("videoFilter.device"));
        return camera;
    }

    return 0;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void qmlInterface::_TRACE(QString log)
{
    qDebug() << log;
}

void qmlInterface::onLoginButton()
{
    qDebug() << "OnLoginButton()";
    if(this->p_engine)
    {
        QList<QObject*> root = this->p_engine->rootObjects();
        QObject *user = root[0]->findChild<QObject*>(QString("login.user"));
        QObject *pass = root[0]->findChild<QObject*>(QString("login.pass"));

        if((user != 0) && (pass != 0))
        {
            QVariant username = QQmlProperty::read(user, QString("text"), this->p_engine);
            QVariant password = QQmlProperty::read(pass, QString("text"), this->p_engine);

            if(username.toString().size() == 0)
            {
                this->alert(QString("Introduzca el nombre de usuario para continuar..."));
                return;
            }

            if(password.toString().size() == 0)
            {
                this->alert(QString("Introduzca la contraseña para continuar..."));
                return;
            }

            this->alert(QString("...go login..."));
            QObject *loader = root[0]->findChild<QObject*>(QString("qml.Loader"));
            QQmlProperty::write(loader, QString("source"), QVariant(QString("qrc:/liveScreen.qml")), this->p_engine);

            _globalStartServices();
        }
    }
}

void qmlInterface::onRunButton()
{
    qDebug() << "OnRunButton()";
    if(this->p_engine)
    {
        QList<QObject*> root = this->p_engine->rootObjects();
        QObject *loader = root[0]->findChild<QObject*>(QString("qml.Loader"));
        QQmlProperty::write(loader, QString("source"), QVariant(QString("qrc:/liveScreen.qml")), this->p_engine);
        _globalStartServices();
    }
}

void qmlInterface::onFrameUpdated()
{
//    static quint8 div = 0;
//    m_mutexA.lock();
//    if((this->p_imageProvider != 0) && (this->p_videoSource != 0))
//    {
//        this->p_imageProvider->updateImage(this->p_videoSource->currentFrame2Image());
//        QList<QObject*> root = this->p_engine->rootObjects();
//        QObject *image = root[0]->findChild<QObject*>(QString("live.image"));
//        QQmlProperty::write(image, QString("source"), QVariant(QString("image://imageProvider/frame.%1").arg(frameId++)), this->p_engine);
//    }
//    m_mutexA.unlock();
//    qDebug() << "onFrameUpdated()";
//    if(div == 1)
//    {
//        this->updateTextState();
//        div = 0;
//    }
//    else div++;
      this->updateTextState();

}

void qmlInterface::onTimer()
{

}

void qmlInterface::writeLog(QString log)
{
    if(this->p_engine)
    {
        QVariant returnedValue;
        QList<QObject*> root = this->p_engine->rootObjects();
        if(root.size() > 0)
        {
            QObject *text = root[0]->findChild<QObject*>(QString("text.log"));
            QMetaObject::invokeMethod(text, "append",
                     Q_RETURN_ARG(QVariant, returnedValue),
                     Q_ARG(QVariant, log));
        }
    }

}

void qmlInterface::onRemoteRequest(int type)
{
    #ifdef WINDOWS_PLATFORM
    Q_UNUSED(type);
    if(this->p_engine)
    {
        QVariant returnedValue;
        QList<QObject*> root = this->p_engine->rootObjects();
        QObject *mainWindow = root[0];
        QVariant flags = QQmlProperty::read(mainWindow, QString("flags"), this->p_engine);
        int iflags = flags.toInt();
        iflags |= Qt::WindowStaysOnTopHint;
        QQmlProperty::write(mainWindow, QString("flags"), QVariant(iflags), this->p_engine);
        QQmlProperty::write(mainWindow, QString("flags"), flags, this->p_engine);
        //QMetaObject::invokeMethod(mainWindow, "raise");
        qDebug() << "onRemoteRequest()" << mainWindow;

    }
    #endif
}



