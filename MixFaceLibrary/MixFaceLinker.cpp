#include "MixFaceLinker.h"
#include <string.h>

MixFaceLinker::MixFaceLinker() : QThread()
{
    listener = new MixFaceListener;
    listener->s_xi.connect(signal_type_xi(&MixFaceLinker::processXinfo, this, boost::arg<1>()));
    //listener->s_st.connect(signal_type_st(&MixFaceLinker::processStatus, this, boost::arg<1>()));
    //listener->s_str.connect(signal_type_str(&MixFaceLinker::newMessage, this, boost::arg<1>()));
    //listener->s_debug.connect(signal_type_debug(&MixFaceLinker::debug, this, boost::arg<2>()));

    /*connect(mfl_signals, &MixFaceListenerSignals::snewMessage, this, &MixFaceLinker::newMessage);
    connect(mfl_signals, &MixFaceListenerSignals::sprocessXinfo, this, &MixFaceLinker::processXinfo);
    connect(mfl_signals, &MixFaceListenerSignals::snewMeters2, this, &MixFaceLinker::newMeters2);
    connect(mfl_signals, &MixFaceListenerSignals::sdebug, this, &MixFaceLinker::debug);*/
    reciever = new SocketReceiveMultiplexer;
    udpSocket = new UdpSocket;
    udpSocket->Bind(IpEndpointName(IpEndpointName::ANY_ADDRESS, ANY_PORT));
    reciever->AttachSocketListener(udpSocket,listener);
}

bool MixFaceLinker::connectTo(QString hostNameString) {
    if (!connected) {
        hostNameStr = hostNameString.toStdString();
        sendCmdOnly("/xinfo");
        sendCmdOnly("/status");
        QThread::msleep(100);
        if (hostNameString/*.toStdString()*/==xinfo[0]/*&&(atof(xinfo[3].c_str())>=4)*/) {
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
    p << osc::BeginMessage(oscAddress) << (float)value << osc::EndMessage;
    udpSocket->SendTo(host, p.Data(), p.Size());
}

void MixFaceLinker::sendInt(const char *oscAddress,int value) {
    IpEndpointName host(hostNameStr.c_str(),PORT);
    char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);
    p.Clear();
    p << osc::BeginMessage(oscAddress) << (int)value << osc::EndMessage;
    udpSocket->SendTo(host, p.Data(), p.Size());
}

void MixFaceLinker::sendString(const char *oscAddress,QString value_) {
    QByteArray valueArray = value_.toLatin1();
    const char *value = valueArray;
    IpEndpointName host(hostNameStr.c_str(),PORT);
    char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);
    p.Clear();
    p << osc::BeginMessage(oscAddress) << value << osc::EndMessage;
    udpSocket->SendTo(host, p.Data(), p.Size());
}

void MixFaceLinker::sendCmdOnly(const char *oscAddress) {
    IpEndpointName host(hostNameStr.c_str(),PORT);
    char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);
    p.Clear();
    p << osc::BeginMessage(oscAddress) << osc::EndMessage;
    udpSocket->SendTo(host, p.Data(), p.Size());
}

void MixFaceLinker::sendDynamicMsg(osc::OutboundPacketStream p) {
    IpEndpointName host(hostNameStr.c_str(),PORT);
    udpSocket->SendTo(host, p.Data(), p.Size());
}
