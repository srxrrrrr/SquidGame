#ifndef STOPTHREAD_H
#define STOPTHREAD_H

#include <QObject>
#include <QThread>
#include <vlc/vlc.h>

class StopThread : public QThread
{
    Q_OBJECT
public:
    explicit StopThread(QObject *parent = nullptr);
    virtual void run();
    libvlc_media_player_t *vlc_media_player;
signals:

};

#endif // STOPTHREAD_H
