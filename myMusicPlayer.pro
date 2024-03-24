QT += core gui network


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG -= debug_and_release debug_and_release_target


macx {

LIBS += $(shell pkg-config --libs mpv taglib)
INCLUDEPATH +=  $(shell pkg-config --cflags mpv taglib

# LIBS += -L/opt/homebrew/Cellar/taglib/1.13.1/lib -ltag -lz
# INCLUDEPATH += /opt/homebrew/Cellar/taglib/1.13.1/include


# INCLUDEPATH += /opt/homebrew/Cellar/mpv/0.37.0_3/include
# LIBS += -L/opt/homebrew/Cellar/mpv/0.37.0_3/lib -lmpv

}


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    HttpClient.cpp \
    main.cpp \
    mainwindow.cpp \
    audioplayer.cpp \
    managetreelist.cpp \
    metadata.cpp \
    asyncimageloader.cpp \
    playqueue.cpp


HEADERS += \
    HttpClient.h \
    mainwindow.h \
    audioplayer.h \
    managetreelist.h \
    metadata.h \
    asyncimageloader.h \
    playqueue.h

FORMS += \
    mainwindow.ui

TARGET = myMusicPlayer

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc \
    styles.qrc



