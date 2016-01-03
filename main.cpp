#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include "main.h"
//03.01.2015 - Motorola Moto G (5.1.1) Runtime OK, HTTP OK, MJPG OK, LAYOUT NOK.
//03.01.2015 - Samsung Note 2 (4.4.2) Runtime OK, HTTP OK, MJPG OK, LAYOUT NOK.

static QtKApplicationParameters* gParams = 0;
static QtkVideoServer* gVideoServer = 0;
static QtkHttpServer* gHttpServer = 0;
static qmlInterface* gInterface = 0;
static qtkImageProvider* gImageProvider = 0;

QtKApplicationParameters* __getApplicationParams()
{
    return gParams;
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    qmlRegisterType<qtkVideoFilter>("qtkvideofilter.uri", 1, 0, "VideoFilter");
    gInterface = new qmlInterface();
    gImageProvider = new qtkImageProvider(QQmlImageProviderBase::Image);
    gInterface->setEngine(&engine);

    engine.addImageProvider(QString("imageProvider"),gImageProvider);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    loadParams();

    qInstallMessageHandler(debugLogger);

    gInterface->setImageProvider(gImageProvider);
    gVideoServer = new QtkVideoServer(gParams, 0);

    gHttpServer = new QtkHttpServer(gParams->loadParam(QString("conexion"),QString("mjpeg-port"),0).toInt(0,10), 0);
    gHttpServer->setFilesRootPath(gParams->loadParam(QString("aplicacion"),QString("root-path"),0));
    gHttpServer->setVideoServer(gVideoServer);
    gHttpServer->setMaxFramerate(gParams->loadParam(QString("video"),QString("framerate-max"),0).toInt(0,10));

    QCamera* cam = qvariant_cast<QCamera*>(gInterface->getQmlCamera()->property("mediaObject"));
    gVideoServer->setCamera(cam);

    QObject* filter = gInterface->getQmlVideoFilter();
    gVideoServer->setVideoFilter((qtkVideoFilter*)filter);


    gInterface->setVideoSource(gVideoServer);
    gInterface->setImageProvider(gImageProvider);
    QObject::connect(gVideoServer, SIGNAL(frameUpdated()), gInterface, SLOT(onFrameUpdated()));

    return app.exec();
}

#ifdef ANDROID_PLATFORM_EXTRAS
#include <QtAndroidExtras>
void keepScreenOn()
{
    QAndroidJniObject activity = QtAndroid::androidActivity();
    if (activity.isValid()) {
        QAndroidJniObject window = activity.callObjectMethod("getWindow", "()Landroid/view/Window;");

        if (window.isValid()) {
            const int FLAG_KEEP_SCREEN_ON = 128;
            window.callObjectMethod("addFlags", "(I)V", FLAG_KEEP_SCREEN_ON);
        }
    }
}
#endif

void loadParams()
{
    gParams = new QtKApplicationParameters(0,QString("vejam"));
    if(gParams->fileLoad(false))
    {
        setDefaultParameters();
        qDebug() << "No se ha encontrado vejam.cfg!\r\nEstableciendo configuración por defecto.\r\n\r\n";
    }
}

quint8 servicesStart()
{
    return 1;
}

void setDefaultParameters()
{
    gParams->saveParam(QString("aplicacion"),QString("username"),QString("user@name.here"));
    gParams->saveParam(QString("aplicacion"),QString("password"),QString("password"));
    gParams->saveParam(QString("aplicacion"),QString("sync-interval"),QString("60"));        
    gParams->saveParam(QString("aplicacion"),QString("streamming-id"),QString("0")); //1...8
    gParams->saveParam(QString("aplicacion"),QString("streamming-alias"),QString("My Webcam!")); //1...8
    gParams->saveParam(QString("aplicacion"),QString("server-url"),QString("www.vejam.info/app-gui")); //http://www.vejam.info/app-gui/app-gui-welcome.html
    gParams->saveParam(QString("aplicacion"),QString("file-log"),QString("0"));
    gParams->saveParam(QString("aplicacion"),QString("root-path"),QString(":/3w/"));
    gParams->saveParam(QString("conexion"),QString("mjpeg-port"),QString("50001"));
    gParams->saveParam(QString("conexion"),QString("mjpeg-uri"),QString("/stream.html"));
    gParams->saveParam(QString("video"),QString("max-width"),QString("0"));
    gParams->saveParam(QString("video"),QString("scale-mode"),QString("0"));
    gParams->saveParam(QString("video"),QString("quality"),QString("-1"));
    gParams->saveParam(QString("video"),QString("framerate-max"),QString("24"));
    gParams->saveParam(QString("video"),QString("mirror-setting"),QString("0"));
    gParams->saveParam(QString("video"),QString("frame-drop"),QString("6"));
    gParams->saveParam(QString("video"),QString("osd-show-title"),QString("1"));
    gParams->saveParam(QString("video"),QString("osd-show-time"),QString("1"));
    gParams->saveParam(QString("video"),QString("osd-x-position"),QString("10"));
    gParams->saveParam(QString("video"),QString("osd-y-position"),QString("10"));
    gParams->saveParam(QString("video"),QString("osd-text-font"),QString("Times"));
    gParams->saveParam(QString("video"),QString("osd-text-font-size"),QString("14"));
    gParams->saveParam(QString("video"),QString("osd-text-font-weight"),QString("75"));
    gParams->saveParam(QString("video"),QString("osd-text-font-color"),QString("0000FF"));
    gParams->saveParam(QString("video"),QString("device"),QString("0"));	//OSLL: Default video (camera) source.
#ifdef ANDROID_PLATFORM
    gParams->saveParam(QString("video-extras"),QString("torch"),QString("0"));	//OSLL: Default torch (flash) state.
#endif
    gParams->fileSave();
}

//http://doc.qt.io/qt-5/qtglobal.html#qInstallMessageHandler
void debugLogger(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        //localMsg.prepend("DEBUG: ");
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        return; //OSLL: Some QML layouts warning are disturbing...
        localMsg.prepend("WARNING: ");
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        localMsg.prepend("CRITICAL: ");
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        localMsg.prepend("FATAL: ");
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        abort();
    }

    //localMsg.append("\r\n");
    if(gInterface)
    {
        gInterface->writeLog(localMsg);
    }

//    QFile f("vejam.debug.log");
//    f.open(QIODevice::Append);
//    f.write(localMsg);
//    f.close();
}


