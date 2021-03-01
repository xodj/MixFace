#include "MixFaceLibrary.h"

MixFaceLibrary::MixFaceLibrary(QObject *parent, DebugLibrary *debug_)
    : QObject(parent),
      debug(debug_) {
    for (int idx=0;idx<80;idx++){
        db.stereoon[idx] = 1;
        db.fader[idx] = 0.75;
        db.pan[idx] = 0.5;
        db.on[idx] = 1;
        db.solo[idx] = 0;
        db.monoon[idx] = 0;
        db.mlevel[idx] = 0;
        db.chlink[idx] = 0;
        db.phantom[idx] = 0;
        db.invert[idx] = 0;
        db.source[idx] = 0;
        db.gain[idx] = 0.5;
        db.trim[idx] = 0.5;
        db.configicon[idx] = 1;
        db.configcolor[idx] = 1;
        db.configname[idx] = channelNameFromIdx(idx);
        for(int idy=0;idy<16;idy++){
            db.sendlevel[idx][idy] = 0.75;
            db.sendpan[idx][idy] = 0.5;
            db.sendpanfollow[idx][idy] = 0;
            db.sendtype[idx][idy] = 5;
            db.sendon[idx][idy] = 1;
        }
    }
    linker = new MixFaceLinker;
    thread = new QThread;
    linker->moveToThread(thread);
    connect(thread, SIGNAL(started()), linker, SLOT(processMessages()));
    sendRenewMessagesTimer = new QTimer(this);
    QTimer::connect(sendRenewMessagesTimer, &QTimer::timeout, this, &MixFaceLibrary::sendXremoteMessage);
    //connect(linker, &MixFaceLinker::debug, debug, &DebugLibrary::sendMessage);
}

bool MixFaceLibrary::connectTo(string hostNameString){

    thread->start();
    connected = linker->connectTo(hostNameString);
    if (connected) {
        sendRenewMessagesTimer->start(1000);
        linker->listener->s_str.connect(signal_type_str(&MixFaceLibrary::processMessage, this, boost::arg<1>()));

        char buffer[OUTPUT_BUFFER_SIZE];
        osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);

        /*p.Clear();
        p << osc::BeginMessage("/batchsubscribe") << ("meters/9") << ("/meters/9")
            << (int)7 << (int)7 << (int)1 << osc::EndMessage;
        linker->sendDynamicMsg(p);

        p.Clear();
        p << osc::BeginMessage("/batchsubscribe") << ("meters/10") << ("/meters/10")
            << (int)7 << (int)7 << (int)1 << osc::EndMessage;
        linker->sendDynamicMsg(p);

        p.Clear();
        p << osc::BeginMessage("/batchsubscribe") << ("meters/14") << ("/meters/14")
            << (int)7 << (int)7 << (int)1 << osc::EndMessage;
        linker->sendDynamicMsg(p);

        p.Clear();
        p << osc::BeginMessage("/batchsubscribe") << ("meters/0") << ("/meters/0")
            << (int)7 << (int)7 << (int)1 << osc::EndMessage;
        linker->sendDynamicMsg(p);

        p.Clear();
        p << osc::BeginMessage("/batchsubscribe") << ("meters/2") << ("/meters/2")
            << (int)7 << (int)7 << (int)1 << osc::EndMessage;
        linker->sendDynamicMsg(p);

        p.Clear();
        p << osc::BeginMessage("/batchsubscribe") << ("meters/5") << ("/meters/5")
            << (int)3 << (int)0 << (int)1 << osc::EndMessage;
        linker->sendDynamicMsg(p);

        p.Clear();
        p << osc::BeginMessage("/batchsubscribe") << ("meters/6")
          << ("/meters/6") << (int)18 << (int)18 << (int)1
          << osc::EndMessage;
        linker->sendDynamicMsg(p);

        p.Clear();
        p << osc::BeginMessage("/batchsubscribe") << ("meters/16")
          << ("/meters/16") << (int)7 << (int)7 << (int)1
          << osc::EndMessage;
        linker->sendDynamicMsg(p);*/
    } else {
        sendRenewMessagesTimer->stop();
        linker->listener->s_str.disconnect_all_slots();
    }
    return connected;
}

void MixFaceLibrary::sendSyncMessages() {
    for (int idx=0;idx<80;idx++){
        QThread::msleep(1);
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);

        MessageType mtype = fader;
        string msg = getOscAddress(mtype, chtype, chN, 0);
        linker->sendCmdOnly(msg.c_str());

        if (idx<64||idx==70){
            mtype = pan;
            msg = getOscAddress(mtype, chtype, chN, 0);
            linker->sendCmdOnly(msg.c_str());
        }

        mtype = solo;
        msg = getOscAddress(mtype, chtype, idx+1, 0);
        linker->sendCmdOnly(msg.c_str());

        mtype = on;
        msg = getOscAddress(mtype, chtype, chN, 0);
        linker->sendCmdOnly(msg.c_str());
        if(idx<48){
            for(int sendN=1;sendN<17;sendN++){
                mtype = sendlevel;
                msg = getOscAddress(mtype, chtype, chN, sendN);
                linker->sendCmdOnly(msg.c_str());
            }
        }
        if((idx>47&&idx<64)||idx==70||idx==71){
            for(int sendN=1;sendN<7;sendN++){
                mtype = sendlevel;
                msg = getOscAddress(mtype, chtype, chN, sendN);
                linker->sendCmdOnly(msg.c_str());
            }
        }
    }
}

void MixFaceLibrary::sendXremoteMessage() {
    char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);
    p.Clear();
    p << osc::BeginMessage("/xremote") << osc::EndMessage;
    linker->sendDynamicMsg(p);

    /*p.Clear();
    p << osc::BeginMessage("/renew") << ("meters/9") <<osc::EndMessage;
    linker->sendDynamicMsg(p);
    p.Clear();
    p << osc::BeginMessage("/renew") << ("meters/10") <<osc::EndMessage;
    linker->sendDynamicMsg(p);
    p.Clear();
    p << osc::BeginMessage("/renew") << ("meters/14") <<osc::EndMessage;
    linker->sendDynamicMsg(p);*/
    p.Clear();
    p << osc::BeginMessage("/renew") << ("hidden/states") <<osc::EndMessage;
    linker->sendDynamicMsg(p);
    p.Clear();
    p << osc::BeginMessage("/renew") << ("hidden/prefs") <<osc::EndMessage;
    linker->sendDynamicMsg(p);
    p.Clear();
    p << osc::BeginMessage("/renew") << ("hidden/solo") <<osc::EndMessage;
    linker->sendDynamicMsg(p);
    p.Clear();
    p << osc::BeginMessage("/renew") << ("meters/0") <<osc::EndMessage;
    linker->sendDynamicMsg(p);
    p.Clear();
    p << osc::BeginMessage("/renew") << ("meters/2") <<osc::EndMessage;
    linker->sendDynamicMsg(p);
    /*p.Clear();
    p << osc::BeginMessage("/renew") << ("meters/5") <<osc::EndMessage;
    linker->sendDynamicMsg(p);
    p.Clear();
    p << osc::BeginMessage("/renew") << ("meters/6") <<osc::EndMessage;
    linker->sendDynamicMsg(p);
    p.Clear();
    p << osc::BeginMessage("/renew") << ("meters/16") <<osc::EndMessage;
    linker->sendDynamicMsg(p);*/
}

string MixFaceLibrary::channelNameFromIdx(int idx) {
  string name;
  idx = idx + 1;
  if (idx < 33) {
    name = to_string(idx);
    name = ("Ch " + name);
  } else if (idx < 41) {
    name = to_string(idx - 32);
    name = ("Aux " + name);
  } else if (idx == 41) {
    name = ("FX 1L");
  } else if (idx == 42) {
    name = ("FX 1R");
  } else if (idx == 43) {
    name = ("FX 2L");
  } else if (idx == 44) {
    name = ("FX 2R");
  } else if (idx == 45) {
    name = ("FX 3L");
  } else if (idx == 46) {
    name = ("FX 3R");
  } else if (idx == 47) {
    name = ("FX 4L");
  } else if (idx == 48) {
    name = ("FX 4R");
  } else if (idx < 65) {
    name = to_string(idx - 48);
    name = ("Bus " + name);
  } else if (idx < 71) {
    name = to_string(idx - 64);
    name = ("Matrix " + name);
  } else if (idx == 71) {
    name = ("Main LR");
  } else if (idx == 72) {
    name = ("M/C");
  } else if (idx < 81) {
    name = to_string(idx - 72);
    name = ("DCA " + name);
  }
  return name;
}

void MixFaceLibrary::processMessage(string message) {
    MessageType mtype = getMessageType(message);
    ValueType vtype = getValueType(message);

    int chN = getChannelNumber(message);
    ChannelType chtype = getChannelType(message);
    int idx = getIdxFromChNandChType(chN, chtype);
    int sendN = getSendNumber(message);
    float fval = 0.0;
    int ival = 0;
    string sval;

    if (vtype == floatvalue) fval = getFloatValue(message);
    else if (vtype == intvalue) ival = getIntValue(message);
    else if (vtype == stringvalue) sval = getStringValue(message);

    switch (mtype) {
    case stereoon:
        db.stereoon[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case monoon:
        db.monoon[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case mlevel:
        db.mlevel[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case fader:
        db.fader[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case pan:
        db.pan[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case on:
        db.on[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case solo:
        db.solo[chN-1] = ival;
        valueChanged(mtype,chN-1,0);
        break;
    case chlink:
        db.chlink[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case auxlink:
        db.auxlink[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case buslink:
        db.buslink[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case mtxlink:
        db.mtxlink[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case phantom:
        db.phantom[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case invert:
        db.invert[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case source:
        db.source[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case gain:
        db.gain[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case trim:
        db.trim[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case hpf:
        db.hpf[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case hpon:
        db.hpon[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case delayon:
        db.delayon[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case delaytime:
        db.delaytime[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case inserton:
        db.inserton[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case insertsel:
        db.insertsel[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case insertpos:
        db.insertpos[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case gateon:
        db.gateon[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case gatethr:
        db.gatethr[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case gaterange:
        db.gaterange[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case gatemode:
        db.gatemode[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case gateattack:
        db.gateattack[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case gatehold:
        db.gatehold[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case gaterelease:
        db.gaterelease[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case gatekeysrc:
        db.gatekeysrc[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case gatefilteron:
        db.gatefilteron[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case gatefiltertype:
        db.gatefiltertype[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case gatefilterf:
        db.gatefilterf[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case dynon:
        db.dynon[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case dynthr:
        db.dynthr[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case dynratio:
        db.dynratio[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case dynmix:
        db.dynmix[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case dynmgain:
        db.dynmgain[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case dynattack:
        db.dynattack[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case dynhold:
        db.dynhold[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case dynrelease:
        db.dynrelease[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case dynmode:
        db.dynmode[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case dynknee:
        db.dynknee[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case dynenv:
        db.dynenv[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case dyndet:
        db.dyndet[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case dynauto:
        db.dynauto[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case dynkeysrc:
        db.dynkeysrc[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case dynfilteron:
        db.dynfilteron[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case dynfiltertype:
        db.dynfiltertype[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case dynfilterf:
        db.dynfilterf[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq1type:
        db.eq1type[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case eq1g:
        db.eq1g[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq1f:
        db.eq1f[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq1q:
        db.eq1q[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq2type:
        db.eq2type[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case eq2g:
        db.eq2g[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq2f:
        db.eq2f[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq2q:
        db.eq2q[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq3type:
        db.eq3type[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case eq3g:
        db.eq3g[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq3f:
        db.eq3f[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq3q:
        db.eq3q[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq4type:
        db.eq4type[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case eq4g:
        db.eq4g[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq4f:
        db.eq4f[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq4q:
        db.eq4q[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq5type:
        db.eq5type[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case eq5g:
        db.eq5g[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq5f:
        db.eq5f[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq5q:
        db.eq5q[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq6type:
        db.eq6type[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case eq6g:
        db.eq6g[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq6f:
        db.eq6f[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq6q:
        db.eq6q[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case sendlevel:
        db.sendlevel[idx][sendN] = fval;
        valueChanged(mtype,idx,sendN);
        break;
    case sendpan:
        db.sendpan[idx][sendN] = fval;
        valueChanged(mtype,idx,sendN);
        break;
    case sendpanfollow:
        db.sendpanfollow[idx][sendN] = ival;
        valueChanged(mtype,idx,sendN);
        break;
    case sendtype:
        db.sendtype[idx][sendN] = ival;
        valueChanged(mtype,idx,sendN);
        break;
    case sendon:
        db.sendon[idx][sendN] = ival;
        valueChanged(mtype,idx,sendN);
        break;
    case configicon:
        db.configicon[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case configcolor:
        db.configcolor[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case configname:
        db.configname[idx] = sval;
        valueChanged(mtype,idx,0);
        break;
    case merror:
        debug->sendMessage("Error message read: " + message,0);
        break;
    }
}

string MixFaceLibrary::getOscAddress(MessageType mtype,
                                   ChannelType chtype,
                                   int channelN, int sendN)
{
    string oscAddress;
    switch (mtype) {
    case stereoon:
        oscAddress = ("/mix/st");
        break;
    case fader:
        oscAddress = ("/mix/fader");
        break;
    case pan:
        oscAddress = ("/mix/pan");
        break;
    case on:
        oscAddress = ("/mix/on");
        break;
    case solo:
        if (channelN < 10)
            oscAddress = ("/-stat/solosw/0" + to_string(channelN));
        else
            oscAddress = ("/-stat/solosw/" + to_string(channelN));
        break;
    case monoon:
        oscAddress = ("/mix/mono");
        break;
    case mlevel:
        oscAddress = ("/mix/mlevel");
        break;
    case chlink:
        if (channelN % 2 == 0) channelN = channelN-1;
        oscAddress = ("/config/chlink/" + to_string(channelN) + "-" +
                      to_string(channelN + 1));
        break;
    case auxlink:
        if (channelN % 2 == 0) channelN = channelN-1;
        oscAddress = ("/config/auxlink/" + to_string(channelN) + "-" +
                      to_string(channelN + 1));
        break;
    case buslink:
        if (channelN % 2 == 0) channelN = channelN-1;
        oscAddress = ("/config/buslink/" + to_string(channelN) + "-" +
                      to_string(channelN + 1));
        break;
    case mtxlink:
        if (channelN % 2 == 0) channelN = channelN-1;
        oscAddress = ("/config/mtxlink/" + to_string(channelN) + "-" +
                      to_string(channelN + 1));
        break;
    case phantom:
        if (channelN < 10)
            oscAddress = ("/headamp/00" + to_string(channelN) + "/phantom");
        else if (channelN < 100)
            oscAddress = ("/headamp/0" + to_string(channelN) + "/phantom");
        else
            oscAddress = ("/headamp/" + to_string(channelN) + "/phantom");
        break;
    case invert:
        oscAddress = ("/preamp/invert");
        break;
    case source:
        oscAddress = ("/config/source");
        break;
    case gain:
        if (channelN < 10)
            oscAddress = ("/headamp/00" + to_string(channelN-1) + "/gain");
        else if (channelN < 100)
            oscAddress = ("/headamp/0" + to_string(channelN-1) + "/gain");
        else
            oscAddress = ("/headamp/" + to_string(channelN-1) + "/gain");
        break;
    case trim:
        oscAddress = ("/preamp/trim");
        break;
    case hpf:
        oscAddress = ("/preamp/hpf");
        break;
    case hpon:
        oscAddress = ("/preamp/hpon");
        break;
    case delayon:
        oscAddress = ("/delay/on");
        break;
    case delaytime:
        oscAddress = ("/delay/time");
        break;
    case inserton:
        oscAddress = ("/insert/on");
        break;
    case insertsel:
        oscAddress = ("/insert/sel");
        break;
    case insertpos:
        oscAddress = ("/insert/pos");
        break;
    case gateon:
        oscAddress = ("/gate/on");
        break;
    case gatethr:
        oscAddress = ("/gate/thr");
        break;
    case gaterange:
        oscAddress = ("/gate/range");
        break;
    case gatemode:
        oscAddress = ("/gate/mode");
        break;
    case gateattack:
        oscAddress = ("/gate/attack");
        break;
    case gatehold:
        oscAddress = ("/gate/hold");
        break;
    case gaterelease:
        oscAddress = ("/gate/release");
        break;
    case gatekeysrc:
        oscAddress = ("/gate/keysrc");
        break;
    case gatefilteron:
        oscAddress = ("/gate/filter/on");
        break;
    case gatefiltertype:
        oscAddress = ("/gate/filter/type");
        break;
    case gatefilterf:
        oscAddress = ("/gate/filter/f");
        break;
    case dynon:
        oscAddress = ("/dyn/on");
        break;
    case dynthr:
        oscAddress = ("/dyn/thr");
        break;
    case dynratio:
        oscAddress = ("/dyn/ratio");
        break;
    case dynmix:
        oscAddress = ("/dyn/mix");
        break;
    case dynmgain:
        oscAddress = ("/dyn/mgain");
        break;
    case dynattack:
        oscAddress = ("/dyn/attack");
        break;
    case dynhold:
        oscAddress = ("/dyn/hold");
        break;
    case dynrelease:
        oscAddress = ("/dyn/release");
        break;
    case dynmode:
        oscAddress = ("/dyn/mode");
        break;
    case dynknee:
        oscAddress = ("/dyn/knee");
        break;
    case dynenv:
        oscAddress = ("/dyn/env");
        break;
    case dyndet:
        oscAddress = ("/dyn/det");
        break;
    case dynauto:
        oscAddress = ("/dyn/auto");
        break;
    case dynkeysrc:
        oscAddress = ("/dyn/keysrc");
        break;
    case dynfilteron:
        oscAddress = ("/dyn/filter/on");
        break;
    case dynfiltertype:
        oscAddress = ("/dyn/filter/type");
        break;
    case dynfilterf:
        oscAddress = ("/dyn/filter/f");
        break;
    case eq1type:
        oscAddress = ("/eq/1/type");
        break;
    case eq1g:
        oscAddress = ("/eq/1/g");
        break;
    case eq1f:
        oscAddress = ("/eq/1/f");
        break;
    case eq1q:
        oscAddress = ("/eq/1/q");
        break;
    case eq2type:
        oscAddress = ("/eq/2/type");
        break;
    case eq2g:
        oscAddress = ("/eq/2/g");
        break;
    case eq2f:
        oscAddress = ("/eq/2/f");
        break;
    case eq2q:
        oscAddress = ("/eq/2/q");
        break;
    case eq3type:
        oscAddress = ("/eq/3/type");
        break;
    case eq3g:
        oscAddress = ("/eq/3/g");
        break;
    case eq3f:
        oscAddress = ("/eq/3/f");
        break;
    case eq3q:
        oscAddress = ("/eq/3/q");
        break;
    case eq4type:
        oscAddress = ("/eq/4/type");
        break;
    case eq4g:
        oscAddress = ("/eq/4/g");
        break;
    case eq4f:
        oscAddress = ("/eq/4/f");
        break;
    case eq4q:
        oscAddress = ("/eq/4/q");
        break;
    case eq5type:
        oscAddress = ("/eq/5/type");
        break;
    case eq5g:
        oscAddress = ("/eq/5/g");
        break;
    case eq5f:
        oscAddress = ("/eq/5/f");
        break;
    case eq5q:
        oscAddress = ("/eq/5/q");
        break;
    case eq6type:
        oscAddress = ("/eq/6/type");
        break;
    case eq6g:
        oscAddress = ("/eq/6/g");
        break;
    case eq6f:
        oscAddress = ("/eq/6/f");
        break;
    case eq6q:
        oscAddress = ("/eq/6/q");
        break;
    case sendlevel:
        if (sendN < 10)
            oscAddress = ("/mix/0" + to_string(sendN) + "/level");
        else
            oscAddress = ("/mix/" + to_string(sendN) + "/level");
        break;
    case sendpan:
        if (sendN < 10)
            oscAddress = ("/mix/0" + to_string(sendN) + "/pan");
        else
            oscAddress = ("/mix/" + to_string(sendN) + "/pan");
        break;
    case sendpanfollow:
        if (sendN < 10)
            oscAddress = ("/mix/0" + to_string(sendN) + "/panFollow");
        else
            oscAddress = ("/mix/" + to_string(sendN) + "/panFollow");
        break;
    case sendtype:
        if (sendN < 10)
            oscAddress = ("/mix/0" + to_string(sendN) + "/type");
        else
            oscAddress = ("/mix/" + to_string(sendN) + "/type");
        break;
    case sendon:
        if (sendN < 10)
            oscAddress = ("/mix/0" + to_string(sendN) + "/on");
        else
            oscAddress = ("/mix/" + to_string(sendN) + "/on");
        break;
    case configicon:
        oscAddress = ("/config/icon");
        break;
    case configcolor:
        oscAddress = ("/config/color");
        break;
    case configname:
        oscAddress = ("/config/name");
        break;
    case merror:
        debug->sendMessage("Message type error " + to_string(channelN),0);
        oscAddress = nullptr;
        break;
    }
    if (mtype != solo && mtype != chlink && mtype != auxlink &&
            mtype != buslink && mtype != mtxlink && mtype != phantom &&
            mtype != gain) {
        switch (chtype) {
        case channel:
            if (channelN < 10) {
                oscAddress = ("/ch/0" + to_string(channelN) + oscAddress);
            } else {
                oscAddress = ("/ch/" + to_string(channelN) + oscAddress);
            }
            break;
        case auxin:
            oscAddress = ("/auxin/0" + to_string(channelN) + oscAddress);
            break;
        case fxreturn:
            oscAddress = ("/fxrtn/0" + to_string(channelN) + oscAddress);
            break;
        case bus:
            if (channelN < 10) {
                oscAddress = ("/bus/0" + to_string(channelN) + oscAddress);
            } else {
                oscAddress = ("/bus/" + to_string(channelN) + oscAddress);
            }
            break;
        case matrix:
            oscAddress = ("/mtx/0" + to_string(channelN) + oscAddress);
            break;
        case lr:
            oscAddress = ("/main/st" + oscAddress);
            break;
        case mc:
            oscAddress = ("/main/m" + oscAddress);
            break;
        case dca:
            if (mtype == fader)
                oscAddress = ("/dca/" + to_string(channelN) + "/fader");
            else if (mtype == on)
                oscAddress = ("/dca/" + to_string(channelN) + "/on");
            break;
        case headamp:
            break;
        case cherror:
            debug->sendMessage("Channel type error " + to_string(channelN),0);
            oscAddress = nullptr;
            break;
        }
    }
    return oscAddress;
}

ChannelType MixFaceLibrary::getChannelTypeFromIdx(int idx) {
    ChannelType chtype = cherror;
    if (idx<32)
        chtype = channel;
    else if (idx < 40)
        chtype = auxin;
    else if (idx < 48)
        chtype = fxreturn;
    else if (idx < 64)
        chtype = bus;
    else if (idx < 70)
        chtype = matrix;
    else if (idx == 70)
        chtype = lr;
    else if (idx == 71)
        chtype = mc;
    else if (idx < 80)
        chtype = dca;
    return chtype;
}

int MixFaceLibrary::getChannelNumberFromIdx(int idx) {
  if (idx < 32) return (idx + 1);
  else if (idx < 40) return (idx - 31);
  else if (idx < 48) return (idx - 39);
  else if (idx < 64) return (idx - 47);
  else if (idx < 70) return (idx - 63);
  else if (idx == 70) return 0;
  else if (idx == 71) return 0;
  else if (idx < 80) return (idx - 71);
  else return 0;
}

MessageType MixFaceLibrary::getMessageType(string message) {
    MessageType mtype = merror;
    if (strstr(message.c_str(), msgTypeStr.stereoon)) mtype = stereoon;
    else if (strstr(message.c_str(), msgTypeStr.monoon)) mtype = monoon;
    else if (strstr(message.c_str(), msgTypeStr.mlevel)) mtype = mlevel;
    else if (strstr(message.c_str(), msgTypeStr.fader)) mtype = fader;
    else if (strstr(message.c_str(), msgTypeStr.pan)) mtype = pan;
    else if (strstr(message.c_str(), msgTypeStr.on)) mtype = on;
    else if (strstr(message.c_str(), msgTypeStr.solo)) mtype = solo;
    else if (strstr(message.c_str(), msgTypeStr.sendlevel)) mtype = sendlevel;
    else if (strstr(message.c_str(), msgTypeStr.sendpan)) mtype = sendpan;
    else if (strstr(message.c_str(), msgTypeStr.sendpanfollow)) mtype = sendpanfollow;
    else if (strstr(message.c_str(), msgTypeStr.sendtype)) mtype = sendtype;
    else if (strstr(message.c_str(), msgTypeStr.sendon)) mtype = sendon;
    else if (strstr(message.c_str(), msgTypeStr.dcaon)) mtype = on;
    else if (strstr(message.c_str(), msgTypeStr.chlink)) mtype = chlink;
    else if (strstr(message.c_str(), msgTypeStr.auxlink)) mtype = auxlink;
    else if (strstr(message.c_str(), msgTypeStr.buslink)) mtype = buslink;
    else if (strstr(message.c_str(), msgTypeStr.mtxlink)) mtype = mtxlink;
    else if (strstr(message.c_str(), msgTypeStr.phantom)) mtype = phantom;
    else if (strstr(message.c_str(), msgTypeStr.invert)) mtype = invert;
    else if (strstr(message.c_str(), msgTypeStr.source)) mtype = source;
    else if (strstr(message.c_str(), msgTypeStr.gain)) mtype = gain;
    else if (strstr(message.c_str(), msgTypeStr.trim)) mtype = trim;
    else if (strstr(message.c_str(), msgTypeStr.hpf)) mtype = hpf;
    else if (strstr(message.c_str(), msgTypeStr.hpon)) mtype = hpon;
    else if (strstr(message.c_str(), msgTypeStr.delayon)) mtype = delayon;
    else if (strstr(message.c_str(), msgTypeStr.delaytime)) mtype = delaytime;
    else if (strstr(message.c_str(), msgTypeStr.inserton)) mtype = inserton;
    else if (strstr(message.c_str(), msgTypeStr.insertsel)) mtype = insertsel;
    else if (strstr(message.c_str(), msgTypeStr.insertpos)) mtype = insertpos;
    else if (strstr(message.c_str(), msgTypeStr.gateon)) mtype = gateon;
    else if (strstr(message.c_str(), msgTypeStr.gatethr)) mtype = gatethr;
    else if (strstr(message.c_str(), msgTypeStr.gaterange)) mtype = gaterange;
    else if (strstr(message.c_str(), msgTypeStr.gatemode)) mtype = gatemode;
    else if (strstr(message.c_str(), msgTypeStr.gateattack)) mtype = gateattack;
    else if (strstr(message.c_str(), msgTypeStr.gatehold)) mtype = gatehold;
    else if (strstr(message.c_str(), msgTypeStr.gaterelease)) mtype = gaterelease;
    else if (strstr(message.c_str(), msgTypeStr.gatekeysrc)) mtype = gatekeysrc;
    else if (strstr(message.c_str(), msgTypeStr.gatefilteron)) mtype = gatefilteron;
    else if (strstr(message.c_str(), msgTypeStr.gatefiltertype)) mtype = gatefiltertype;
    else if (strstr(message.c_str(), msgTypeStr.gatefilterf)) mtype = gatefilterf;
    else if (strstr(message.c_str(), msgTypeStr.dynon)) mtype = dynon;
    else if (strstr(message.c_str(), msgTypeStr.dynthr)) mtype = dynthr;
    else if (strstr(message.c_str(), msgTypeStr.dynratio)) mtype = dynratio;
    else if (strstr(message.c_str(), msgTypeStr.dynmix)) mtype = dynmix;
    else if (strstr(message.c_str(), msgTypeStr.dynmgain)) mtype = dynmgain;
    else if (strstr(message.c_str(), msgTypeStr.dynattack)) mtype = dynattack;
    else if (strstr(message.c_str(), msgTypeStr.dynhold)) mtype = dynhold;
    else if (strstr(message.c_str(), msgTypeStr.dynrelease)) mtype = dynrelease;
    else if (strstr(message.c_str(), msgTypeStr.dynmode)) mtype = dynmode;
    else if (strstr(message.c_str(), msgTypeStr.dynknee)) mtype = dynknee;
    else if (strstr(message.c_str(), msgTypeStr.dynenv)) mtype = dynenv;
    else if (strstr(message.c_str(), msgTypeStr.dyndet)) mtype = dyndet;
    else if (strstr(message.c_str(), msgTypeStr.dynauto)) mtype = dynauto;
    else if (strstr(message.c_str(), msgTypeStr.dynkeysrc)) mtype = dynkeysrc;
    else if (strstr(message.c_str(), msgTypeStr.dynfilteron)) mtype = dynfilteron;
    else if (strstr(message.c_str(), msgTypeStr.dynfiltertype)) mtype = dynfiltertype;
    else if (strstr(message.c_str(), msgTypeStr.dynfilterf)) mtype = dynfilterf;
    else if (strstr(message.c_str(), msgTypeStr.eq1type)) mtype = eq1type;
    else if (strstr(message.c_str(), msgTypeStr.eq1g)) mtype = eq1g;
    else if (strstr(message.c_str(), msgTypeStr.eq1f)) mtype = eq1f;
    else if (strstr(message.c_str(), msgTypeStr.eq1q)) mtype = eq1q;
    else if (strstr(message.c_str(), msgTypeStr.eq2type)) mtype = eq2type;
    else if (strstr(message.c_str(), msgTypeStr.eq2g)) mtype = eq2g;
    else if (strstr(message.c_str(), msgTypeStr.eq2f)) mtype = eq2f;
    else if (strstr(message.c_str(), msgTypeStr.eq2q)) mtype = eq2q;
    else if (strstr(message.c_str(), msgTypeStr.eq3type)) mtype = eq3type;
    else if (strstr(message.c_str(), msgTypeStr.eq3g)) mtype = eq3g;
    else if (strstr(message.c_str(), msgTypeStr.eq3f)) mtype = eq3f;
    else if (strstr(message.c_str(), msgTypeStr.eq3q)) mtype = eq3q;
    else if (strstr(message.c_str(), msgTypeStr.eq4type)) mtype = eq4type;
    else if (strstr(message.c_str(), msgTypeStr.eq4g)) mtype = eq4g;
    else if (strstr(message.c_str(), msgTypeStr.eq4f)) mtype = eq4f;
    else if (strstr(message.c_str(), msgTypeStr.eq4q)) mtype = eq4q;
    else if (strstr(message.c_str(), msgTypeStr.eq5type)) mtype = eq5type;
    else if (strstr(message.c_str(), msgTypeStr.eq5g)) mtype = eq5g;
    else if (strstr(message.c_str(), msgTypeStr.eq5f)) mtype = eq5f;
    else if (strstr(message.c_str(), msgTypeStr.eq5q)) mtype = eq5q;
    else if (strstr(message.c_str(), msgTypeStr.eq6type)) mtype = eq6type;
    else if (strstr(message.c_str(), msgTypeStr.eq6g)) mtype = eq6g;
    else if (strstr(message.c_str(), msgTypeStr.eq6f)) mtype = eq6f;
    else if (strstr(message.c_str(), msgTypeStr.eq6q)) mtype = eq6q;
    else if (strstr(message.c_str(), msgTypeStr.configicon)) mtype = configicon;
    else if (strstr(message.c_str(), msgTypeStr.configcolor)) mtype = configcolor;
    else if (strstr(message.c_str(), msgTypeStr.configname)) mtype = configname;
    return mtype;
}

ChannelType MixFaceLibrary::getChannelType(string message) {
    ChannelType chtype = cherror;
    if (strstr(message.c_str(), chTypeStr.channel)) chtype = channel;
    else if (strstr(message.c_str(), chTypeStr.auxin)) chtype = auxin;
    else if (strstr(message.c_str(), chTypeStr.fxreturn)) chtype = fxreturn;
    else if (strstr(message.c_str(), chTypeStr.bus)) chtype = bus;
    else if (strstr(message.c_str(), chTypeStr.matrix)) chtype = matrix;
    else if (strstr(message.c_str(), chTypeStr.lr)) chtype = lr;
    else if (strstr(message.c_str(), chTypeStr.mc)) chtype = mc;
    else if (strstr(message.c_str(), chTypeStr.dca)) chtype = dca;
    return chtype;
}

ValueType MixFaceLibrary::getValueType(string message) {
    ValueType vtype = verror;
    if (strstr(message.c_str(), valTypeStr.intvalue)) vtype = intvalue;
    else if (strstr(message.c_str(), valTypeStr.floatvalue)) vtype = floatvalue;
    else if (strstr(message.c_str(), valTypeStr.stringvalue)) vtype = stringvalue;
    else if (strstr(message.c_str(), valTypeStr.boolvalue)) vtype = boolvalue;
    return vtype;
}

int MixFaceLibrary::getChannelNumber(string message) {
    debug->sendMessage("MixFaceLibrary::getChannelNumber in message: " + message, 3);
    int chN = 0;
    if (strstr(message.c_str(), busTypeStr.ch)) {
        message.erase(0,strlen(busTypeStr.ch));
        debug->sendMessage("MixFaceLibrary::getChannelNumber erase symbols: " + to_string(strlen(busTypeStr.ch)), 3);
        message.resize(2);
        chN = atoi(message.c_str());
    } else if (strstr(message.c_str(), busTypeStr.auxin)) {
        message.erase(0,strlen(busTypeStr.auxin));
        message.resize(2);
        chN = atoi(message.c_str());
    } else if (strstr(message.c_str(), busTypeStr.fxrtn)) {
        message.erase(0,strlen(busTypeStr.fxrtn));
        message.resize(2);
        chN = atoi(message.c_str());
    } else if (strstr(message.c_str(), busTypeStr.bus)) {
        message.erase(0,strlen(busTypeStr.bus));
        message.resize(2);
        chN = atoi(message.c_str());
    } else if (strstr(message.c_str(), busTypeStr.mtx)) {
        message.erase(0,strlen(busTypeStr.mtx));
        message.resize(2);
        chN = atoi(message.c_str());
    } else if (strstr(message.c_str(), busTypeStr.mainst)) {
        chN = 0;
    } else if (strstr(message.c_str(), busTypeStr.mainm)) {
        chN = 0;
    } else if (strstr(message.c_str(), busTypeStr.dca)) {
        message.erase(0,strlen(busTypeStr.dca));
        message.resize(1);
        chN = atoi(message.c_str());
    } else if (strstr(message.c_str(), busTypeStr.headamp)) {
        message.erase(0,strlen(busTypeStr.headamp));
        message.resize(3);
        chN = atoi(message.c_str());
    } else if (strstr(message.c_str(), busTypeStr.statsolosw)) {
        message.erase(0,strlen(busTypeStr.statsolosw));
        message.resize(2);
        chN = atoi(message.c_str());
    }
    debug->sendMessage("MixFaceLibrary::getChannelNumber ch number is: " + to_string(chN), 3);
    return chN;
}

int MixFaceLibrary::getSendNumber(string message) {
    debug->sendMessage("MixFaceLibrary::getSendNumber in message: " + message, 3);
    int sendN = 0;
    if (strstr(message.c_str(), busTypeStr.ch)) {
        message.erase(0,strlen(busTypeStr.ch) + strlen(busTypeStr.mix) + 2);
        message.resize(2);
        sendN = atoi(message.c_str());
    } else if (strstr(message.c_str(), busTypeStr.auxin)) {
        message.erase(0,strlen(busTypeStr.auxin) + strlen(busTypeStr.mix) + 2);
        message.resize(2);
        sendN = atoi(message.c_str());
    } else if (strstr(message.c_str(), busTypeStr.fxrtn)) {
        message.erase(0,strlen(busTypeStr.fxrtn) + strlen(busTypeStr.mix) + 2);
        message.resize(2);
        sendN = atoi(message.c_str());
    } else if (strstr(message.c_str(), busTypeStr.bus)) {
        message.erase(0,strlen(busTypeStr.bus) + strlen(busTypeStr.mix) + 2);
        message.resize(2);
        sendN = atoi(message.c_str());
    } else if (strstr(message.c_str(), busTypeStr.mainst)) {
        message.erase(0,strlen("%A/main/st/mix/"));
        message.resize(2);
        sendN = atoi(message.c_str());
    } else if (strstr(message.c_str(), busTypeStr.mainm)) {
        message.erase(0,strlen("%A/main/m/mix/"));
        message.resize(2);
        sendN = atoi(message.c_str());
    }
    debug->sendMessage("MixFaceLibrary::getSendNumber send number: " + to_string(sendN), 3);
    return sendN;
}

float MixFaceLibrary::getFloatValue(string message) {
    debug->sendMessage("MixFaceLibrary::getFloatValue in message: " + message, 3);
    float fval = 0;
    int i = message.find(valTypeStr.floatvalue) + 2;
    debug->sendMessage("MixFaceLibrary::getFloatValue find val index: " + to_string(i), 3);
    message.erase(0,i);
    debug->sendMessage("MixFaceLibrary::getFloatValue value in str: " + message, 3);
    fval = atof(message.c_str());
    debug->sendMessage("MixFaceLibrary::getFloatValue value in float: " + to_string(fval), 3);
    return fval;
}

int MixFaceLibrary::getIntValue(string message) {
    debug->sendMessage("MixFaceLibrary::getIntValue in message: " + message, 3);
    int ival = 0;
    int i = message.find(valTypeStr.intvalue) + 2;
    debug->sendMessage("MixFaceLibrary::getIntValue find val index: " + to_string(i), 3);
    message.erase(0,i);
    debug->sendMessage("MixFaceLibrary::getIntValue value in str: " + message, 3);
    ival = atoi(message.c_str());
    debug->sendMessage("MixFaceLibrary::getIntValue value in int: " + to_string(ival), 3);
    return ival;
}

string MixFaceLibrary::getStringValue(string message) {
    debug->sendMessage("MixFaceLibrary::getStringValue in message: " + message, 3);
    int i = message.find(valTypeStr.stringvalue) + 2;
    debug->sendMessage("MixFaceLibrary::getStringValue find val index: " + to_string(i), 3);
    message.erase(0,i);
    debug->sendMessage("MixFaceLibrary::getStringValue value in str: " + message, 3);
    return message;
}

int MixFaceLibrary::getIdxFromChNandChType(int chN, ChannelType chtype){
    int idx = -1;
    switch (chtype){
    case channel: idx = chN - 1; break;
    case auxin: idx = chN + 31; break;
    case fxreturn: idx = chN + 39; break;
    case bus: idx = chN + 47; break;
    case matrix: idx = chN + 63; break;
    case lr: idx = 70; break;
    case mc: idx = 71; break;
    case dca: idx = chN + 71; break;
    case headamp: idx = -1; break;
    case cherror: idx = -1; break; }
    return idx;
}

