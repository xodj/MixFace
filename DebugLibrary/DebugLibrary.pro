CONFIG -= qt
CONFIG -= gui

TEMPLATE = lib
#Comment for dynamic library
#CONFIG += staticlib

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

TARGET = DebugLibrary
win32: {
#VERSION = 1.2.0.81
#TARGET_EXT = .dll
QMAKE_TARGET_COMPANY = "xo.dj"
QMAKE_TARGET_DESCRIPTION = "Debug messages recorder."
QMAKE_TARGET_COPYRIGHT = "GPL-3.0 License."
QMAKE_TARGET_PRODUCT = "Debug Library"
QMAKE_TARGET_ORIGINAL_FILENAME = DebugLibrary.dll
RC_FILE += DebugLibrary.rc
} else: VERSION = 1.2.0

HEADERS += \
    DebugLibrary.hpp \
    DebugLibrary.rc

SOURCES += \
    DebugLibrary.cpp

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
