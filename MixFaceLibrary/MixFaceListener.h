#ifndef MIXFACELISTENER_H
#define MIXFACELISTENER_H

#include <iostream>
#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"
#include "osc/OscPacketListener.h"
#include <boost/signals2.hpp>

#define ANY_PORT -1
#define OUTPUT_BUFFER_SIZE 1024
#define PORT 10023

using namespace std;

typedef boost::signals2::signal<void(string*)> signal_xinfo;
typedef boost::signals2::signal<void(string*)> signal_status;
typedef boost::signals2::signal<void(string)> signal_str;
typedef boost::signals2::signal<void(string, int)> signal_str_int;
typedef boost::signals2::signal<void(string, float)> signal_str_float;
typedef boost::signals2::signal<void(string, bool)> signal_str_bool;
typedef boost::signals2::signal<void(string, string)> signal_str_str;

typedef signal_xinfo::slot_type  signal_type_xi;
typedef signal_status::slot_type  signal_type_st;
typedef signal_str::slot_type  signal_type_str;
typedef signal_str_int::slot_type  signal_type_str_int;
typedef signal_str_float::slot_type  signal_type_str_float;
typedef signal_str_bool::slot_type  signal_type_str_bool;
typedef signal_str_str::slot_type  signal_type_str_str;

typedef boost::signals2::signal<void(string, int)> signal_debug;
typedef signal_debug::slot_type signal_type_debug;

using namespace osc;

class MixFaceListener : public OscPacketListener {
public:
    signal_xinfo s_xi;
    signal_status s_st;
    signal_str s_str;
    signal_str_int s_str_int;
    signal_str_float s_str_float;
    signal_str_bool s_str_bool;
    signal_str_str s_str_str;

    signal_debug s_debug;

private:
    string xinfo[4];
    string status[3];

protected:
    virtual void ProcessMessage(const ReceivedMessage& m, const IpEndpointName& remoteEndpoint) {
        (void) remoteEndpoint;
        try{
            if (strcmp(m.AddressPattern(), "/xinfo") == 0) {
                ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
                xinfo[0] = (arg++)->AsString();
                xinfo[1] = (arg++)->AsString();
                xinfo[2] = (arg++)->AsString();
                xinfo[3] = (arg++)->AsString();
                if(arg != m.ArgumentsEnd()) throw ExcessArgumentException();
                s_xi(xinfo);
            } else if (strcmp(m.AddressPattern(), "/status") == 0) {
                ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
                status[0] = (arg++)->AsString();
                status[1] = (arg++)->AsString();
                status[2] = (arg++)->AsString();
                if (arg != m.ArgumentsEnd()) throw ExcessArgumentException();
                s_st(status);
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
                    s_str("%A" + address
                                    + "%B" + value);
                } else if ((arg)->IsInt32()) {
                    s_str("%A" + address
                                    + "%I" + to_string((arg)->AsInt32()));
                } else if ((arg)->IsFloat()) {
                    s_str("%A" + address
                                    + "%F" + to_string((arg)->AsFloat()));
                } else if ((arg)->IsString()) {
                    s_str("%A" + address
                                    + "%S" + string((arg)->AsString()));
                } else
                    throw WrongArgumentTypeException();
            }
        }
        catch (Exception& e) {
            s_debug("Error while parsing message:" +
                          string(m.AddressPattern()) + ":" +
                          string(e.what()),0);
        }
    }
};

#endif // MIXFACELISTENER_H
