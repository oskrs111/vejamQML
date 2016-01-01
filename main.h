#ifndef MAIN_H
#define MAIN_H
void setDefaultParameters();
void loadParams();
quint8 servicesStart();

#ifdef ANDROID_PLATFORM
void keepScreenOn()
#endif
#endif // MAIN_H

