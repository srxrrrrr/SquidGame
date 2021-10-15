#include "squid_game.h"
#include "ui_squidgame.h"
#include <QPainter>
#include <QPixmap>
#include <QImage>
#include <QThread>

#include "controller/bridge_game_handle.h"
#include "model/bridge_game_service.h"

EStage g_stage = EStage::Title;

SquidGame* SquidGame::pThis = nullptr;

SquidGame::SquidGame(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SquidGame)
{
    ui->setupUi(this);
    pThis = this;

    //讓所有的滑鼠移動能被mouseMoveEvent()抓到位置
    setMouseTracking(true);

    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::green);
    ui->label_Grid->setPalette(pe);
    ui->label_StepTotal->setPalette(pe);
    pe.setColor(QPalette::WindowText,Qt::red);
    ui->label_GridNum->setPalette(pe);
    ui->label_Next->setPalette(pe);
    pe.setColor(QPalette::WindowText,Qt::blue);
    ui->label_Soi->setPalette(pe);
    pe.setColor(QPalette::WindowText,Qt::white);
    ui->label_Title->setPalette(pe);
    ui->label_Mark->setStyleSheet("QLabel { color: rgb(255,20,147);}"); //dark pink

    QFont f("Kristen ITC", 100);
    ui->label_Soi->setFont(f);
    ui->label_Soi->setText("soi");
    ui->label_Title->setFont(f);
    ui->label_Title->setText("squid game");

    connect(this, SIGNAL(UpdateScreen()), this, SLOT(update()));

    QString str;
    str= "/" + QString::number(TOTAL_STEPS);
    ui->label_StepTotal->setText(str);

    ui->label_Grid->setVisible(false);
    ui->label_GridNum->setVisible(false);
    ui->label_StepTotal->setVisible(false);
    g_stage = EStage::Title;

    rectangle_position[0].start_point=QPoint(LEFT_RECTANGLE_START_X,LEFT_RECTANGLE_START_Y);
    rectangle_position[0].end_point=QPoint(LEFT_RECTANGLE_END_X,LEFT_RECTANGLE_END_Y);
    rectangle_position[1].start_point=QPoint(RIGHT_RECTANGLE_START_X,RIGHT_RECTANGLE_START_Y);
    rectangle_position[1].end_point=QPoint(RIGHT_RECTANGLE_END_X,RIGHT_RECTANGLE_END_Y);

    game_service = new BridgeGameService();
    presenter = new Presenter();
    handle = new BridgeGameHandle(game_service, presenter);

    handle->Init(TOTAL_STEPS);

    vlc_instance = libvlc_new(0, NULL);
    SquidGame::pThis->PlayAudio(opening_music, false);

    timer = new QTimer();
    connect(timer ,SIGNAL(timeout()) ,this, SLOT(TimerSlot()));

    stop_thread = new StopThread(this);
}

void SquidGame::TimerSlot()
{
    qDebug() << "timeout";

    if(timer->isActive())
    {
        timer->stop();
    }

    //在主執行緒裡libvlc_media_player_stop()，偶爾會造成vlc crash，
    //但在子thread裡libvlc_media_player_stop()則不會有此問題...
    stop_thread->start();
}

SquidGame::~SquidGame()
{
    delete ui;
}

void SquidGame::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if(g_stage==EStage::Title)
    {
        painter.drawPixmap(0,0,width(),height(),
                           qApp->applicationDirPath() + "/img/start.jpg");
        return;
    }
    else if(g_stage==EStage::Dead)
    {
        painter.drawPixmap(0,0,width(),height(),
                           qApp->applicationDirPath() + "/img/dead.jpg");
        QFont f("Kristen ITC", 100);
        ui->label_Next->setFont(f);
        ui->label_Next->setText("Next one...");
        ui->label_Next->setVisible(true);
        return;
    }
    else if(g_stage==EStage::Win)
    {
        return;
    }

    painter.drawPixmap(0,0,width(),height(),
                       qApp->applicationDirPath() + "/img/select.jpg");

    if(inside_left_grid)
    {
        painter.setPen(QPen(Qt::red, 10));
        painter.drawRect(rectangle_position[0].start_point.x(),rectangle_position[0].start_point.y(),
                rectangle_position[0].end_point.x() - rectangle_position[0].start_point.x(),
                rectangle_position[0].end_point.y() - rectangle_position[0].start_point.y());
    }
    else if(inside_right_grid)
    {
        painter.setPen(QPen(Qt::red, 10));
        painter.drawRect(rectangle_position[1].start_point.x(),rectangle_position[1].start_point.y(),
                rectangle_position[1].end_point.x() - rectangle_position[1].start_point.x(),
                rectangle_position[1].end_point.y() - rectangle_position[1].start_point.y());
    }
}

void SquidGame::closeEvent(QCloseEvent *event)
{
    delete presenter;
    delete handle;
    delete game_service;
    delete stop_thread;
    event->accept();
}

void SquidGame::mouseMoveEvent(QMouseEvent *event)
{
    QPoint point = event->pos();
    x_pos = point.x();
    y_pos = point.y();
//    qDebug() << "x:" << x_pos << " y:" << y_pos;

    if((x_pos > rectangle_position[0].start_point.x() && y_pos > rectangle_position[0].start_point.y()) &&
       (x_pos < rectangle_position[0].end_point.x() && y_pos < rectangle_position[0].end_point.y()))
    {
//        qDebug() << "left IN!";
        inside_left_grid = true;
        emit UpdateScreen();
    }
    else
    {
        inside_left_grid = false;
        emit UpdateScreen();
    }

    if((x_pos > rectangle_position[1].start_point.x() && y_pos > rectangle_position[1].start_point.y()) &&
       (x_pos < rectangle_position[1].end_point.x() && y_pos < rectangle_position[1].end_point.y()))
    {
//        qDebug() << "right IN!";
        inside_right_grid = true;
        emit UpdateScreen();
    }
    else
    {
        inside_right_grid = false;
        emit UpdateScreen();
    }
}

void SquidGame::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(g_stage == EStage::Title)
        {
            handle->Start();
            ui->label_Soi->setVisible(false);
            ui->label_Mark->setVisible(false);
            ui->label_Title->setVisible(false);
            ui->label_Grid->setVisible(true);
            ui->label_GridNum->setVisible(true);
            ui->label_StepTotal->setVisible(true);
            return;
        }
        else if(g_stage == EStage::Dead)
        {
            g_stage = EStage::Playing;
            ui->label_Next->setVisible(false);

            SquidGame::pThis->PlayAudio(opening_music, true);

            emit UpdateScreen();
            return;
        }
        else if(g_stage==EStage::Win)
        {
            return;
        }

        if((x_pos > rectangle_position[0].start_point.x() && y_pos > rectangle_position[0].start_point.y()) &&
           (x_pos < rectangle_position[0].end_point.x() && y_pos < rectangle_position[0].end_point.y()))
        {
            qDebug() << "GO left!";
            handle->SelectItem(ESelection::Left);
        }
        else if((x_pos > rectangle_position[1].start_point.x() && y_pos > rectangle_position[1].start_point.y()) &&
           (x_pos < rectangle_position[1].end_point.x() && y_pos < rectangle_position[1].end_point.y()))
        {
            qDebug() << "GO right!";
            handle->SelectItem(ESelection::Right);
        }
    }
}

void SquidGame::PlayAudio(QString path, bool stop_first)
{
    //尚未播放過任何多媒體，就執行此function，會造成process crash
//    libvlc_media_player_stop(mp);

    qDebug() << "play audio";

    if(stop_first)
    {
        libvlc_media_player_stop(vlc_media_player);
        libvlc_media_player_release(vlc_media_player);
    }

    QByteArray byte_arr = path.toLocal8Bit();

    //提供絕對路徑
//    QString full_path = qApp->applicationDirPath() + path;
//    QByteArray byte_arr = full_path.toLocal8Bit();

    const char *str = byte_arr.data();

    vlc_media = libvlc_media_new_path(vlc_instance, str);
    vlc_media_player = libvlc_media_player_new_from_media(vlc_media);
    libvlc_media_player_set_hwnd(vlc_media_player, (void *)this->winId());
    libvlc_media_release(vlc_media);
    libvlc_media_player_play(vlc_media_player);
}

void SquidGame::PlayVideo(QString path, bool stop_first)
{
    qDebug() << "play video";

    if(stop_first)
    {
        libvlc_media_player_stop(vlc_media_player);
        libvlc_media_player_release(vlc_media_player);
    }

    QByteArray byte_arr = path.toLocal8Bit();
    const char *str = byte_arr.data();

    vlc_media = libvlc_media_new_path(vlc_instance, str);
    vlc_media_player = libvlc_media_player_new_from_media(vlc_media);
    stop_thread->vlc_media_player = vlc_media_player;
    libvlc_media_player_set_hwnd(vlc_media_player, (void *)this->winId());
    libvlc_media_release(pThis->vlc_media);
    libvlc_media_player_play(pThis->vlc_media_player);
}

void Presenter::ShowImage(EStage stage)
{
    g_stage = stage;

    if(g_stage == EStage::Dead)
    {
        qDebug() << "dead event";

        SquidGame::pThis->timer->start(5000);

        SquidGame::pThis->PlayVideo(SquidGame::pThis->dead_video, true);
    }
    else if(g_stage == EStage::Win)
    {
        SquidGame::pThis->PlayVideo(SquidGame::pThis->win_video, true);
    }

    emit SquidGame::pThis->UpdateScreen();
}

void Presenter::ShowProgress(unsigned char progress)
{
    QString str;
    str=QString::number((int)progress);
    SquidGame::pThis->ui->label_GridNum->setText(str);
}
