TEMPLATE = lib
CONFIG += staticlib

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

SOURCES += \
ip/IpEndpointName.cpp \
osc/OscOutboundPacketStream.cpp \
osc/OscPrintReceivedElements.cpp \
osc/OscReceivedElements.cpp \
osc/OscTypes.cpp

win32{
    SOURCES += ip/win32/NetworkingUtils.cpp
    SOURCES += ip/win32/UdpSocket.cpp
}else{
    SOURCES += ip/posix/NetworkingUtils.cpp
    SOURCES += ip/posix/UdpSocket.cpp
}

HEADERS += \
ip/IpEndpointName.h \
ip/NetworkingUtils.h \
ip/PacketListener.h \
ip/TimerListener.h \
ip/UdpSocket.h \
osc/MessageMappingOscPacketListener.h \
osc/OscException.h \
osc/OscHostEndianness.h \
osc/OscOutboundPacketStream.h \
osc/OscPacketListener.h \
osc/OscPrintReceivedElements.h \
osc/OscReceivedElements.h \
osc/OscTypes.h

win32{
LIBS += -lkernel32 -lws2_32 -lwinmm
}

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
