CONFIG -= qt
CONFIG -= gui

TEMPLATE = lib
#Comment for dynamic library
#CONFIG += staticlib

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

TARGET = MixFaceLibrary
RC_ICONS = ../MixFaceMedia/mf_icon.ico
#win32:VERSION = 2.0.3.55
#else:VERSION = 2.0.3
QMAKE_TARGET_COMPANY = "xo.dj"
QMAKE_TARGET_DESCRIPTION = "Manipulating mixing consoles library."
QMAKE_TARGET_COPYRIGHT = "Commercial license."
QMAKE_TARGET_PRODUCT = "MixFace Library"
QMAKE_TARGET_ORIGINAL_FILENAME = MixFaceLibrary.dll

HEADERS += \
    MixFaceKeeper.h \
    MixFaceLibrary.h \
    MixFaceLinker.h \
    MixFaceListener.h \
    Timer.hpp

SOURCES += \
    MixFaceKeeper.cpp \
    MixFaceLibrary.cpp \
    MixFaceLinker.cpp

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
win32:{
INCLUDEPATH += $$PWD/../../../../../Boost/boost_1_74_0
DEPENDPATH += $$PWD/../../../../../Boost/boost_1_74_0

win32:!win32-g++*:contains(QMAKE_TARGET.arch, x86_64):{ # x32
#C
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../Boost/boost_1_74_0/stage/lib/ -lboost_thread-mgw8-mt-x32-1_74
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../Boost/boost_1_74_0/stage/lib/ -lboost_thread-mgw8-mt-d-x32-1_74
#C++
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../Boost/boost_1_74_0/stage/lib/libboost_thread-mgw8-mt-x32-1_74.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../Boost/boost_1_74_0/stage/lib/libboost_thread-mgw8-mt-d-x32-1_74.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../Boost/boost_1_74_0/stage/lib/boost_thread-mgw8-mt-x32-1_74.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../Boost/boost_1_74_0/stage/lib/boost_thread-mgw8-mt-d-x32-1_74.lib
} else: { # x64
#C
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../Boost/boost_1_74_0/stage/lib/ -lboost_thread-mgw8-mt-x64-1_74
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../Boost/boost_1_74_0/stage/lib/ -lboost_thread-mgw8-mt-d-x64-1_74
#C++
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../Boost/boost_1_74_0/stage/lib/libboost_thread-mgw8-mt-x64-1_74.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../Boost/boost_1_74_0/stage/lib/libboost_thread-mgw8-mt-d-x64-1_74.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../Boost/boost_1_74_0/stage/lib/boost_thread-mgw8-mt-x64-1_74.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../Boost/boost_1_74_0/stage/lib/boost_thread-mgw8-mt-d-x64-1_74.lib
}
}

#ANDROID
unix:!macx{
INCLUDEPATH += $$PWD/../../../Boost/boost_1_74_0/stage/include/boost-1_74
DEPENDPATH += $$PWD/../../../Boost/boost_1_74_0/stage/include/boost-1_74

LIBS += -L$$PWD/../../../Boost/boost_1_74_0/stage/lib/ -lboost_thread-clang-mt-a32-1_74
PRE_TARGETDEPS += $$PWD/../../../Boost/boost_1_74_0/stage/lib/libboost_thread-clang-mt-a32-1_74.a

LIBS += -L$$PWD/../../../Boost/boost_1_74_0/stage/lib/ -lboost_chrono-clang-mt-a32-1_74
PRE_TARGETDEPS += $$PWD/../../../Boost/boost_1_74_0/stage/lib/libboost_chrono-clang-mt-a32-1_74.a

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle.properties \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
