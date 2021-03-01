#ifndef MIXFACELINKER_H
#define MIXFACELINKER_H

#include <boost/thread.hpp>
#include <boost/signals2.hpp>
#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"
#include "osc/OscPacketListener.h"
#include "MixFaceListener.h"

class MixFaceListener;

class MixFaceLinker : public boost::thread::thread {
public:
    MixFaceLinker();
    bool connectTo(string hostNameStr_);
    void sendFloat(const char *oscAddress, float value);
    void sendInt(const char *oscAddress, int value);
    void sendString(const char *oscAddress, string value);
    void sendCmdOnly(const char *oscAddress);
    void sendDynamicMsg(osc::OutboundPacketStream p);
    void processMessages(){
        reciever->Run();
    }

    MixFaceListener *listener;

    string xinfo[4];

private:
    void processXinfo(string *xinfo_) { for(int i = 0;i < 4; i++) xinfo[i] = xinfo_[i]; }

    UdpSocket *udpSocket;
    SocketReceiveMultiplexer *reciever;
    boost::thread *recieverThread;
    bool connected = false;
    string hostNameStr = "127.0.0.1";

};

#endif // MIXFACELINKER_H
