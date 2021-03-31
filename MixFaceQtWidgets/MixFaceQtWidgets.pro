QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

TARGET = MixFace
RC_ICONS = ../MixFaceMedia/mf_icon.ico
#win32:VERSION = 2.0.3.55
#else:VERSION = 2.0.3
QMAKE_TARGET_COMPANY = "xo.dj"
QMAKE_TARGET_DESCRIPTION = "MixFace Qt Widgets GUI for MixFace Library."
QMAKE_TARGET_COPYRIGHT = "GPL-3.0 License."
QMAKE_TARGET_PRODUCT = "MixFace Qt Widgets GUI."
QMAKE_TARGET_ORIGINAL_FILENAME = OSCPACK.dll

RESOURCES += \
    MixFaceResources.qrc

HEADERS += \
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

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../MixFaceLibrary/release/ -lMixFaceLibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../MixFaceLibrary/debug/ -lMixFaceLibrary
else:unix: LIBS += -L$$OUT_PWD/../MixFaceLibrary/ -lMixFaceLibrary_armeabi-v7a

INCLUDEPATH += $$PWD/../MixFaceLibrary
DEPENDPATH += $$PWD/../MixFaceLibrary

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../MixFaceLibrary/release/libMixFaceLibrary.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../MixFaceLibrary/debug/libMixFaceLibrary.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../MixFaceLibrary/release/MixFaceLibrary.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../MixFaceLibrary/debug/MixFaceLibrary.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../MixFaceLibrary/libMixFaceLibrary_armeabi-v7a.so

win32{
    LIBS += -lkernel32 -lws2_32 -lwinmm
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../DebugLibrary/release/ -lDebugLibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../DebugLibrary/debug/ -lDebugLibrary
else:unix: LIBS += -L$$OUT_PWD/../DebugLibrary/ -lDebugLibrary_armeabi-v7a

INCLUDEPATH += $$PWD/../DebugLibrary
DEPENDPATH += $$PWD/../DebugLibrary

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../DebugLibrary/release/libDebugLibrary.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../DebugLibrary/debug/libDebugLibrary.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../DebugLibrary/release/DebugLibrary.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../DebugLibrary/debug/DebugLibrary.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../DebugLibrary/libDebugLibrary_armeabi-v7a.so

INCLUDEPATH += $$PWD/../OSCPACK
DEPENDPATH += $$PWD/../OSCPACK

#Boost libraries
win32:{
INCLUDEPATH += $$PWD/../../../../../Boost/boost_1_74_0
DEPENDPATH += $$PWD/../../../../../Boost/boost_1_74_0
}

#ANDROID
unix:!macx{
INCLUDEPATH += $$PWD/../../../Boost/boost_1_74_0/stage/include/boost-1_74
DEPENDPATH += $$PWD/../../../Boost/boost_1_74_0/stage/include/boost-1_74

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
