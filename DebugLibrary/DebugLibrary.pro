TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++11

HEADERS += \
    DebugLibrary.h

#Boost libraries
INCLUDEPATH += $$PWD/../../../Boost/boost_1_75_0
DEPENDPATH += $$PWD/../../../Boost/boost_1_75_0
