#ifndef MIXFACELINKER_H
#define MIXFACELINKER_H

#include <QObject>
#include <QString>
#include <QTimer>
#include <QThread>
#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"
#include "osc/OscPacketListener.h"
#include "MixFaceLibrary.h"

#define ANY_PORT -1
#define OUTPUT_BUFFER_SIZE 1024
#define PORT 10023

class packetListener : public osc::OscPacketListener {
    Q_OBJECT

public:
signals:
    void newMessage(QString message);
    void newXinfo(QString xip,QString xname,QString xmodel,QString xversion);
    void newStatus(QString status0,QString status1,QString status2);
    void newMeters2(float *floatArray);
    void debug(QString message,int debugLevel);

private:
    QString xinfo[4];
    QString status[3];

protected:
    virtual void ProcessMessage( const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint ) {
        (void) remoteEndpoint;

        try{
            if (std::strcmp(m.AddressPattern(), "/xinfo") == 0) {
                osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
                xinfo[0] = QString::fromUtf8((arg++)->AsString());
                xinfo[1] = QString::fromUtf8((arg++)->AsString());
                xinfo[2] = QString::fromUtf8((arg++)->AsString());
                xinfo[3] = QString::fromUtf8((arg++)->AsString());
                if(arg != m.ArgumentsEnd()) throw osc::ExcessArgumentException();
                emit newXinfo(xinfo[0],xinfo[1],xinfo[2],xinfo[3]);
            } else if (std::strcmp(m.AddressPattern(), "/status") == 0) {
                osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
                status[0] = QString::fromUtf8((arg++)->AsString());
                status[1] = QString::fromUtf8((arg++)->AsString());
                status[2] = QString::fromUtf8((arg++)->AsString());
                if (arg != m.ArgumentsEnd()) throw osc::ExcessArgumentException();
                newStatus(status[0],status[1],status[2]);
            } else if (std::strcmp(m.AddressPattern(), "meters/9") == 0) {
            } else if (std::strcmp(m.AddressPattern(), "meters/10") == 0) {
            } else if (std::strcmp(m.AddressPattern(), "meters/14") == 0) {
            } else if (std::strcmp(m.AddressPattern(), "meters/0") == 0) {
            } else if (std::strcmp(m.AddressPattern(), "meters/2") == 0) {
                /*const void *bData;
                osc::osc_bundle_element_size_t bSize;
                m.ArgumentsBegin()->AsBlob(bData, bSize);
                float const *mFloatArray = (float const *)bData;
                int aSize = int(bSize / 4);
                float floatArray[aSize];
                for (int i = 1; i < aSize; i++) { floatArray[i-1] = mFloatArray[i]; }
                emit newMeters2(floatArray);*/
            } else if (std::strcmp(m.AddressPattern(), "meters/5") == 0) {
            } else if (std::strcmp(m.AddressPattern(), "meters/6") == 0) {
            } else if (std::strcmp(m.AddressPattern(), "meters/16") == 0) {
            } else {
                const char *address = m.AddressPattern();
                osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
                if ((arg)->IsBool()) {
                    bool boolV = (arg)->AsBool();
                    emit newMessage("%A" + QString::fromUtf8(address)
                                    + "%B" + QString::number(boolV));
                } else if ((arg)->IsInt32()) {
                    osc::int32 intV;
                    intV = (arg)->AsInt32();
                    emit newMessage("%A" + QString::fromUtf8(address)
                                    + "%I" + QString::number(intV));
                } else if ((arg)->IsFloat()) {
                    float floatV = (arg)->AsFloat();
                    emit newMessage("%A" + QString::fromUtf8(address)
                                    + "%F" + QString::number(floatV));
                } else if ((arg)->IsString()) {
                    const char *stringV = (arg)->AsString();
                    emit newMessage("%A" + QString::fromUtf8(address)
                                    + "%S" + QString::fromUtf8(stringV));
                } else throw osc::WrongArgumentTypeException();
            }
        }
        catch (osc::Exception& e) {
            emit debug("Error while parsing message:" + QString(m.AddressPattern()) + ":" + QString(e.what()),0);
        }
    }
};

class MixFaceLinker : public QObject
{
    Q_OBJECT
public:
    explicit MixFaceLinker(QObject *parent = nullptr);

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
    void processXinfo(QString xip,QString xname,QString xmodel,QString xversion);
    bool connected = false;
    UdpSocket *udpSocket;
    SocketReceiveMultiplexer *reciever;
    packetListener *listener;
    QByteArray hostNameArray;

};

#endif // MIXFACELINKER_H
