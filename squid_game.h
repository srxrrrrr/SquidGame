#ifndef SQUIDGAME_H
#define SQUIDGAME_H

#include <QWidget>

#include <QMouseEvent>
#include "controller/IGameController.h"
#include "model/IGameService.h"
#include <vlc/vlc.h>

QT_BEGIN_NAMESPACE
namespace Ui { class SquidGame; }
QT_END_NAMESPACE

#define TOTAL_STEPS 18
#define LEFT_RECTANGLE_START_X 153
#define LEFT_RECTANGLE_START_Y 88
#define LEFT_RECTANGLE_END_X 662
#define LEFT_RECTANGLE_END_Y 488
#define RIGHT_RECTANGLE_START_X 788
#define RIGHT_RECTANGLE_START_Y 88
#define RIGHT_RECTANGLE_END_X 1294
#define RIGHT_RECTANGLE_END_Y 488

typedef struct RectanglePosition
{
    QPoint start_point;
    QPoint end_point;
}RectanglePosition;

class SquidGame : public QWidget
{
    Q_OBJECT

public:
    SquidGame(QWidget *parent = nullptr);
    ~SquidGame();
    static SquidGame *pThis;
    Ui::SquidGame *ui;

private:
    virtual void paintEvent(QPaintEvent *event);
    virtual void closeEvent(QCloseEvent *event);

protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);

public:
    void PlayAudio(QString path, bool stop_first);

private:
    IGameHandle* handle = NULL;
    IPresenter* presenter = NULL;
    IGameService* game_service = NULL;
    int x_pos = 0;
    int y_pos = 0;
    bool inside_left_grid = false;
    bool inside_right_grid = false;
    RectanglePosition rectangle_position[2];

public:
    //Q: libvlc使用絕對路徑會找不到檔案...
//    const QString opening_music = "D:/Git/github/SquidGame/build/debug/music/dead.m4a";

    //case1: IDE develop
    const QString opening_music = "debug/music/Squid Game - Way Back Then.mp3";
    const QString dead_music = "debug/music/dead.m4a";
    const QString win_music = "debug/music/win.m4a";

    //case2: run exe
//    const QString opening_music = "/music/Squid Game - Way Back Then.mp3";
//    const QString dead_music = "/music/dead.m4a";
//    const QString win_music = "/music/win.m4a";

    libvlc_instance_t * vlc_instance;
    libvlc_media_player_t *vlc_media_player;
    libvlc_media_t *vlc_media;

signals:
    void UpdateScreen();
};

class Presenter : public IPresenter
{
    virtual void ShowImage(EStage stage);
    virtual void ShowProgress(unsigned char progress);
};

#endif // SQUIDGAME_H
