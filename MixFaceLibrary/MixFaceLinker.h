#ifndef MIXFACELINKER_H
#define MIXFACELINKER_H

#include <QString>
#include <QThread>
#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"
#include "osc/OscPacketListener.h"
#include "MixFaceListener.h"

#define ANY_PORT -1
#define OUTPUT_BUFFER_SIZE 1024
#define PORT 10023

class MixFaceListener;

class MixFaceLinker : public QThread
{
    Q_OBJECT
public:
    explicit MixFaceLinker();

    QString xinfo[4];

signals:
    void newMessage(QString message);
    void newMeters2(float *floatArray);
    void debug(QString message,int debugLevel);

public slots:
    void processMessages();

    bool connectTo(QString hostNameString);

    void sendFloat(const char *oscAddress,float value);
    void sendInt(const char *oscAddress,int value);
    void sendString(const char *oscAddress, QString value);
    void sendCmdOnly(const char *oscAddress);
    void sendDynamicMsg(osc::OutboundPacketStream p);

private:
    void processXinfo(QString xinfo_[]);
    bool connected = false;
    UdpSocket *udpSocket;
    SocketReceiveMultiplexer *reciever;
    MixFaceListener *listener;
    QByteArray hostNameArray;

};

#endif // MIXFACELINKER_H
