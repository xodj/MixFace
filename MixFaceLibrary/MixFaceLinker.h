#ifndef MIXFACELINKER_H
#define MIXFACELINKER_H

#include <QThread>
#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"
#include "osc/OscPacketListener.h"
#include "MixFaceListener.h"
#include <boost/thread.hpp>
#include <boost/signals2.hpp>

class MixFaceListener;

class MixFaceLinker : public QThread
{
    Q_OBJECT
public:
    explicit MixFaceLinker();
    bool connectTo(string hostNameStr_);
    void sendFloat(const char *oscAddress, float value);
    void sendInt(const char *oscAddress, int value);
    void sendString(const char *oscAddress, string value);
    void sendCmdOnly(const char *oscAddress);
    void sendDynamicMsg(osc::OutboundPacketStream p);

    MixFaceListener *listener;

    string xinfo[4];

signals:
    void newMeters2(float *floatArray);

public slots:
    void processMessages(){ reciever->Run(); }

private:
    void processXinfo(string *xinfo_) { for(int i = 0;i < 4; i++) xinfo[i] = xinfo_[i]; }

    UdpSocket *udpSocket;
    SocketReceiveMultiplexer *reciever;

    bool connected = false;
    string hostNameStr = "127.0.0.1";

};

#endif // MIXFACELINKER_H
