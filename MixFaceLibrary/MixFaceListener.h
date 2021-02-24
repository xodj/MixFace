#ifndef MIXFACELISTENER_H
#define MIXFACELISTENER_H

#include <iostream>
#include <QObject>
#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"
#include "osc/OscPacketListener.h"
#include "MixFaceLinker.h"

#define ANY_PORT -1
#define OUTPUT_BUFFER_SIZE 1024
#define PORT 10023

using namespace std;

class MixFaceListenerSignals : public QObject {
    Q_OBJECT
public:
    void newMessage(string message) {
        emit snewMessage(QString::fromStdString(message));
    }

    void newXinfo(string *xinfo) {
        QString qxinfo[4] = { QString::fromStdString(xinfo[0]), QString::fromStdString(xinfo[1]),
                              QString::fromStdString(xinfo[2]), QString::fromStdString(xinfo[3])};
        emit sprocessXinfo(qxinfo);
    }

    void newStatus(string *status) {
        QString qstatus[3] = { QString::fromStdString(status[0]), QString::fromStdString(status[1]),
                               QString::fromStdString(status[2])};
        emit snewStatus(qstatus);
    }

    void newMeters2(float *floatArray) {
        emit snewMeters2(floatArray);
    }

    void debug(string message,int debugLevel) {
        emit sdebug(QString::fromStdString(message), debugLevel);
    }

signals:
    void snewMessage(QString message);
    void sprocessXinfo(QString xinfo[]);
    void snewStatus(QString *status);
    void snewMeters2(float *floatArray);
    void sdebug(QString message, int debugLevel);

};

using namespace osc;

class MixFaceListener : public OscPacketListener {
public:
    MixFaceListener(MixFaceListenerSignals *mfl_signals_) :
          OscPacketListener(),
          mfl_signals(mfl_signals_) {}

private:
    MixFaceListenerSignals *mfl_signals;
    string xinfo[4];
    string status[3];

protected:
    virtual void ProcessMessage(const ReceivedMessage& m, const IpEndpointName& remoteEndpoint) {
        (void) remoteEndpoint;
        try{
            cout<<"MixFaceListener::ProcessMessage message"<<endl<<endl;
            if (strcmp(m.AddressPattern(), "/xinfo") == 0) {
                ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
                xinfo[0] = (arg++)->AsString();
                xinfo[1] = (arg++)->AsString();
                xinfo[2] = (arg++)->AsString();
                xinfo[3] = (arg++)->AsString();
                if(arg != m.ArgumentsEnd()) throw ExcessArgumentException();
                mfl_signals->newXinfo(xinfo);
            } else if (strcmp(m.AddressPattern(), "/status") == 0) {
                ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
                status[0] = (arg++)->AsString();
                status[1] = (arg++)->AsString();
                status[2] = (arg++)->AsString();
                if (arg != m.ArgumentsEnd()) throw ExcessArgumentException();
                mfl_signals->newStatus(status);
            } else if (strcmp(m.AddressPattern(), "meters/9") == 0) {
            } else if (strcmp(m.AddressPattern(), "meters/10") == 0) {
            } else if (strcmp(m.AddressPattern(), "meters/14") == 0) {
            } else if (strcmp(m.AddressPattern(), "meters/0") == 0) {
            } else if (strcmp(m.AddressPattern(), "meters/2") == 0) {
                /*const void *bData;
                osc::osc_bundle_element_size_t bSize;
                m.ArgumentsBegin()->AsBlob(bData, bSize);
                float const *mFloatArray = (float const *)bData;
                int aSize = int(bSize / 4);
                float floatArray[aSize];
                for (int i = 1; i < aSize; i++) { floatArray[i-1] = mFloatArray[i]; }
                emit newMeters2(floatArray);*/
            } else if (strcmp(m.AddressPattern(), "meters/5") == 0) {
            } else if (strcmp(m.AddressPattern(), "meters/6") == 0) {
            } else if (strcmp(m.AddressPattern(), "meters/16") == 0) {
            } else {
                string address = m.AddressPattern();
                ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
                if ((arg)->IsBool()) {
                    string value;
                    (arg)->AsBool() ? value = "1" : value = "0";
                    mfl_signals->newMessage("%A" + address
                                    + "%B" + value);
                } else if ((arg)->IsInt32()) {
                    mfl_signals->newMessage("%A" + address
                                    + "%I" + to_string((arg)->AsInt32()));
                } else if ((arg)->IsFloat()) {
                    mfl_signals->newMessage("%A" + address
                                    + "%F" + to_string((arg)->AsFloat()));
                } else if ((arg)->IsString()) {
                    mfl_signals->newMessage("%A" + address
                                    + "%S" + string((arg)->AsString()));
                } else
                    throw WrongArgumentTypeException();
            }
        }
        catch (Exception& e) {
            mfl_signals->debug("Error while parsing message:" +
                          string(m.AddressPattern()) + ":" +
                          string(e.what()),0);
        }
    }
};

#endif // MIXFACELISTENER_H
