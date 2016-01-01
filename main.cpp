#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include "qmlinterface.h"
#include "main.h"
//#include "qtkHttpServer.h"
#include "qtkImageProvider.h"
#include "qtkvideoserver.h"
#include "qtkVideoFilter.h"
#include "qtkHttpServer.h"
#include "qtkapplicationparameters.h"

QtKApplicationParameters* gParams;
QtkVideoServer* gVideoServer;
QtkHttpServer* gHttpServer;
qmlInterface* gInterface;
qtkImageProvider* gImageProvider;


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

        gInterface->setImageProvider(gImageProvider);       
        gVideoServer = new QtkVideoServer(gParams, 0);

        gHttpServer = new QtkHttpServer(gParams->loadParam(QString("conexion"),QString("mjpeg-port"),0).toInt(0,10), 0);
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
    gParams->saveParam(QString("aplicacion"),QString("webkit-debug"),QString("0"));
    gParams->saveParam(QString("aplicacion"),QString("streamming-mode"),QString("2")); //1: WebKit, 2: MJPEG
    gParams->saveParam(QString("aplicacion"),QString("streamming-id"),QString("0")); //1...8
    gParams->saveParam(QString("aplicacion"),QString("streamming-alias"),QString("My Webcam!")); //1...8
    gParams->saveParam(QString("aplicacion"),QString("server-url"),QString("www.vejam.info/app-gui")); //http://www.vejam.info/app-gui/app-gui-welcome.html
    gParams->saveParam(QString("aplicacion"),QString("cleanup-enable"),QString("1"));
    gParams->saveParam(QString("aplicacion"),QString("file-log"),QString("0"));
    gParams->saveParam(QString("conexion"),QString("webkit-port"),QString("40001"));
    gParams->saveParam(QString("conexion"),QString("mjpeg-port"),QString("50001"));
    gParams->saveParam(QString("conexion"),QString("mjpeg-uri"),QString("/stream.html"));
    gParams->saveParam(QString("video"),QString("resolucion-x"),QString("0"));
    gParams->saveParam(QString("video"),QString("scale-mode"),QString("0"));
    gParams->saveParam(QString("video"),QString("calidad"),QString("-1"));
    gParams->saveParam(QString("video"),QString("framerate-max"),QString("24"));
    gParams->saveParam(QString("video"),QString("mirror-setting"),QString("0"));

    gParams->saveParam(QString("video"),QString("osd-enable"),QString("1"));
    gParams->saveParam(QString("video"),QString("osd-x-position"),QString("10"));
    gParams->saveParam(QString("video"),QString("osd-y-position"),QString("10"));
    gParams->saveParam(QString("video"),QString("osd-text-size"),QString("10"));
    gParams->saveParam(QString("video"),QString("osd-text-font"),QString("0"));
    gParams->saveParam(QString("video"),QString("osd-show-title"),QString("1"));
    gParams->saveParam(QString("video"),QString("osd-show-time"),QString("1"));

    gParams->saveParam(QString("device"),QString("selected"),QString("1"));	//Indica la camara per defecte.
    gParams->fileSave();
}
