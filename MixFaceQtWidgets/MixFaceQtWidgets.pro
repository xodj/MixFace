QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

TARGET = MixFace
win32: {
#RC_ICONS = ../MixFaceMedia/mf_icon.ico
#VERSION = 0.2.0.65
#TARGET_EXT = .exe
QMAKE_TARGET_COMPANY = "xo.dj"
QMAKE_TARGET_DESCRIPTION = "MixFace Qt Widgets GUI for MixFace Library."
QMAKE_TARGET_COPYRIGHT = "GPL-3.0 License."
QMAKE_TARGET_PRODUCT = "MixFace Qt Widgets GUI."
QMAKE_TARGET_ORIGINAL_FILENAME = MixFace.exe
RC_FILE += MixFace.rc
} else: VERSION = 0.2.0

RESOURCES += \
    MixFaceResources.qrc

HEADERS += \
    MixFace.rc \
    MixFaceDynamicsWidget.h \
    MixFaceEnumTypes.h \
    MixFaceEqualizerWidget.h \
    MixFaceFaderWidget.h \
    MixFaceFonts.h \
    MixFaceIconPicker.h \
    MixFaceRightWidget.h \
    MixFaceSourceWidget.h \
    MixFaceStaticMath.h \
    MixFaceTopWidget.h \
    MixFaceVolumeMeter.h \
    MixFaceWindow.h

SOURCES += \
    MixFaceDynamicsWidget.cpp \
    MixFaceEqualizerWidget.cpp \
    MixFaceFaderWidget.cpp \
    MixFaceIconPicker.cpp \
    MixFaceRightWidget.cpp \
    MixFaceSourceWidget.cpp \
    MixFaceTopWidget.cpp \
    MixFaceVolumeMeter.cpp \
    MixFaceWindow.cpp


INCLUDEPATH += $$PWD/../MixFaceLibrary
DEPENDPATH += $$PWD/../MixFaceLibrary
INCLUDEPATH += $$PWD/../DebugLibrary
DEPENDPATH += $$PWD/../DebugLibrary
INCLUDEPATH += $$PWD/../OSCPACK
DEPENDPATH += $$PWD/../OSCPACK

# WINDOWS
win32:{
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../MixFaceLibrary/release/ -lMixFaceLibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../MixFaceLibrary/debug/ -lMixFaceLibrary
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../MixFaceLibrary/release/libMixFaceLibrary.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../MixFaceLibrary/debug/libMixFaceLibrary.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../MixFaceLibrary/release/MixFaceLibrary.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../MixFaceLibrary/debug/MixFaceLibrary.lib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../DebugLibrary/release/ -lDebugLibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../DebugLibrary/debug/ -lDebugLibrary
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../DebugLibrary/release/libDebugLibrary.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../DebugLibrary/debug/libDebugLibrary.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../DebugLibrary/release/DebugLibrary.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../DebugLibrary/debug/DebugLibrary.lib

LIBS += -lkernel32 -lws2_32 -lwinmm

INCLUDEPATH += $$PWD/../../../../../Boost/boost_1_74_0
DEPENDPATH += $$PWD/../../../../../Boost/boost_1_74_0
}

# ANDROID
android {
INCLUDEPATH += $$PWD/../../../Boost/boost_1_74_0/stage/include/boost-1_74
DEPENDPATH += $$PWD/../../../Boost/boost_1_74_0/stage/include/boost-1_74

contains(QT_ARCH, armeabi-v7a) {
    LIBS += -L$$OUT_PWD/../MixFaceLibrary/ -lMixFaceLibrary_armeabi-v7a
    PRE_TARGETDEPS += $$OUT_PWD/../MixFaceLibrary/libMixFaceLibrary_armeabi-v7a.so
    PRE_TARGETDEPS += $$OUT_PWD/../DebugLibrary/libDebugLibrary_armeabi-v7a.so
    LIBS += -L$$OUT_PWD/../DebugLibrary/ -lDebugLibrary_armeabi-v7a
} else: contains(QT_ARCH, arm64-v8a) {
    LIBS += -L$$OUT_PWD/../MixFaceLibrary/ -lMixFaceLibrary_arm64-v8a
    PRE_TARGETDEPS += $$OUT_PWD/../MixFaceLibrary/libMixFaceLibrary_arm64-v8a.so
    PRE_TARGETDEPS += $$OUT_PWD/../DebugLibrary/libDebugLibrary_arm64-v8a.so
    LIBS += -L$$OUT_PWD/../DebugLibrary/ -lDebugLibrary_arm64-v8a
} else: contains(QT_ARCH, x86) {
    LIBS += -L$$OUT_PWD/../MixFaceLibrary/ -lMixFaceLibrary_x86
    PRE_TARGETDEPS += $$OUT_PWD/../MixFaceLibrary/libMixFaceLibrary_x86.so
    PRE_TARGETDEPS += $$OUT_PWD/../DebugLibrary/libDebugLibrary_x86.so
    LIBS += -L$$OUT_PWD/../DebugLibrary/ -lDebugLibrary_x86
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
    INCLUDEPATH += /home/xodj/Boost/boost_1_74_0
    DEPENDPATH += /home/xodj/Boost/boost_1_74_0

    LIBS += -L$$OUT_PWD/../MixFaceLibrary/ -lMixFaceLibrary
    PRE_TARGETDEPS += $$OUT_PWD/../MixFaceLibrary/libMixFaceLibrary.so
    LIBS += -L$$OUT_PWD/../DebugLibrary/ -lDebugLibrary
    PRE_TARGETDEPS += $$OUT_PWD/../DebugLibrary/libDebugLibrary.so

    LIBS += -L$$OUT_PWD/../OSCPACK/ -lOSCPACK
    PRE_TARGETDEPS += $$OUT_PWD/../OSCPACK/libOSCPACK.so
    LIBS += -L/home/xodj/Boost/boost_1_74_0/stage/lib/ -lboost_thread
    PRE_TARGETDEPS += /home/xodj/Boost/boost_1_74_0/stage/lib/libboost_thread.so
    LIBS += -L/home/xodj/Boost/boost_1_74_0/stage/lib/ -lboost_chrono
    PRE_TARGETDEPS += /home/xodj/Boost/boost_1_74_0/stage/lib/libboost_chrono.so

    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
