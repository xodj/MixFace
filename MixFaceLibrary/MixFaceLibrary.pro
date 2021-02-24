TEMPLATE = lib
CONFIG += staticlib

QT       += core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    MixFaceKeeper.h \
    MixFaceLibrary.h \
    MixFaceLinker.h \
    MixFaceListener.h

SOURCES += \
    MixFaceKeeper.cpp \
    MixFaceLibrary.cpp \
    MixFaceLinker.cpp

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

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
