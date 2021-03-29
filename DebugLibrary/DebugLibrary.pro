CONFIG -= qt

TEMPLATE = lib
#Coment for dynamic library
CONFIG += staticlib

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

HEADERS += \
    DebugLibrary.hpp

SOURCES += \
    DebugLibrary.cpp

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
