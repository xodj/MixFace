CONFIG -= qt
CONFIG -= gui

TEMPLATE = lib
#Comment for dynamic library
#CONFIG += staticlib

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

TARGET = MixFaceLibrary
win32: {
#VERSION = 2.0.3.58
#TARGET_EXT = .dll
QMAKE_TARGET_COMPANY = "xo.dj"
QMAKE_TARGET_DESCRIPTION = "Manipulating mixing consoles library."
QMAKE_TARGET_COPYRIGHT = "Commercial license."
QMAKE_TARGET_PRODUCT = "MixFace Library"
QMAKE_TARGET_ORIGINAL_FILENAME = MixFaceLibrary.dll
RC_FILE += MixFaceLibrary.rc
} else: VERSION = 2.0.3

HEADERS += \
    MixFaceKeeper.h \
    MixFaceLibrary.h \
    MixFaceLibrary.rc \
    MixFaceLinker.h \
    MixFaceListener.h \
    Timer.hpp

SOURCES += \
    MixFaceKeeper.cpp \
    MixFaceLibrary.cpp \
    MixFaceLinker.cpp

INCLUDEPATH += $$PWD/../OSCPACK
DEPENDPATH += $$PWD/../OSCPACK
INCLUDEPATH += $$PWD/../DebugLibrary
DEPENDPATH += $$PWD/../DebugLibrary

# WINDOWS
win32:{
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../DebugLibrary/release/ -lDebugLibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../DebugLibrary/debug/ -lDebugLibrary
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../DebugLibrary/release/libDebugLibrary.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../DebugLibrary/debug/libDebugLibrary.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../DebugLibrary/release/DebugLibrary.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../DebugLibrary/debug/DebugLibrary.lib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../OSCPACK/release/ -lOSCPACK
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../OSCPACK/debug/ -lOSCPACK
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../OSCPACK/release/libOSCPACK.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../OSCPACK/debug/libOSCPACK.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../OSCPACK/release/OSCPACK.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../OSCPACK/debug/OSCPACK.lib

LIBS += -lkernel32 -lws2_32 -lwinmm

INCLUDEPATH += $$PWD/../../boost_1_74_0/
DEPENDPATH += $$PWD/../../boost_1_74_0/

contains(QT_ARCH, i386):{ # x32
#C
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../boost_1_74_0/stage/lib/ -lboost_thread-mgw8-mt-x32-1_74
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../boost_1_74_0/stage/lib/ -lboost_thread-mgw8-mt-d-x32-1_74
#C++
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../boost_1_74_0/stage/lib/libboost_thread-mgw8-mt-x32-1_74.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../boost_1_74_0/stage/lib/libboost_thread-mgw8-mt-d-x32-1_74.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../boost_1_74_0/stage/lib/boost_thread-mgw8-mt-x32-1_74.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../boost_1_74_0/stage/lib/boost_thread-mgw8-mt-d-x32-1_74.lib
} else: { # x64
#C
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../boost_1_74_0/stage/lib/ -lboost_thread-mgw8-mt-x64-1_74
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../boost_1_74_0/stage/lib/ -lboost_thread-mgw8-mt-d-x64-1_74
#C++
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../boost_1_74_0/stage/lib/libboost_thread-mgw8-mt-x64-1_74.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../boost_1_74_0/stage/lib/libboost_thread-mgw8-mt-d-x64-1_74.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../boost_1_74_0/stage/lib/boost_thread-mgw8-mt-x64-1_74.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../boost_1_74_0/stage/lib/boost_thread-mgw8-mt-d-x64-1_74.lib
}
}

#ANDROID
android {
INCLUDEPATH += $$PWD/../../boost_1_74_0/stage/include/boost-1_74
DEPENDPATH += $$PWD/../../boost_1_74_0/stage/include/boost-1_74

contains(QT_ARCH, armeabi-v7a) {
    LIBS += -L$$OUT_PWD/../OSCPACK/ -lOSCPACK_armeabi-v7a
    PRE_TARGETDEPS += $$OUT_PWD/../OSCPACK/libOSCPACK_armeabi-v7a.so
    LIBS += -L$$OUT_PWD/../DebugLibrary/ -lDebugLibrary_armeabi-v7a
    PRE_TARGETDEPS += $$OUT_PWD/../DebugLibrary/libDebugLibrary_armeabi-v7a.so
    LIBS += -L$$PWD/../../boost_1_74_0/stage/lib/ -lboost_thread-clang-mt-a32-1_74
    PRE_TARGETDEPS += $$PWD/../../boost_1_74_0/stage/lib/libboost_thread-clang-mt-a32-1_74.a
    LIBS += -L$$PWD/../../boost_1_74_0/stage/lib/ -lboost_chrono-clang-mt-a32-1_74
    PRE_TARGETDEPS += $$PWD/../../boost_1_74_0/stage/lib/libboost_chrono-clang-mt-a32-1_74.a
} else: contains(QT_ARCH, arm64-v8a) {
    LIBS += -L$$OUT_PWD/../OSCPACK/ -lOSCPACK_arm64-v8a
    PRE_TARGETDEPS += $$OUT_PWD/../OSCPACK/libOSCPACK_arm64-v8a.so
    LIBS += -L$$OUT_PWD/../DebugLibrary/ -lDebugLibrary_arm64-v8a
    PRE_TARGETDEPS += $$OUT_PWD/../DebugLibrary/libDebugLibrary_arm64-v8a.so
    LIBS += -L$$PWD/../../boost_1_74_0/stage/lib/ -lboost_thread-clang-mt-a64-1_74
    PRE_TARGETDEPS += $$PWD/../../boost_1_74_0/stage/lib/libboost_thread-clang-mt-a64-1_74.a
    LIBS += -L$$PWD/../../boost_1_74_0/stage/lib/ -lboost_chrono-clang-mt-a64-1_74
    PRE_TARGETDEPS += $$PWD/../../boost_1_74_0/stage/lib/libboost_chrono-clang-mt-a64-1_74.a
} else: contains(QT_ARCH, x86) {
    LIBS += -L$$OUT_PWD/../OSCPACK/ -lOSCPACK_x86
    PRE_TARGETDEPS += $$OUT_PWD/../OSCPACK/libOSCPACK_x86.so
    LIBS += -L$$OUT_PWD/../DebugLibrary/ -lDebugLibrary_x86
    PRE_TARGETDEPS += $$OUT_PWD/../DebugLibrary/libDebugLibrary_x86.so
    LIBS += -L$$PWD/../../boost_1_74_0/stage/lib/ -lboost_thread-clang-mt-x32-1_74
    PRE_TARGETDEPS += $$PWD/../../boost_1_74_0/stage/lib/libboost_thread-clang-mt-x32-1_74.a
    LIBS += -L$$PWD/../../boost_1_74_0/stage/lib/ -lboost_chrono-clang-mt-x32-1_74
    PRE_TARGETDEPS += $$PWD/../../boost_1_74_0/stage/lib/libboost_chrono-clang-mt-x32-1_74.a
} else: contains(QT_ARCH, x86_64) {
    LIBS += -L$$OUT_PWD/../OSCPACK/ -lOSCPACK_x86_64
    PRE_TARGETDEPS += $$OUT_PWD/../OSCPACK/libOSCPACK_x86_64.so
    LIBS += -L$$OUT_PWD/../DebugLibrary/ -lDebugLibrary_x86_64
    PRE_TARGETDEPS += $$OUT_PWD/../DebugLibrary/libDebugLibrary_x86_64.so
    LIBS += -L$$PWD/../../boost_1_74_0/stage/lib/ -lboost_thread-clang-mt-x64-1_74
    PRE_TARGETDEPS += $$PWD/../../boost_1_74_0/stage/lib/libboost_thread-clang-mt-x64-1_74.a
    LIBS += -L$$PWD/../../boost_1_74_0/stage/lib/ -lboost_chrono-clang-mt-x64-1_74
    PRE_TARGETDEPS += $$PWD/../../boost_1_74_0/stage/lib/libboost_chrono-clang-mt-x64-1_74.a
}

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

# LINUX
unix:!android {
    INCLUDEPATH += $$PWD/../../boost_1_74_0/
    DEPENDPATH += $$PWD/../../boost_1_74_0/

    LIBS += -L$$OUT_PWD/../OSCPACK/ -lOSCPACK
    PRE_TARGETDEPS += $$OUT_PWD/../OSCPACK/libOSCPACK.so
    LIBS += -L$$OUT_PWD/../DebugLibrary/ -lDebugLibrary
    PRE_TARGETDEPS += $$OUT_PWD/../DebugLibrary/libDebugLibrary.so
    LIBS += -L$$PWD/../../boost_1_74_0/stage/lib/ -lboost_thread
    PRE_TARGETDEPS += $$PWD/../../boost_1_74_0/stage/lib/libboost_thread.so
    LIBS += -L$$PWD/../../boost_1_74_0/stage/lib/ -lboost_chrono
    PRE_TARGETDEPS += $$PWD/../../boost_1_74_0/stage/lib/libboost_chrono.so

    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
