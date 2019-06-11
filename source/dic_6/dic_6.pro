#-------------------------------------------------
#
# Project created by QtCreator 2019-04-09T11:11:35
#
#-------------------------------------------------

QT       += core gui webchannel websockets widgets sql xml webenginewidgets multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dic_6
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mywindow.cpp \
    docxml.cpp \
    main.cpp \
    mywindow.cpp \
    searchwidget.cpp \
    serversocket.cpp \
    socketin.cpp \
    status.cpp \
    timethread.cpp \
    webpage.cpp \
    audiothread.cpp \
    searchthread.cpp \
    MyGlobalShortCut/MyGlobalShortCut.cpp \
    MyGlobalShortCut/MyWinEventFilter.cpp \
    MyGlobalShortCut/MyGlobalShortCut.cpp \
    MyGlobalShortCut/MyWinEventFilter.cpp

HEADERS += \
        mywindow.h \
    bridge.h \
    docxml.h \
    mywindow.h \
    searchwidget.h \
    serversocket.h \
    socketin.h \
    status.h \
    timethread.h \
    webpage.h \
    audiothread.h \
    searchthread.h \
    MyGlobalShortCut/MyWinEventFilter.h \
    MyGlobalShortCut/MyGlobalShortCut.h \
    MyGlobalShortCut/MyWinEventFilter.h
LIBS +=User32.LIB
FORMS += \
        mywindow.ui \
    mywindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx|win32: LIBS += -L$$PWD/../build-dic_6-Desktop_Qt_5_10_1_MSVC2015_32bit-Debug/audio/C++/ -lzplay

INCLUDEPATH += $$PWD/../build-dic_6-Desktop_Qt_5_10_1_MSVC2015_32bit-Debug/audio/C++
DEPENDPATH += $$PWD/../build-dic_6-Desktop_Qt_5_10_1_MSVC2015_32bit-Debug/audio/C++

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../build-dic_6-Desktop_Qt_5_10_1_MSVC2015_32bit-Debug/audio/C++/zplay.lib
else:unix:!macx|win32-g++: PRE_TARGETDEPS += $$PWD/../build-dic_6-Desktop_Qt_5_10_1_MSVC2015_32bit-Debug/audio/C++/libzplay.a
INCLUDEPATH += $$PWD/dev/include
LIBS += $$PWD/dev/lib/libavcodec.dll.a   \
        $$PWD/dev/lib/libavdevice.dll.a    \
        $$PWD/dev/lib/libavfilter.dll.a   \
        $$PWD/dev/lib/libavformat.dll.a   \
        $$PWD/dev/lib/libavutil.dll.a     \
        $$PWD/dev/lib/libpostproc.dll.a  \
        $$PWD/dev/lib/libswresample.dll.a \
        $$PWD/dev/lib/libswscale.dll.a
CONFIG += C++11 release
