#ifndef SQUIDGAME_H
#define SQUIDGAME_H

#include <QWidget>

#include <QMouseEvent>
#include "controller/IGameController.h"
#include "model/IGameService.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SquidGame; }
QT_END_NAMESPACE

#define TOTAL_STEPS 5
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

private:
    IGameHandle* handle = NULL;
    IPresenter* presenter = NULL;
    IGameService* game_service = NULL;
//    unsigned char total_steps;
    int x_pos = 0;
    int y_pos = 0;
    bool inside_left_grid = false;
    bool inside_right_grid = false;
    RectanglePosition rectangle_position[2];

signals:
    void UpdateScreen();
};

class Presenter : public IPresenter
{
    virtual void ShowImage(EStage stage);
    virtual void ShowProgress(unsigned char progress);
};

#endif // SQUIDGAME_H
