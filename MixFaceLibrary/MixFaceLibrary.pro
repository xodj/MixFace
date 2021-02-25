TEMPLATE = lib
CONFIG += staticlib
QT += core
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS

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

#Boost libraries
INCLUDEPATH += $$PWD/../../../Boost/boost_1_75_0
DEPENDPATH += $$PWD/../../../Boost/boost_1_75_0

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../Boost/boost_1_75_0/libsOnly/ -lboost_thread-mgw8-mt-x32-1_75
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../Boost/boost_1_75_0/libsOnly/ -lboost_thread-mgw8-mt-d-x32-1_75
#else:unix: LIBS += -L$$PWD/../../../Boost/boost_1_75_0/libsOnly/ -lboost_thread-mgw8-mt-d-x32-1_75

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../Boost/boost_1_75_0/libsOnly/libboost_thread-mgw8-mt-x32-1_75.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../Boost/boost_1_75_0/libsOnly/libboost_thread-mgw8-mt-d-x32-1_75.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../Boost/boost_1_75_0/libsOnly/boost_thread-mgw8-mt-x32-1_75.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../Boost/boost_1_75_0/libsOnly/boost_thread-mgw8-mt-d-x32-1_75.lib
#else:unix: PRE_TARGETDEPS += $$PWD/../../../Boost/boost_1_75_0/libsOnly/libboost_thread-mgw8-mt-x32-1_75.a
