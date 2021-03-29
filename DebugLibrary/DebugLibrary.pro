CONFIG -= qt
CONFIG -= gui

TEMPLATE = lib
#Comment for dynamic library
#CONFIG += staticlib

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

TARGET = DebugLibrary
RC_ICONS = ../MixFaceMedia/mf_icon.ico
#win32:VERSION = 1.2.0.80
#else:VERSION = 1.2.0
QMAKE_TARGET_COMPANY = "xo.dj"
QMAKE_TARGET_DESCRIPTION = "Debug messages recorder."
QMAKE_TARGET_COPYRIGHT = "GPL-3.0"
QMAKE_TARGET_PRODUCT = "Debug Library"
QMAKE_TARGET_ORIGINAL_FILENAME = DebugLibrary.dll

HEADERS += \
    DebugLibrary.hpp

SOURCES += \
    DebugLibrary.cpp

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

message($$TARGET)
message($$VERSION)
message($$QMAKE_TARGET_DESCRIPTION)
