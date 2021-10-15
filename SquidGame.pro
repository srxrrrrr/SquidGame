QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    controller/bridge_game_handle.cpp \
    main.cpp \
    model/bridge_game_service.cpp \
    squid_game.cpp \
    stop_thread.cpp

HEADERS += \
    controller/IGameController.h \
    controller/bridge_game_handle.h \
    model/IGameService.h \
    model/bridge_game_service.h \
    squid_game.h \
    stop_thread.h \
    type_define.h

FORMS += \
    squidgame.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix|win32: LIBS += -L$$PWD/lib/ -llibvlc
unix|win32: LIBS += -L$$PWD/lib/ -llibvlccore
unix|win32: LIBS += -L$$PWD/lib/ -lvlc
unix|win32: LIBS += -L$$PWD/lib/ -lvlccore
