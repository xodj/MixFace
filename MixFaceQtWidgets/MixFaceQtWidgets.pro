QT       += core gui network

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
win32:{
INCLUDEPATH += $$PWD/../../../../../Boost/boost_1_74_0
DEPENDPATH += $$PWD/../../../../../Boost/boost_1_74_0

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../Boost/boost_1_74_0/stage/lib/ -lboost_thread-mgw8-mt-x32-1_74
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../Boost/boost_1_74_0/stage/lib/ -lboost_thread-mgw8-mt-d-x32-1_74

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../Boost/boost_1_74_0/stage/lib/libboost_thread-mgw8-mt-x32-1_74.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../Boost/boost_1_74_0/stage/lib/libboost_thread-mgw8-mt-d-x32-1_74.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../Boost/boost_1_74_0/stage/lib/boost_thread-mgw8-mt-x32-1_74.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../Boost/boost_1_74_0/stage/lib/boost_thread-mgw8-mt-d-x32-1_74.lib
}

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

