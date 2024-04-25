QT += core gui network


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets



QT_CONFIG -= no-pkg-config debug_and_release debug_and_release_target
CONFIG += link_pkgconfig c++17
PKGCONFIG += mpv

# macx {

# #LIBS += -L/opt/homebrew/Cellar/mpv/0.38.0/lib -L/opt/homebrew/Cellar/taglib/1.13.1/lib -lmpv -ltag -lz
# #INCLUDEPATH += -I/opt/homebrew/Cellar/libarchive/3.7.3/include -I/opt/homebrew/Cellar/mpv/0.38.0/include -I/opt/homebrew/Cellar/libass/0.17.1_1/include -I/opt/homebrew/Cellar/libunibreak/6.1/include -I/opt/homebrew/Cellar/harfbuzz/8.4.0/include/harfbuzz -I/opt/homebrew/Cellar/glib/2.80.0_2/include/glib-2.0 -I/opt/homebrew/Cellar/glib/2.80.0_2/lib/glib-2.0/include -I/opt/homebrew/opt/gettext/include -I/opt/homebrew/Cellar/pcre2/10.43/include -I/opt/homebrew/Cellar/graphite2/1.3.14/include -I/opt/homebrew/Cellar/fribidi/1.0.13/include/fribidi -I/opt/homebrew/Cellar/libplacebo/6.338.2/include -I/opt/homebrew/Cellar/shaderc/2024.0/include -I/opt/homebrew/Cellar/mujs/1.3.4/include -I/opt/homebrew/Cellar/little-cms2/2.16/include -I/opt/homebrew/Cellar/ffmpeg/7.0/include -I/opt/homebrew/Cellar/libbluray/1.3.4/include -I/opt/homebrew/Cellar/fontconfig/2.15.0/include -I/opt/homebrew/opt/freetype/include/freetype2 -I/opt/homebrew/opt/libpng/include/libpng16 -I/opt/homebrew/Cellar/luajit/2.1.1710088188/include/luajit-2.1 -I/opt/homebrew/Cellar/rubberband/3.3.0/include -I/opt/homebrew/Cellar/libsamplerate/0.2.2/include -I/opt/homebrew/Cellar/uchardet/0.0.8/include/uchardet -I/opt/homebrew/Cellar/vapoursynth/66/include/vapoursynth -I/opt/homebrew/opt/python@3.12/Frameworks/Python.framework/Versions/3.12/include/python3.12 -I/opt/homebrew/Cellar/vapoursynth/66/include/vapoursynth -I/opt/homebrew/Cellar/zimg/3.0.5/include -I/opt/homebrew/Cellar/jpeg-turbo/3.0.2/include -I/opt/homebrew/Cellar/vulkan-loader/1.3.280/../../../opt/vulkan-headers/include -I/opt/homebrew/Cellar/taglib/1.13.1/include -I/opt/homebrew/Cellar/taglib/1.13.1/include/taglib


# #LIBS += -L/opt/homebrew/Cellar/taglib/1.13.1/lib -ltag -lz
# #INCLUDEPATH += /opt/homebrew/Cellar/taglib/1.13.1/include

# # INCLUDEPATH += /opt/homebrew/Cellar/mpv/0.38.0/include
# # LIBS += -L/opt/homebrew/Cellar/mpv/0.38.0/lib -lmpv

# }


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

TARGET = "Radio Player"

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


RESOURCES += \
    icons.qrc \
    styles.qrc


