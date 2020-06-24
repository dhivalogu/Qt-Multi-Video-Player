TEMPLATE = app
QT      +=avwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG -= app_bundle

TARGET = videowall


SOURCES += main.cpp VideoWall.cpp
HEADERS += VideoWall.h
RC_ICONS = myappico1.ico

LIBS += -L$$PWD/"3rdparty/qtav" -lQtAV1 -lQtAVWidgets1

