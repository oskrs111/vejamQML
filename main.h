#ifndef MAIN_H
#define MAIN_H
#include "qmlinterface.h"
#include "qtkImageProvider.h"
#include "qtkvideoserver.h"
#include "qtkVideoFilter.h"
#include "qtkHttpServer.h"
#include "qtkapplicationparameters.h"

QtKApplicationParameters* __getApplicationParams();
void setDefaultParameters();
void loadParams();
void _globalStartServices();
void _globalUpdateApplicationSettings();
void debugLogger(QtMsgType type, const QMessageLogContext &context, const QString &msg);
#ifdef ANDROID_PLATFORM_EXTRAS
void keepScreenOn()
#endif
#endif // MAIN_H

