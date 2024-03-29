#include "MixFaceLinker.h"

MixFaceLinker::MixFaceLinker(DebugLibrary *debug_)
    : linkerThread(new boost::thread{}), debug(debug_)
{
    linkerThread->join();
    debug->sendMessage("MixFaceLinker::MixFaceLinker",5);
    listener = new MixFaceListener(debug);
    listener->s_xi.connect(signal_type_xi(&MixFaceLinker::processXinfo, this, boost::arg<1>()));
    reciever = new SocketReceiveMultiplexer;
    udpSocket = new UdpSocket;
    udpSocket->Bind(IpEndpointName(IpEndpointName::ANY_ADDRESS, ANY_PORT));
    reciever->AttachSocketListener(udpSocket, listener);
    recieverThread = new boost::thread{&SocketReceiveMultiplexer::Run, reciever};
}

bool MixFaceLinker::connectTo(string hostNameStr_) {
    if (!connected) {
        hostNameStr = hostNameStr_;
        sendCmdOnly("/xinfo");
        sendCmdOnly("/status");
        boost::this_thread::sleep_for(boost::chrono::milliseconds{300});
        if (hostNameStr_ == xinfo[0] && (atof(xinfo[3].c_str()) >= 4)) {
            connected = true;
            //sendInt("/-stat/lock",1);
        } else {
            connected = false;
            hostNameStr = "127.0.0.1";
        }
    } else {
        connected = false;
        sendCmdOnly("/unsubscribe");
        //sendInt("/-stat/lock",0);
        //sendCmdOnly("/-stat/screen/screen");
        hostNameStr = "127.0.0.1";
    }
    return connected;
}

void MixFaceLinker::sendFloat(const char *oscAddress,float value) {
    IpEndpointName host(hostNameStr.c_str(),PORT);
    char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);
    p.Clear();
    p << osc::BeginMessage(oscAddress) << (float)value << End;
    udpSocket->SendTo(host, p.Data(), p.Size());
}

void MixFaceLinker::sendInt(const char *oscAddress,int value) {
    IpEndpointName host(hostNameStr.c_str(),PORT);
    char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);
    p.Clear();
    p << osc::BeginMessage(oscAddress) << (int)value << End;
    udpSocket->SendTo(host, p.Data(), p.Size());
}

void MixFaceLinker::sendString(const char *oscAddress, string value_) {
    const char *value = value_.c_str();
    IpEndpointName host(hostNameStr.c_str(),PORT);
    char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);
    p.Clear();
    p << osc::BeginMessage(oscAddress) << value << End;
    udpSocket->SendTo(host, p.Data(), p.Size());
}

void MixFaceLinker::sendCmdOnly(const char *oscAddress) {
    IpEndpointName host(hostNameStr.c_str(),PORT);
    char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);
    p.Clear();
    p << osc::BeginMessage(oscAddress) << End;
    udpSocket->SendTo(host, p.Data(), p.Size());
}

void MixFaceLinker::sendDynamicMsg(osc::OutboundPacketStream p) {
    IpEndpointName host(hostNameStr.c_str(),PORT);
    p << End;
    udpSocket->SendTo(host, p.Data(), p.Size());
}
