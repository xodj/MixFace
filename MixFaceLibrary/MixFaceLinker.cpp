#include "MixFaceLinker.h"

MixFaceLinker::MixFaceLinker(QObject *parent) : QObject(parent)
{
    listener = new packetListener;
    connect(listener, &packetListener::newMessage, this, &MixFaceLinker::newMessage);
    connect(listener, &packetListener::newXinfo, this, &MixFaceLinker::processXinfo);
    connect(listener, &packetListener::newMeters2, this, &MixFaceLinker::newMeters2);
    connect(listener, &packetListener::debug, this, &MixFaceLinker::debug);
    reciever = new SocketReceiveMultiplexer;
    udpSocket = new UdpSocket;
    udpSocket->Bind(IpEndpointName(IpEndpointName::ANY_ADDRESS, ANY_PORT));
    reciever->AttachSocketListener(udpSocket,listener);

    QString hostNameString = ("127.0.0.1");
    hostNameArray = hostNameString.toLatin1();
}

void MixFaceLinker::processMessages(){reciever->Run();}

void MixFaceLinker::processXinfo(QString xinfo0, QString xinfo1, QString xinfo2, QString xinfo3){
    xinfo[0] = xinfo0;xinfo[1] = xinfo1;xinfo[2] = xinfo2;xinfo[3] = xinfo3;
}

bool MixFaceLinker::connectTo(QString hostNameString) {
    if (!connected) {
        hostNameArray = hostNameString.toLatin1();
        sendCmdOnly("/xinfo");
        sendCmdOnly("/status");
        QThread::msleep(100);
        if (hostNameString==xinfo[0]&&(xinfo[3].toFloat()>=4)) {
            connected = true;
            //sendInt("/-stat/lock",1);
        } else {
            connected = false;
            QString hostNameString = ("127.0.0.1");
            hostNameArray = hostNameString.toLatin1();
        }
    } else {
        connected = false;
        sendCmdOnly("/unsubscribe");
        //sendInt("/-stat/lock",0);
        //sendCmdOnly("/-stat/screen/screen");
        QString hostNameString = ("127.0.0.1");
        hostNameArray = hostNameString.toLatin1();
    }
    return connected;
}

void MixFaceLinker::sendFloat(const char *oscAddress,float value)
{
    const char *hostName = hostNameArray;
    IpEndpointName host(hostName,PORT);
    char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);
    p.Clear();
    p << osc::BeginMessage(oscAddress) << (float)value << osc::EndMessage;
    udpSocket->SendTo(host, p.Data(), p.Size());
}

void MixFaceLinker::sendInt(const char *oscAddress,int value)
{
    const char *hostName = hostNameArray;
    IpEndpointName host(hostName,PORT);
    char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);
    p.Clear();
    p << osc::BeginMessage(oscAddress) << (int)value << osc::EndMessage;
    udpSocket->SendTo(host, p.Data(), p.Size());
}

void MixFaceLinker::sendString(const char *oscAddress,QString value_){
        const char *hostName = hostNameArray;
        QByteArray valueArray = value_.toLatin1();
        const char *value = valueArray;
        IpEndpointName host(hostName,PORT);
        char buffer[OUTPUT_BUFFER_SIZE];
        osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);
        p.Clear();
        p << osc::BeginMessage(oscAddress) << value << osc::EndMessage;
        udpSocket->SendTo(host, p.Data(), p.Size());
    }

void MixFaceLinker::sendCmdOnly(const char *oscAddress){
    const char *hostName = hostNameArray;
    IpEndpointName host(hostName,PORT);
    char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);
    p.Clear();
    p << osc::BeginMessage(oscAddress) << osc::EndMessage;
    udpSocket->SendTo(host, p.Data(), p.Size());
}

void MixFaceLinker::sendDynamicMsg(osc::OutboundPacketStream p){
    const char *hostName = hostNameArray;
    IpEndpointName host(hostName,PORT);
    udpSocket->SendTo(host, p.Data(), p.Size());
}
