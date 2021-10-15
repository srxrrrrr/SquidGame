#include "stop_thread.h"
#include <QtCore>

StopThread::StopThread(QObject *parent) : QThread(parent)
{

}

void StopThread::run()
{
    qDebug() << "thread stop";
    libvlc_media_player_stop(vlc_media_player);
}
