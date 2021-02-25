QT -= core gui network widgets

CONFIG += c++11

RESOURCES += \
    win_resources.rc

HEADERS += \
    MainFrame.h \
    dbMeter.h \
    wxcrafter.h

SOURCES += \
    MainFrame.cpp \
    dbMeter.cpp \
    main.cpp \
    wxcrafter_bitmaps.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../MixFaceLibrary/release/ -lMixFaceLibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../MixFaceLibrary/debug/ -lMixFaceLibrary
else:unix: LIBS += -L$$OUT_PWD/../MixFaceLibrary/ -lMixFaceLibrary

INCLUDEPATH += $$PWD/../MixFaceLibrary
DEPENDPATH += $$PWD/../MixFaceLibrary

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../MixFaceLibrary/release/libMixFaceLibrary.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../MixFaceLibrary/debug/libMixFaceLibrary.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../MixFaceLibrary/release/MixFaceLibrary.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../MixFaceLibrary/debug/MixFaceLibrary.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../MixFaceLibrary/libMixFaceLibrary.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../OSCPACK/release/ -lOSCPACK
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../OSCPACK/debug/ -lOSCPACK
else:unix: LIBS += -L$$OUT_PWD/../OSCPACK/ -lOSCPACK

INCLUDEPATH += $$PWD/../OSCPACK
DEPENDPATH += $$PWD/../OSCPACK

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../OSCPACK/release/libOSCPACK.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../OSCPACK/debug/libOSCPACK.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../OSCPACK/release/OSCPACK.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../OSCPACK/debug/OSCPACK.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../OSCPACK/libOSCPACK.a

win32{
    LIBS += -lkernel32 -lws2_32 -lwinmm
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../DebugLibrary/release/ -lDebugLibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../DebugLibrary/debug/ -lDebugLibrary
else:unix: LIBS += -L$$OUT_PWD/../DebugLibrary/ -lDebugLibrary

INCLUDEPATH += $$PWD/../DebugLibrary
DEPENDPATH += $$PWD/../DebugLibrary

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../DebugLibrary/release/libDebugLibrary.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../DebugLibrary/debug/libDebugLibrary.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../DebugLibrary/release/DebugLibrary.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../DebugLibrary/debug/DebugLibrary.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../DebugLibrary/libDebugLibrary.a



#Boost libraries
INCLUDEPATH += $$PWD/../../../Boost/boost_1_75_0
DEPENDPATH += $$PWD/../../../Boost/boost_1_75_0



#wxWidgets libraries
INCLUDEPATH += $$PWD/../../../wxWidgets-3.0.5/include
DEPENDPATH += $$PWD/../../../wxWidgets-3.0.5/include
INCLUDEPATH += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/mswu
DEPENDPATH += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/mswu


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxbase30u
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxbase30ud
else:unix: LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxbase30u

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxbase30u.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxbase30ud.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxbase30u.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxbase30ud.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxbase30u.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxbase30u_xml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxbase30ud_xml
else:unix: LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxbase30u_xml

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxbase30u_xml.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxbase30ud_xml.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxbase30u_xml.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxbase30ud_xml.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxbase30u_xml.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxexpat
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxexpatd
else:unix: LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxexpat

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxexpat.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxexpatd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxexpat.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxexpatd.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxexpat.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxjpeg
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxjpegd
else:unix: LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxjpeg

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxjpeg.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxjpegd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxjpeg.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxjpegd.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxjpeg.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30u_adv
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30ud_adv
else:unix: LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30u_adv

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30u_adv.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30ud_adv.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxmsw30u_adv.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxmsw30ud_adv.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30u_adv.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30u_aui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30ud_aui
else:unix: LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30u_aui

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30u_aui.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30ud_aui.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxmsw30u_aui.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxmsw30ud_aui.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30u_aui.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30u_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30ud_core
else:unix: LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30u_core

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30u_core.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30ud_core.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxmsw30u_core.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxmsw30ud_core.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30u_core.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30u_html
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30ud_html
else:unix: LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30u_html

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30u_html.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30ud_html.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxmsw30u_html.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxmsw30ud_html.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30u_html.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30u_xrc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30ud_xrc
else:unix: LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30u_xrc

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30u_xrc.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30ud_xrc.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxmsw30u_xrc.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxmsw30ud_xrc.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30u_xrc.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxpng
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxpngd
else:unix: LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxpng

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxpng.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxpngd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxpng.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxpngd.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxpng.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxregexu
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxregexud
else:unix: LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxregexu

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxregexu.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxregexud.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxregexu.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxregexud.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxregexu.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxscintilla
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxscintillad
else:unix: LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxscintilla

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxscintilla.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxscintillad.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxscintilla.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxscintillad.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxscintilla.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxtiff
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxtiffd
else:unix: LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxtiff

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxtiff.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxtiffd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxtiff.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxtiffd.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxtiff.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxzlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxzlibd
else:unix: LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxzlib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxzlib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxzlibd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxzlib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxzlibd.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxzlib.a




win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxbase30u_net
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxbase30ud_net
else:unix: LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxbase30u_net

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxbase30u_net.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxbase30ud_net.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxbase30u_net.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxbase30ud_net.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxbase30u_net.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30u_gl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30ud_gl
else:unix: LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30u_gl

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30u_gl.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30ud_gl.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxmsw30u_gl.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxmsw30ud_gl.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30u_gl.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30u_media
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30ud_media
else:unix: LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30u_media

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30u_media.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30ud_media.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxmsw30u_media.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxmsw30ud_media.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30u_media.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30u_propgrid
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30ud_propgrid
else:unix: LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30u_propgri

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30u_propgrid.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30ud_propgrid.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxmsw30u_propgrid.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxmsw30ud_propgrid.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30u_propgrid.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30u_ribbon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30ud_ribbon
else:unix: LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30u_ribbon

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30u_ribbon.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30ud_ribbon.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxmsw30u_ribbon.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxmsw30ud_ribbon.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30u_ribbon.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30u_richtext
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30ud_richtext
else:unix: LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30ud_richtext

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30u_richtext.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30ud_richtext.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxmsw30u_richtext.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxmsw30ud_richtext.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30ud_richtext.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30u_stc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30ud_stc
else:unix: LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30u_stc

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30u_stc.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30ud_stc.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxmsw30u_stc.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxmsw30ud_stc.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30u_stc.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30u_webview
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30ud_webview
else:unix: LIBS += -L$$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/ -lwxmsw30u_webview

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30u_webview.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30ud_webview.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxmsw30u_webview.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/wxmsw30ud_webview.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../wxWidgets-3.0.5/lib/gcc_lib/libwxmsw30u_webview.a
