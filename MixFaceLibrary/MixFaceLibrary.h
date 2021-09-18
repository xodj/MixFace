#ifndef MIXFACELIBRARY_H
#define MIXFACELIBRARY_H

#include <iostream>
#include <string>
#include <boost/thread.hpp>
#include <boost/signals2.hpp>
#include "Timer.hpp"
#include "MixFaceLinker.h"
#include "MixFaceKeeper.h"
#include "DebugLibrary.hpp"

typedef boost::signals2::signal<void(int, int, int)> signal_thr_int;
typedef signal_thr_int::slot_type signal_type_thr_int;
typedef boost::signals2::signal<void(bool)> signal_bool;
typedef signal_bool::slot_type signal_type_bool;

class MixFaceLibrary {
public:
    MixFaceLibrary(DebugLibrary *debug_ = nullptr);
    void sendFloat(const char *oscAddress, float value){
        linker->sendFloat(oscAddress, value);
    }
    void sendInt(const char *oscAddress, int value){
        linker->sendInt(oscAddress, value);
    }
    void sendString(const char *oscAddress, std::string value){
        linker->sendString(oscAddress, value);
    }
    void sendRequest(const char *oscAddress){
        linker->sendCmdOnly(oscAddress);
    }
    void keeperSaveEvent(){
        keeper->saveX32DB();
    }
    //Struct
    x32db *db;
    std::string xinfo[4];
    //Slots
    signal_thr_int valueChanged;
    signal_bool slotConnected;

    signal_float_array newMeters1;
    signal_float_array newMeters2;
    signal_float_array newMeters3;
    //Threads
    void threadConnect(std::string hostNameString){
        boost::thread{&MixFaceLibrary::connect, this, hostNameString};
    }
    void threadSendSyncMessages(){
        boost::thread{&MixFaceLibrary::sendSyncMessages, this};
    }
    void threadSendDynRequestsMessages(int idx){
        boost::thread{&MixFaceLibrary::sendDynRequestsMessages, this, idx};
    }

private:
    void connect(std::string hostNameString);
    void sendSyncMessages();
    void sendXremoteMessage();
    void sendDynRequestsMessages(int idx){
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        std::string msg = getOscAddress(dynon, chtype, chN, 0);
        sendRequest(msg.c_str());
        msg = getOscAddress(dynthr, chtype, chN, 0);
        sendRequest(msg.c_str());
        msg = getOscAddress(dynratio, chtype, chN, 0);
        sendRequest(msg.c_str());
        msg = getOscAddress(dynmix, chtype, chN, 0);
        sendRequest(msg.c_str());
        msg = getOscAddress(dynmgain, chtype, chN, 0);
        sendRequest(msg.c_str());
        msg = getOscAddress(dynattack, chtype, chN, 0);
        sendRequest(msg.c_str());
        msg = getOscAddress(dynhold, chtype, chN, 0);
        sendRequest(msg.c_str());
        msg = getOscAddress(dynrelease, chtype, chN, 0);
        sendRequest(msg.c_str());
        msg = getOscAddress(dynmode, chtype, chN, 0);
        sendRequest(msg.c_str());
        msg = getOscAddress(dynknee, chtype, chN, 0);
        sendRequest(msg.c_str());
        msg = getOscAddress(dynenv, chtype, chN, 0);
        sendRequest(msg.c_str());
        msg = getOscAddress(dyndet, chtype, chN, 0);
        sendRequest(msg.c_str());
        msg = getOscAddress(dynauto, chtype, chN, 0);
        sendRequest(msg.c_str());
        msg = getOscAddress(dynkeysrc, chtype, chN, 0);
        sendRequest(msg.c_str());
        msg = getOscAddress(dynfilteron, chtype, chN, 0);
        sendRequest(msg.c_str());
        msg = getOscAddress(dynfiltertype, chtype, chN, 0);
        sendRequest(msg.c_str());
        msg = getOscAddress(dynfilterf, chtype, chN, 0);
        sendRequest(msg.c_str());

        msg = getOscAddress(gateon, chtype, chN, 0);
        sendRequest(msg.c_str());
        msg = getOscAddress(gatethr, chtype, chN, 0);
        sendRequest(msg.c_str());
        msg = getOscAddress(gaterange, chtype, chN, 0);
        sendRequest(msg.c_str());
        msg = getOscAddress(gatemode, chtype, chN, 0);
        sendRequest(msg.c_str());
        msg = getOscAddress(gateattack, chtype, chN, 0);
        sendRequest(msg.c_str());
        msg = getOscAddress(gatehold, chtype, chN, 0);
        sendRequest(msg.c_str());
        msg = getOscAddress(gaterelease, chtype, chN, 0);
        sendRequest(msg.c_str());
        msg = getOscAddress(gatekeysrc, chtype, chN, 0);
        sendRequest(msg.c_str());
        msg = getOscAddress(gatefilteron, chtype, chN, 0);
        sendRequest(msg.c_str());
        msg = getOscAddress(gatefiltertype, chtype, chN, 0);
        sendRequest(msg.c_str());
        msg = getOscAddress(gatefilterf, chtype, chN, 0);

        msg = getOscAddress(keysolo, cherror, 0, 0);
        sendRequest(msg.c_str());
    }
    void processStringMessage(std::string message, std::string sval);
    void processIntMessage(std::string message, int ival);
    void processFloatMessage(std::string message, float fval);
    void processXinfo(std::string *xinfo_) { for(int i = 0;i < 4; i++) xinfo[i] = xinfo_[i]; }
    MessageType getMessageType(std::string message, ChannelType type);
    ChannelType getChannelType(std::string message);
    int getChannelNumber(std::string message);
    int getSendNumber(std::string message);
    //classes
    DebugLibrary *debug;
    IntervalThread *sendRenewMessagesTimer;
    MixFaceLinker *linker;
    MixFaceKeeper *keeper;
};

#endif // MIXFACELIBRARY_H
