#include "MixFaceLibrary.h"
#include "osc/OscTypes.h"

MixFaceLibrary::MixFaceLibrary(DebugLibrary *debug_)
    : debug(debug_){
    linker = new MixFaceLinker(debug);
    keeper = new MixFaceKeeper(debug);
    db = keeper->getX32DB();
    sendRenewMessagesTimer = new IntervalThread(1000, false, true);
    sendRenewMessagesTimer->connect(IntervalThread::interval_slot_t(&MixFaceLibrary::sendXremoteMessage, this));
    linker->listener->s_xi.connect(signal_type_xi(&MixFaceLibrary::processXinfo, this, boost::arg<1>()));
}

void MixFaceLibrary::connect(std::string hostNameString){
    bool connected = linker->connectTo(hostNameString);
    if (connected) {
        sendRenewMessagesTimer->start();
        linker->listener->s_str_str.connect(signal_type_str_str(&MixFaceLibrary::processStringMessage, this, boost::arg<1>(), boost::arg<2>()));
        linker->listener->s_str_int.connect(signal_type_str_int(&MixFaceLibrary::processIntMessage, this, boost::arg<1>(), boost::arg<2>()));
        linker->listener->s_str_float.connect(signal_type_str_float(&MixFaceLibrary::processFloatMessage, this, boost::arg<1>(), boost::arg<2>()));

        linker->listener->newMeters1.connect(newMeters1);
        linker->listener->newMeters2.connect(newMeters2);
        linker->listener->newMeters3.connect(newMeters3);

        char buffer[OUTPUT_BUFFER_SIZE];
        osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);

        p.Clear();
        p << osc::BeginMessage("/batchsubscribe") << ("meters/1") << ("/meters/1")
            << (int)7 << (int)7 << (int)1;
        linker->sendDynamicMsg(p);

        p.Clear();
        p << osc::BeginMessage("/batchsubscribe") << ("meters/2") << ("/meters/2")
            << (int)7 << (int)7 << (int)1;
        linker->sendDynamicMsg(p);

        p.Clear();
        p << osc::BeginMessage("/batchsubscribe") << ("meters/3") << ("/meters/3")
            << (int)7 << (int)7 << (int)1;
        linker->sendDynamicMsg(p);
    } else {
        sendRenewMessagesTimer->stop();
        linker->listener->s_str_str.disconnect_all_slots();
        linker->listener->s_str_int.disconnect_all_slots();
        linker->listener->s_str_float.disconnect_all_slots();

        linker->listener->newMeters1.disconnect_all_slots();
        linker->listener->newMeters2.disconnect_all_slots();
        linker->listener->newMeters3.disconnect_all_slots();
    }
    slotConnected(connected);
}

void MixFaceLibrary::sendSyncMessages() {
    for (int idx=0;idx<80;idx++){
        boost::this_thread::sleep_for(boost::chrono::milliseconds{1});
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);

        MessageType mtype = fader;
        std::string msg = getOscAddress(mtype, chtype, chN, 0);
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

        mtype = configcolor;
        msg = getOscAddress(mtype, chtype, chN, 0);
        linker->sendCmdOnly(msg.c_str());
        mtype = configicon;
        msg = getOscAddress(mtype, chtype, chN, 0);
        linker->sendCmdOnly(msg.c_str());
        mtype = configname;
        msg = getOscAddress(mtype, chtype, chN, 0);
        linker->sendCmdOnly(msg.c_str());
    }
}

void MixFaceLibrary::sendXremoteMessage() {
    char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);

    p.Clear();
    p << osc::BeginMessage("/xremote");
    linker->sendDynamicMsg(p);

    p.Clear();
    p << osc::BeginMessage("/renew") << ("hidden/states");
    linker->sendDynamicMsg(p);
    p.Clear();
    p << osc::BeginMessage("/renew") << ("hidden/prefs");
    linker->sendDynamicMsg(p);
    p.Clear();
    p << osc::BeginMessage("/renew") << ("hidden/solo");
    linker->sendDynamicMsg(p);
    p.Clear();
    p << osc::BeginMessage("/renew") << ("meters/1");
    linker->sendDynamicMsg(p);
    p.Clear();
    p << osc::BeginMessage("/renew") << ("meters/2");
    linker->sendDynamicMsg(p);
    p.Clear();
    p << osc::BeginMessage("/renew") << ("meters/3");
    linker->sendDynamicMsg(p);
}

void MixFaceLibrary::processStringMessage(std::string message, std::string sval) {
    ChannelType chtype = getChannelType(message);
    MessageType mtype = getMessageType(message, chtype);

    int chN = getChannelNumber(message);
    int idx = getIdxFromChNandChType(chN, chtype);

    switch (mtype) {
    case configname:
        db->configname[idx] = sval;
        valueChanged(mtype,idx,0);
        break;
    case merror:
        debug->sendMessage("Error message read: " + message + " with string value " + sval,0);
        break;
    }
}

void MixFaceLibrary::processIntMessage(std::string message, int ival) {
    ChannelType chtype = getChannelType(message);
    MessageType mtype = getMessageType(message, chtype);

    int chN = getChannelNumber(message);
    int idx = getIdxFromChNandChType(chN, chtype);
    int sendN = getSendNumber(message);

    switch (mtype) {
    case stereoon:
        db->stereoon[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case monoon:
        db->monoon[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case on:
        db->on[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case solo:
        db->solo[chN-1] = ival;
        valueChanged(mtype,chN-1,0);
        break;
    case keysolo:
        db->keysolo = ival;
        valueChanged(mtype,0,0);
        break;
    case chlink:
        db->chlink[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case auxlink:
        db->auxlink[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case buslink:
        db->buslink[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case mtxlink:
        db->mtxlink[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case phantom:
        db->phantom[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case invert:
        db->invert[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case source:
        db->source[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case hpon:
        db->hpon[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case delayon:
        db->delayon[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case inserton:
        db->inserton[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case insertsel:
        db->insertsel[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case insertpos:
        db->insertpos[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case gateon:
        db->gateon[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case gatemode:
        db->gatemode[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case gatekeysrc:
        db->gatekeysrc[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case gatefilteron:
        db->gatefilteron[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case gatefiltertype:
        db->gatefiltertype[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case dynon:
        db->dynon[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case dynratio:
        db->dynratio[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case dynmode:
        db->dynmode[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case dynenv:
        db->dynenv[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case dyndet:
        db->dyndet[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case dynauto:
        db->dynauto[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case dynkeysrc:
        db->dynkeysrc[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case dynfilteron:
        db->dynfilteron[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case dynfiltertype:
        db->dynfiltertype[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case eq1type:
        db->eq1type[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case eq2type:
        db->eq2type[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case eq3type:
        db->eq3type[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case eq4type:
        db->eq4type[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case eq5type:
        db->eq5type[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case eq6type:
        db->eq6type[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case sendpanfollow:
        db->sendpanfollow[idx][sendN] = ival;
        valueChanged(mtype,idx,sendN);
        break;
    case sendtype:
        db->sendtype[idx][sendN] = ival;
        valueChanged(mtype,idx,sendN);
        break;
    case sendon:
        db->sendon[idx][sendN] = ival;
        valueChanged(mtype,idx,sendN);
        break;
    case configicon:
        db->configicon[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case configcolor:
        db->configcolor[idx] = ival;
        valueChanged(mtype,idx,0);
        break;
    case merror:
        debug->sendMessage("Error message read: " + message + " with int value " + std::to_string(ival),0);
        break;
    }
}

void MixFaceLibrary::processFloatMessage(std::string message, float fval) {
    ChannelType chtype = getChannelType(message);
    MessageType mtype = getMessageType(message, chtype);

    int chN = getChannelNumber(message);
    int idx = getIdxFromChNandChType(chN, chtype);
    int sendN = getSendNumber(message);

    switch (mtype) {
    case mlevel:
        db->mlevel[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case fader:
        db->fader[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case pan:
        db->pan[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case gain:
        db->gain[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case trim:
        db->trim[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case hpf:
        db->hpf[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case delaytime:
        db->delaytime[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case gatethr:
        db->gatethr[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case gaterange:
        db->gaterange[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case gateattack:
        db->gateattack[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case gatehold:
        db->gatehold[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case gaterelease:
        db->gaterelease[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case gatefilterf:
        db->gatefilterf[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case dynthr:
        db->dynthr[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case dynmix:
        db->dynmix[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case dynmgain:
        db->dynmgain[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case dynattack:
        db->dynattack[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case dynhold:
        db->dynhold[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case dynrelease:
        db->dynrelease[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case dynknee:
        db->dynknee[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case dynfilterf:
        db->dynfilterf[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq1g:
        db->eq1g[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq1f:
        db->eq1f[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq1q:
        db->eq1q[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq2g:
        db->eq2g[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq2f:
        db->eq2f[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq2q:
        db->eq2q[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq3g:
        db->eq3g[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq3f:
        db->eq3f[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq3q:
        db->eq3q[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq4g:
        db->eq4g[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq4f:
        db->eq4f[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq4q:
        db->eq4q[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq5g:
        db->eq5g[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq5f:
        db->eq5f[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq5q:
        db->eq5q[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq6g:
        db->eq6g[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq6f:
        db->eq6f[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case eq6q:
        db->eq6q[idx] = fval;
        valueChanged(mtype,idx,0);
        break;
    case sendlevel:
        db->sendlevel[idx][sendN] = fval;
        valueChanged(mtype,idx,sendN);
        break;
    case sendpan:
        db->sendpan[idx][sendN] = fval;
        valueChanged(mtype,idx,sendN);
        break;
    case merror:
        debug->sendMessage("Error message read: " + message + " with float value " + std::to_string(fval),0);
        break;
    }
}

MessageType MixFaceLibrary::getMessageType(std::string message, ChannelType type) {
    MessageType mtype = merror;
    if (type != dca) {
        if (strstr(message.c_str(), msgTypeStr.stereoon)) mtype = stereoon;
        else if (strstr(message.c_str(), msgTypeStr.monoon)) mtype = monoon;
        else if (strstr(message.c_str(), msgTypeStr.mlevel)) mtype = mlevel;
        else if (strstr(message.c_str(), msgTypeStr.fader)) mtype = fader;
        else if (strstr(message.c_str(), msgTypeStr.pan)) mtype = pan;
        else if (strstr(message.c_str(), msgTypeStr.on)) mtype = on;
        else if (strstr(message.c_str(), msgTypeStr.solo)) mtype = solo;
        else if (strstr(message.c_str(), msgTypeStr.keysolo)) mtype = keysolo;
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
        else if (strstr(message.c_str(), msgTypeStr.sendlevel)) mtype = sendlevel;
        else if (strstr(message.c_str(), msgTypeStr.sendpan)) mtype = sendpan;
        else if (strstr(message.c_str(), msgTypeStr.sendpanfollow)) mtype = sendpanfollow;
        else if (strstr(message.c_str(), msgTypeStr.sendtype)) mtype = sendtype;
        else if (strstr(message.c_str(), msgTypeStr.sendon)) mtype = sendon;
    } else {
        if (strstr(message.c_str(), msgTypeStr.dcaon)) mtype = on;
        else if (strstr(message.c_str(), msgTypeStr.configicon)) mtype = configicon;
        else if (strstr(message.c_str(), msgTypeStr.configcolor)) mtype = configcolor;
        else if (strstr(message.c_str(), msgTypeStr.configname)) mtype = configname;
        else if (strstr(message.c_str(), msgTypeStr.dca)) mtype = fader;
    }
    return mtype;
}

ChannelType MixFaceLibrary::getChannelType(std::string message) {
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

int MixFaceLibrary::getChannelNumber(std::string message) {
    debug->sendMessage("MixFaceLibrary::getChannelNumber in message: " + message, 3);
    int chN = 0;
    if (strstr(message.c_str(), busTypeStr.ch)) {
        message.erase(0,strlen(busTypeStr.ch));
        debug->sendMessage("MixFaceLibrary::getChannelNumber erase symbols: " + std::to_string(strlen(busTypeStr.ch)), 3);
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
    debug->sendMessage("MixFaceLibrary::getChannelNumber ch number is: " + std::to_string(chN), 3);
    return chN;
}

int MixFaceLibrary::getSendNumber(std::string message) {
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
        message.erase(0,strlen("/main/st/mix/"));
        message.resize(2);
        sendN = atoi(message.c_str());
    } else if (strstr(message.c_str(), busTypeStr.mainm)) {
        message.erase(0,strlen("/main/m/mix/"));
        message.resize(2);
        sendN = atoi(message.c_str());
    }
    debug->sendMessage("MixFaceLibrary::getSendNumber send number: " + std::to_string(sendN), 3);
    return sendN;
}
