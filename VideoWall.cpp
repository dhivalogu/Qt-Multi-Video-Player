/******************************************************************************
    VideoWall:  this file is part of QtAV examples
    Copyright (C) 2012-2016 Wang Bin <wbsecg1@gmail.com>

*   This file is part of QtAV

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include "VideoWall.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QEvent>
#include <QFileDialog>
#include <QGridLayout>
#include <QInputDialog>
#include <QKeyEvent>
#include <QMenu>
#include <QMessageBox>
#include <QMimeData>
#include <QtCore/QUrl>
#include <QtAV/AudioOutput.h>
#include <QtAVWidgets>
#include <QDebug>
#include<QStyleFactory>
#include <QToolButton>
#include <QSlider>
#include <dos.h>
#include <QPixelFormat>
#include <QWidgetAction>
#include <QDoubleSpinBox>
#include <QPoint>
#include <QtCore/QtDebug>
#include <QtCore/QLocale>
#include <QtCore/QTimer>
#include <QTimeEdit>
#include <QLabel>
#include <QApplication>
#include <QDesktopWidget>
#include <QDesktopServices>
#include <QtCore/QFileInfo>
#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>
#include <QtCore/QUrl>
#include <QGraphicsOpacityEffect>
#include <QComboBox>
#include <QResizeEvent>
#include <QWidgetAction>
#include <QLayout>
#include <QPushButton>
#include <QtAV/VideoCapture.h>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include <QToolButton>
#include <QToolTip>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QStyleFactory>
using namespace QtAV;
const int kSyncInterval = 100;
int i=0;

VideoWall::VideoWall(QObject *parent) :
    QObject(parent),r(3),c(3),view(0),menu(0)
  , vid(QString::fromLatin1("qpainter"))
{
    QtAV::Widgets::registerRenderers();
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    clock = new AVClock(this);
    clock->setClockType(AVClock::ExternalClock);
    view = new QWidget;
    view->setWindowTitle(QString::fromLatin1("ADA Player"));
    vl = new QVBoxLayout;
    if (view) {
        qDebug("WA_OpaquePaintEvent=%d", view->testAttribute(Qt::WA_OpaquePaintEvent));
        view->resize(qApp->desktop()->size());
        view->move(QPoint(0, 0));
        view->showMaximized();
    }

    view->installEventFilter(this);
}

VideoWall::~VideoWall()
{
    if (menu) {
        delete menu;
        menu = 0;
    }
    if (!players.isEmpty()) {
        foreach (AVPlayer *player, players) {
            player->stop();
            VideoRenderer* renderer = player->renderer();
            if (renderer->widget()) {
                renderer->widget()->close(); //TODO: rename
                if (!renderer->widget()->testAttribute(Qt::WA_DeleteOnClose) && !renderer->widget()->parent())
                    delete renderer;
                delete player;
            }
        }
        players.clear();
    }
    delete view;
}

void VideoWall::setVideoRendererTypeString(const QString &vt)
{
    vid = vt.toLower();
}

void VideoWall::setRows(int n)
{
    r = n;
}

void VideoWall::setCols(int n)
{
    c = n;
}

int VideoWall::rows() const
{
    return r;
}

int VideoWall::cols() const
{
    return c;
}

void VideoWall::show()
{
    if (!players.isEmpty()) {
        foreach (AVPlayer *player, players) {
            player->stop();
            VideoRenderer* renderer = player->renderer();
            if (renderer->widget()) {
                renderer->widget()->close();
                if (!renderer->widget()->testAttribute(Qt::WA_DeleteOnClose) && !renderer->widget()->parent())
                    delete renderer;
                delete player;
            }
        }
        players.clear();
    }
    qDebug("show wall: %d x %d", r, c);


    QHBoxLayout *hb = new QHBoxLayout();
    QHBoxLayout *hb1 = new QHBoxLayout();
    QHBoxLayout *hb2 = new QHBoxLayout();
    QHBoxLayout *hb3 = new QHBoxLayout();
    QHBoxLayout *hb4 = new QHBoxLayout();

    mpPlayerLayout = new QVBoxLayout();
    mpControl = new QWidget();
    mpControl->setMaximumHeight(30);

    //mpPreview = new QLable(this);

    mpTimeSlider = new QSlider(mpControl);
    mpTimeSlider->setDisabled(true);
    mpTimeSlider->setTracking(true);
    mpTimeSlider->setOrientation(Qt::Horizontal);
    mpTimeSlider->setMinimum(0);
    mpCurrent = new QLabel(mpControl);
    mpCurrent->setToolTip(tr("Current time"));
    mpCurrent->setContentsMargins(QMargins(2, 2, 2, 2));
    mpCurrent->setText(QString::fromLatin1("00:00:00"));
    mpEnd = new QLabel(mpControl);
    mpEnd->setToolTip(tr("Duration"));
    mpEnd->setContentsMargins(QMargins(2, 2, 2, 2));
    mpEnd->setText(QString::fromLatin1("00:00:00"));
    mpTitle = new QLabel(mpControl);
    mpTitle->setToolTip(tr("Render engine"));
    mpTitle->setText(QString::fromLatin1("QPainter"));
    mpTitle->setIndent(8);
    mpSpeed = new QLabel(QString::fromLatin1("1.00"));
    mpSpeed->setContentsMargins(QMargins(1, 1, 1, 1));
    mpSpeed->setToolTip(tr("Speed. Ctrl+Up/Down"));

    mpPlayPauseBtn = new QToolButton(mpControl);
    mpPlayPauseBtn->setIcon(QIcon(QString::fromLatin1("theme/dark/play.svg")));
    mpStopBtn = new QToolButton(mpControl);
    mpStopBtn->setIcon(QIcon(QString::fromLatin1("theme/dark/stop.svg")));
    mpBackwardBtn = new QToolButton(mpControl);
    mpBackwardBtn->setIcon(QIcon(QString::fromLatin1("theme/dark/backward.svg")));

    mpForwardBtn = new QToolButton(mpControl);
    mpForwardBtn->setIcon(QIcon(QString::fromLatin1("theme/dark/forward.svg")));
    mpOpenBtn = new QToolButton(mpControl);
    mpOpenBtn->setToolTip(tr("Open"));
    mpOpenBtn->setIcon(QIcon(QString::fromLatin1("theme/dark/open.svg")));

    /*mpInfoBtn = new QToolButton();
    mpInfoBtn->setToolTip(QString::fromLatin1("Media information"));
    mpInfoBtn->setIcon(QIcon(QString::fromLatin1("theme/dark/info.svg")));*/
    mpCaptureBtn = new QToolButton();
    mpCaptureBtn->setToolTip(tr("Capture"));
    mpCaptureBtn->setIcon(QIcon(QString::fromLatin1("theme/dark/capture.svg")));
    /*mpVolumeBtn = new QToolButton();
    mpVolumeBtn->setIcon(QIcon(QString::fromLatin1("theme/dark/sound.svg")));

    mpVolumeSlider = new QSlider();
    mpVolumeSlider->hide();
    mpVolumeSlider->setOrientation(Qt::Horizontal);
    mpVolumeSlider->setMinimum(0);
    const int kVolumeSliderMax = 100;
    mpVolumeSlider->setMaximum(kVolumeSliderMax);
    //mpVolumeSlider->setMaximumHeight(12);
    mpVolumeSlider->setMaximumWidth(88);
    mpVolumeSlider->setValue(int(5));
    //setVolume();

    mpMenuBtn = new QToolButton();
    mpMenuBtn->setIcon(QIcon(QString::fromLatin1(":/theme/dark/menu.svg")));
    mpMenuBtn->setAutoRaise(true);
    mpMenuBtn->setPopupMode(QToolButton::InstantPopup);
    QMenu *subMenu = 0;
    QWidgetAction *pWA = 0;
    mpMenu = new QMenu(mpMenuBtn);
    subMenu = new QMenu(tr("Speed"));
    mpMenu->addMenu(subMenu);
    QDoubleSpinBox *pSpeedBox = new QDoubleSpinBox(0);
    pSpeedBox->setRange(0.01, 20);
    pSpeedBox->setValue(1.0);
    pSpeedBox->setSingleStep(0.01);
    pSpeedBox->setCorrectionMode(QAbstractSpinBox::CorrectToPreviousValue);
    pWA = new QWidgetAction(0);
    pWA->setDefaultWidget(pSpeedBox);
    subMenu->addAction(pWA);*/



    VideoRendererId v = VideoRendererId_Widget;
    if (vid == QLatin1String("gl"))
        v = VideoRendererId_GLWidget2;
    else if (vid == QLatin1String("opengl"))
        v = VideoRendererId_OpenGLWidget;
    else if (vid == QLatin1String("d2d"))
        v = VideoRendererId_Direct2D;
    else if (vid == QLatin1String("gdi"))
        v = VideoRendererId_GDI;
    else if (vid == QLatin1String("x11"))
        v = VideoRendererId_X11;
    else if (vid == QLatin1String("xv"))
        v = VideoRendererId_XV;
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            WidgetRenderer* renderer = new WidgetRenderer();
            renderer->widget()->setWindowFlags(renderer->widget()->windowFlags()| Qt::FramelessWindowHint);
            renderer->widget()->setAttribute(Qt::WA_DeleteOnClose);
            renderer->show();
            AVPlayer *player = new AVPlayer;
            player->setVideoDecoderPriority(QStringList() << "FFmpeg" );
            player->setRenderer(renderer);
            connect(player, SIGNAL(started()), SLOT(changeClockType()));
            players.append(player);
            if (i==0)
            {
                hb2->addWidget(renderer->widget());
            }
            if (i==1)
            {
                hb3->addWidget(renderer->widget());
            }
            if (i==2)
            {
                hb4->addWidget(renderer->widget());
            }
        }
    }
    //vl->addLayout(mpPlayerLayout);
    QHBoxLayout *controlLayout = new QHBoxLayout();
    controlLayout->setSpacing(4);
    controlLayout->setContentsMargins(QMargins(2, 2, 2, 2));
    mpControl->setLayout(controlLayout);
    mpTitle->setText("Media Player");
    controlLayout->addWidget(mpCurrent);
    controlLayout->addWidget(mpTitle);
    QSpacerItem *space = new QSpacerItem(mpPlayPauseBtn->width(), mpPlayPauseBtn->height(), QSizePolicy::MinimumExpanding);
    //controlLayout->addSpacerItem(space);
    view->setStyleSheet("background-color:white");
    mpOpenBtn->setStyleSheet("background-color:white");
            mpPlayPauseBtn->setStyleSheet("background-color:white");
            mpStopBtn->setStyleSheet("background-color:white");
    controlLayout->addWidget(mpOpenBtn);
    controlLayout->addWidget(mpPlayPauseBtn);
    controlLayout->addWidget(mpStopBtn);
    /*controlLayout->addWidget(mpVolumeSlider);

    controlLayout->addWidget(mpVolumeBtn);*/
    controlLayout->addWidget(mpCaptureBtn);


    controlLayout->addWidget(mpBackwardBtn);
    controlLayout->addWidget(mpForwardBtn);

   /* controlLayout->addWidget(mpInfoBtn);
    controlLayout->addWidget(mpSpeed);
    controlLayout->addWidget(mpSetupBtn);
    controlLayout->addWidget(mpMenuBtn);
    controlLayout->addWidget(mpEnd);*/

    vl->addWidget(mpControl);
    QVBoxLayout *vl1=new QVBoxLayout();
    vl1->addLayout(hb2);
    vl1->addLayout(hb3);
    vl1->addLayout(hb4);
    vl->addLayout(vl1);
    vl->addLayout(hb1);
    //vl->addWidget(mpControl);
    vl->addWidget(mpTimeSlider);
    view->setLayout(vl);
    connect(mpCaptureBtn, SIGNAL(clicked()), SLOT(capture()));
    connect(mpOpenBtn, SIGNAL(clicked()), SLOT(openLocalFile()));
    connect(mpPlayPauseBtn, SIGNAL(clicked()), SLOT(pause()));

    connect(mpTimeSlider, SIGNAL(sliderMoved(int)), SLOT(seek(int)));
    connect(mpTimeSlider, SIGNAL(sliderPressed()), SLOT(seek()));
    connect(players[0], SIGNAL(started()), this, SLOT(onStartPlay()));
    foreach (AVPlayer *player, players) {

        connect(mpStopBtn, SIGNAL(clicked()), player, SLOT(stop()));
        connect(player->videoCapture(), SIGNAL(saved(QString)), SLOT(onCaptureSaved(QString)));
    }
}
void VideoWall::pause()
{
        clock->pause(!clock->isPaused());
        qDebug()<<"Pause";
    if(!players[0]->isPaused())
    {
        mpPlayPauseBtn->setIcon(QIcon(QString::fromLatin1("theme/dark/play.svg")));
    }
    else {
        mpPlayPauseBtn->setIcon(QIcon(QString::fromLatin1("theme/dark/pause.svg")));
    }
    foreach (AVPlayer* player, players) {
        player->pause(!player->isPaused());
    }

}
void VideoWall::play(const QString &file)
{
    if (players.isEmpty())
        return;
    clock->reset();
    clock->start();
    foreach (AVPlayer *player, players) {

        player->play(file);


    }

    timer_id = startTimer(kSyncInterval);
}

void VideoWall::stop()
{
    clock->reset();
    killTimer(timer_id);
    foreach (AVPlayer* player, players) {
        player->stop(); //check playing?
    }
}

void VideoWall::openLocalFile()
{
    stop();

    try {
        clock->reset();

       foreach (AVPlayer* player, players) {


           player->setInterruptOnTimeout(false);
           player->setInterruptTimeout(-1);
           player->setBufferMode(QtAV::BufferPackets);
           player->setBufferValue(2);

           //player->setPosition(0);
           //TODO: load all playe
       }


        QString file;

       foreach (AVPlayer* player, players) {

           file = QFileDialog::getOpenFileName(0, tr("Open a video"));
           player->setFile(file);
           player->load();


       }
       foreach (AVPlayer* player, players) {

           player->play();


       }
       onStartPlay();
       /*foreach (AVPlayer* player, players) {

           player->pause(!player->isPaused());


       }

       foreach (AVPlayer* player, players) {

           player->pause(!player->isPaused());


       }*/
       clock->start();
       timer_id = startTimer(kSyncInterval);
    }
    catch (const std::bad_alloc&) {
        qDebug()<<"Memory";
    }


}

void VideoWall::openUrl()
{
    QString url = QInputDialog::getText(0, tr("Open an url"), tr("Url"));
    if (url.isEmpty())
        return;
    stop();
    clock->reset();
    clock->start();
    timer_id = startTimer(kSyncInterval);
    foreach (AVPlayer* player, players) {
        player->setFile(url);
        player->play(); //TODO: load all players before play
    }
}

void VideoWall::about()
{
    QMessageBox::about(0, tr("About QtAV"), QString::fromLatin1("<h3>%1</h3>\n\n%2")
                       .arg(tr("This is a demo for playing and synchronising multiple players"))
                       .arg(aboutQtAV_HTML()));
}



bool VideoWall::eventFilter(QObject *watched, QEvent *event)
{
    //qDebug("EventFilter::eventFilter to %p", watched);
    Q_UNUSED(watched);
    if (players.isEmpty())
        return false;
    QEvent::Type type = event->type();
    switch (type) {
    case QEvent::KeyPress: {
        //qDebug("Event target = %p %p", watched, player->renderer);
        //avoid receive an event multiple times
        QKeyEvent *key_event = static_cast<QKeyEvent*>(event);
        int key = key_event->key();
        Qt::KeyboardModifiers modifiers = key_event->modifiers();
        switch (key) {
        case Qt::Key_F: {
            QWidget *w = qApp->activeWindow();
            if (!w)
                return false;
            if (w->isFullScreen())
                w->showNormal();
            else
                w->showFullScreen();
        }
            break;
        case Qt::Key_N: //check playing?
            foreach (AVPlayer* player, players) {
                player->stepForward();
            }
            break;

        case Qt::Key_O: {
            if (modifiers == Qt::ControlModifier) {
                openLocalFile();
                return true;
            } else/* if (m == Qt::NoModifier) */{
                return false;
            }
        }
            break;
        case Qt::Key_P:
            clock->reset();
            clock->start();
            foreach (AVPlayer* player, players) {
                player->play();
            }
            timer_id = startTimer(kSyncInterval);
            break;
        case Qt::Key_S:
            stop();
            break;
        case Qt::Key_Space: //check playing?

            foreach (AVPlayer* player, players) {
                player->pause(!player->isPaused());
            }
            break;
        case Qt::Key_Up:
            foreach (AVPlayer* player, players) {
                if (player->audio()) {
                    qreal v = player->audio()->volume();
                    if (v > 0.5)
                        v += 0.1;
                    else if (v > 0.1)
                        v += 0.05;
                    else
                        v += 0.025;
                    player->audio()->setVolume(v);
                }
            }
            break;
        case Qt::Key_Down:
            foreach (AVPlayer* player, players) {
                if (player->audio()) {
                    qreal v = player->audio()->volume();
                    if (v > 0.5)
                        v -= 0.1;
                    else if (v > 0.1)
                        v -= 0.05;
                    else
                        v -= 0.025;
                    player->audio()->setVolume(v);
                }
            }
            break;
        case Qt::Key_Left: {
            qDebug("<-");
            const qint64 newPos = clock->value()*1000.0 - 2000.0;
            clock->updateExternalClock(newPos);
            foreach (AVPlayer* player, players) {
                player->setPosition(newPos);
            }
        }
            break;
        case Qt::Key_Right: {
            qDebug("->");
            const qint64 newPos = clock->value()*1000.0 + 2000.0;
            clock->updateExternalClock(newPos);
            foreach (AVPlayer* player, players) {
                player->setPosition(newPos);
            }
        }
            break;
        case Qt::Key_M:
            foreach (AVPlayer* player, players) {
                if (player->audio()) {
                    player->audio()->setMute(!player->audio()->isMute());
                }
            }
            break;
        default:
            return false;
        }
        break;
    }
    case QEvent::ContextMenu: {
        QContextMenuEvent *e = static_cast<QContextMenuEvent*>(event);
        if (!menu) {
            menu = new QMenu();
            menu->addAction(tr("Open"), this, SLOT(openLocalFile()));
            menu->addAction(tr("Open Url"), this, SLOT(openUrl()));
            menu->addSeparator();
            menu->addAction(tr("About"), this, SLOT(about()));
            menu->addAction(tr("Help"), this, SLOT(help()));
            menu->addSeparator();
            menu->addAction(tr("About Qt"), qApp, SLOT(aboutQt()));
        }
        menu->popup(e->globalPos());
        menu->exec();
    }
    case QEvent::DragEnter:
    case QEvent::DragMove: {
        QDropEvent *e = static_cast<QDropEvent*>(event);
        e->acceptProposedAction();
    }
        break;
    case QEvent::Drop: {
        QDropEvent *e = static_cast<QDropEvent*>(event);
        QString path = e->mimeData()->urls().first().toLocalFile();
        stop();
        play(path);
        e->acceptProposedAction();
    }
        break;
    default:
        return false;
    }
    return true; //false: for text input
}

void VideoWall::timerEvent(QTimerEvent *e)
{
    if (e->timerId() != timer_id) {
        qDebug("Not clock id");
        return;
    }
    if (!clock->isActive()) {
        qDebug("clock not running");
        return;
    }
    foreach (AVPlayer *player, players) {
        player->masterClock()->updateExternalClock(*clock);
    }
}

void VideoWall::changeClockType()
{
    AVPlayer* player = qobject_cast<AVPlayer*>(sender());
    player->masterClock()->setClockAuto(false);
    player->masterClock()->setClockType(AVClock::ExternalClock);
}
void VideoWall::loade()
{
    i++;
    qDebug()<<i;
}
void VideoWall::seek(int value)
{
    /*pause();
    foreach (AVPlayer *player, players) {
        player->setSeekType(AccurateSeek);
        if (!player->isPlaying())
            return;
         player->seek((qint64)value);
    }
    pause();

   /* if (!m_preview || !Config::instance().previewEnabled())
        return;
    m_preview->setTimestamp(value);
    m_preview->preview();
    m_preview->setWindowFlags(m_preview->windowFlags() |Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    m_preview->resize(Config::instance().previewWidth(), Config::instance().previewHeight());
    m_preview->show();*/
}

void VideoWall::seek()
{/*
    seek(mpTimeSlider->value());*/
}
void VideoWall::onStartPlay()
{

   qDebug()<<"Started";
    mpPlayPauseBtn->setIcon(QIcon(QString::fromLatin1("theme/dark/pause.svg")));
    /*mpTimeSlider->setMinimum(players[0]->mediaStartPosition());
    qint64 a=0;
    foreach (AVPlayer *player, players) {
        if(player->mediaStopPosition() > a)
        {
            a=player->mediaStopPosition();
        }
    }
    mpTimeSlider->setMaximum(a);
    mpTimeSlider->setValue(0);
    mpTimeSlider->setEnabled(true);
    /*mpEnd->setText(QTime(0, 0, 0).addMSecs(mpPlayer->mediaStopPosition()).toString(QString::fromLatin1("HH:mm:ss")));
    setVolume();*/
   }
void VideoWall::capture()
{
    //m_player->captureVideo();
    foreach (AVPlayer *player, players) {
    player->videoCapture()->capture();
    }
}

void VideoWall::onCaptureSaved(const QString &path)
{
    view->setWindowTitle(tr("saved to: ") + path);
}
