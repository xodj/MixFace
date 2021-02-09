#include "MixFaceLibrary.h"

MixFaceLibrary::MixFaceLibrary(QObject *parent, DebugLibrary *debug_) : QObject(parent){
    debug = debug_;
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
    keeper = new MixFaceKeeper;
    QThread *keeperThread = new QThread;
    keeper->moveToThread(keeperThread);
    connect(keeperThread, SIGNAL(started()), keeper, SLOT(open()));
    keeperThread->start();

    linker = new MixFaceLinker;
    thread = new QThread;
    linker->moveToThread(thread);
    connect(thread, SIGNAL(started()), linker, SLOT(processMessages()));
    sendRenewMessagesTimer = new QTimer(this);
    QTimer::connect(sendRenewMessagesTimer, &QTimer::timeout, this, &MixFaceLibrary::sendXremoteMessage);
    connect(linker, &MixFaceLinker::debug, debug, &DebugLibrary::sendMessage);
}

bool MixFaceLibrary::connectTo(QString hostNameString){

    thread->start();
    connected = linker->connectTo(hostNameString);
    if (connected) {
        sendRenewMessagesTimer->start(1000);
        connect(linker, &MixFaceLinker::newMessage, this, &MixFaceLibrary::processMessage);

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
        disconnect(linker, &MixFaceLinker::newMessage, this, &MixFaceLibrary::processMessage);
    }
    return connected;
}

void MixFaceLibrary::sendSyncMessages() {
    for (int idx=0;idx<80;idx++){
        QThread::msleep(1);
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);

        MessageType mtype = fader;
        QString msg = getOscAddress(mtype, chtype, chN, 0);
        QByteArray oscAddressArray = msg.toLatin1();
        const char *oscAddress = oscAddressArray;
        linker->sendCmdOnly(oscAddress);

        if (idx<64||idx==70){
            mtype = pan;
            msg = getOscAddress(mtype, chtype, chN, 0);
            oscAddressArray = msg.toLatin1();
            oscAddress = oscAddressArray;
            linker->sendCmdOnly(oscAddress);
        }

        mtype = solo;
        msg = getOscAddress(mtype, chtype, idx+1, 0);
        oscAddressArray = msg.toLatin1();
        oscAddress = oscAddressArray;
        linker->sendCmdOnly(oscAddress);

        mtype = on;
        msg = getOscAddress(mtype, chtype, chN, 0);
        oscAddressArray = msg.toLatin1();
        oscAddress = oscAddressArray;
        linker->sendCmdOnly(oscAddress);
        if(idx<48){
            for(int sendN=1;sendN<17;sendN++){
                mtype = sendlevel;
                msg = getOscAddress(mtype, chtype, chN, sendN);
                oscAddressArray = msg.toLatin1();
                oscAddress = oscAddressArray;
                linker->sendCmdOnly(oscAddress);
            }
        }
        if((idx>47&&idx<64)||idx==70||idx==71){
            for(int sendN=1;sendN<7;sendN++){
                mtype = sendlevel;
                msg = getOscAddress(mtype, chtype, chN, sendN);
                oscAddressArray = msg.toLatin1();
                oscAddress = oscAddressArray;
                linker->sendCmdOnly(oscAddress);
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

QString MixFaceLibrary::channelNameFromIdx(int idx) {
  QString name;
  idx = idx + 1;
  if (idx < 33) {
    name = QString::number(idx);
    name = ("Ch " + name);
  } else if (idx < 41) {
    name = QString::number(idx - 32);
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
    name = QString::number(idx - 48);
    name = ("Bus " + name);
  } else if (idx < 71) {
    name = QString::number(idx - 64);
    name = ("Matrix " + name);
  } else if (idx == 71) {
    name = ("Main LR");
  } else if (idx == 72) {
    name = ("M/C");
  } else if (idx < 81) {
    name = QString::number(idx - 72);
    name = ("DCA " + name);
  }
  return name;
}

void MixFaceLibrary::processMessage(QString message) {
    MessageType mtype = getMessageType(message);
    ValueType vtype = getValueType(message);

    int chN = getChannelNumber(message);
    ChannelType chtype = getChannelType(message);
    int idx = getIdxFromChNandChType(chN, chtype);
    int sendN = getSendNumber(message);
    float fval = 0.0;
    int ival = 0;
    QString sval;

    if (vtype == floatvalue) fval = getFloatValue(message);
    else if (vtype == intvalue) ival = getIntValue(message);
    else if (vtype == stringvalue) sval = getStringValue(message);

    switch (mtype) {
    case stereoon:
        db.stereoon[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case monoon:
        db.monoon[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case mlevel:
        db.mlevel[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case fader:
        db.fader[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case pan:
        db.pan[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case on:
        db.on[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case solo:
        db.solo[chN-1] = ival;
        emit valueChanged(mtype,chN-1,0);
        break;
    case chlink:
        db.chlink[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case auxlink:
        db.auxlink[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case buslink:
        db.buslink[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case mtxlink:
        db.mtxlink[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case phantom:
        db.phantom[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case invert:
        db.invert[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case source:
        db.source[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case gain:
        db.gain[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case trim:
        db.trim[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case hpf:
        db.hpf[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case hpon:
        db.hpon[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case delayon:
        db.delayon[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case delaytime:
        db.delaytime[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case inserton:
        db.inserton[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case insertsel:
        db.insertsel[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case insertpos:
        db.insertpos[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case gateon:
        db.gateon[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case gatethr:
        db.gatethr[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case gaterange:
        db.gaterange[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case gatemode:
        db.gatemode[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case gateattack:
        db.gateattack[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case gatehold:
        db.gatehold[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case gaterelease:
        db.gaterelease[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case gatekeysrc:
        db.gatekeysrc[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case gatefilteron:
        db.gatefilteron[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case gatefiltertype:
        db.gatefiltertype[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case gatefilterf:
        db.gatefilterf[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case dynon:
        db.dynon[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case dynthr:
        db.dynthr[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case dynratio:
        db.dynratio[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case dynmix:
        db.dynmix[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case dynmgain:
        db.dynmgain[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case dynattack:
        db.dynattack[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case dynhold:
        db.dynhold[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case dynrelease:
        db.dynrelease[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case dynmode:
        db.dynmode[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case dynknee:
        db.dynknee[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case dynenv:
        db.dynenv[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case dyndet:
        db.dyndet[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case dynauto:
        db.dynauto[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case dynkeysrc:
        db.dynkeysrc[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case dynfilteron:
        db.dynfilteron[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case dynfiltertype:
        db.dynfiltertype[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case dynfilterf:
        db.dynfilterf[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case eq1type:
        db.eq1type[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case eq1g:
        db.eq1g[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case eq1f:
        db.eq1f[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case eq1q:
        db.eq1q[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case eq2type:
        db.eq2type[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case eq2g:
        db.eq2g[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case eq2f:
        db.eq2f[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case eq2q:
        db.eq2q[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case eq3type:
        db.eq3type[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case eq3g:
        db.eq3g[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case eq3f:
        db.eq3f[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case eq3q:
        db.eq3q[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case eq4type:
        db.eq4type[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case eq4g:
        db.eq4g[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case eq4f:
        db.eq4f[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case eq4q:
        db.eq4q[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case eq5type:
        db.eq5type[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case eq5g:
        db.eq5g[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case eq5f:
        db.eq5f[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case eq5q:
        db.eq5q[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case eq6type:
        db.eq6type[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case eq6g:
        db.eq6g[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case eq6f:
        db.eq6f[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case eq6q:
        db.eq6q[idx] = fval;
        emit valueChanged(mtype,idx,0);
        break;
    case sendlevel:
        db.sendlevel[idx][sendN] = fval;
        emit valueChanged(mtype,idx,sendN);
        break;
    case sendpan:
        db.sendpan[idx][sendN] = fval;
        emit valueChanged(mtype,idx,sendN);
        break;
    case sendpanfollow:
        db.sendpanfollow[idx][sendN] = ival;
        emit valueChanged(mtype,idx,sendN);
        break;
    case sendtype:
        db.sendtype[idx][sendN] = ival;
        emit valueChanged(mtype,idx,sendN);
        break;
    case sendon:
        db.sendon[idx][sendN] = ival;
        emit valueChanged(mtype,idx,sendN);
        break;
    case configicon:
        db.configicon[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case configcolor:
        db.configcolor[idx] = ival;
        emit valueChanged(mtype,idx,0);
        break;
    case configname:
        db.configname[idx] = sval;
        emit valueChanged(mtype,idx,0);
        break;
    case merror:
        debug->sendMessage("Error message read: " + message,0);
        break;
    }
}

QString MixFaceLibrary::getOscAddress(MessageType mtype,
                                   ChannelType chtype,
                                   int channelN, int sendN)
{
    QString oscAddress;
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
            oscAddress = ("/-stat/solosw/0" + QString::number(channelN));
        else
            oscAddress = ("/-stat/solosw/" + QString::number(channelN));
        break;
    case monoon:
        oscAddress = ("/mix/mono");
        break;
    case mlevel:
        oscAddress = ("/mix/mlevel");
        break;
    case chlink:
        if (channelN % 2 == 0) channelN = channelN-1;
        oscAddress = ("/config/chlink/" + QString::number(channelN) + "-" +
                      QString::number(channelN + 1));
        break;
    case auxlink:
        if (channelN % 2 == 0) channelN = channelN-1;
        oscAddress = ("/config/auxlink/" + QString::number(channelN) + "-" +
                      QString::number(channelN + 1));
        break;
    case buslink:
        if (channelN % 2 == 0) channelN = channelN-1;
        oscAddress = ("/config/buslink/" + QString::number(channelN) + "-" +
                      QString::number(channelN + 1));
        break;
    case mtxlink:
        if (channelN % 2 == 0) channelN = channelN-1;
        oscAddress = ("/config/mtxlink/" + QString::number(channelN) + "-" +
                      QString::number(channelN + 1));
        break;
    case phantom:
        if (channelN < 10)
            oscAddress = ("/headamp/00" + QString::number(channelN) + "/phantom");
        else if (channelN < 100)
            oscAddress = ("/headamp/0" + QString::number(channelN) + "/phantom");
        else
            oscAddress = ("/headamp/" + QString::number(channelN) + "/phantom");
        break;
    case invert:
        oscAddress = ("/preamp/invert");
        break;
    case source:
        oscAddress = ("/config/source");
        break;
    case gain:
        if (channelN < 10)
            oscAddress = ("/headamp/00" + QString::number(channelN-1) + "/gain");
        else if (channelN < 100)
            oscAddress = ("/headamp/0" + QString::number(channelN-1) + "/gain");
        else
            oscAddress = ("/headamp/" + QString::number(channelN-1) + "/gain");
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
            oscAddress = ("/mix/0" + QString::number(sendN) + "/level");
        else
            oscAddress = ("/mix/" + QString::number(sendN) + "/level");
        break;
    case sendpan:
        if (sendN < 10)
            oscAddress = ("/mix/0" + QString::number(sendN) + "/pan");
        else
            oscAddress = ("/mix/" + QString::number(sendN) + "/pan");
        break;
    case sendpanfollow:
        if (sendN < 10)
            oscAddress = ("/mix/0" + QString::number(sendN) + "/panFollow");
        else
            oscAddress = ("/mix/" + QString::number(sendN) + "/panFollow");
        break;
    case sendtype:
        if (sendN < 10)
            oscAddress = ("/mix/0" + QString::number(sendN) + "/type");
        else
            oscAddress = ("/mix/" + QString::number(sendN) + "/type");
        break;
    case sendon:
        if (sendN < 10)
            oscAddress = ("/mix/0" + QString::number(sendN) + "/on");
        else
            oscAddress = ("/mix/" + QString::number(sendN) + "/on");
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
        debug->sendMessage("Message type error " + QString::number(channelN),0);
        oscAddress = nullptr;
        break;
    }
    if (mtype != solo && mtype != chlink && mtype != auxlink &&
            mtype != buslink && mtype != mtxlink && mtype != phantom &&
            mtype != gain) {
        switch (chtype) {
        case channel:
            if (channelN < 10) {
                oscAddress = ("/ch/0" + QString::number(channelN) + oscAddress);
            } else {
                oscAddress = ("/ch/" + QString::number(channelN) + oscAddress);
            }
            break;
        case auxin:
            oscAddress = ("/auxin/0" + QString::number(channelN) + oscAddress);
            break;
        case fxreturn:
            oscAddress = ("/fxrtn/0" + QString::number(channelN) + oscAddress);
            break;
        case bus:
            if (channelN < 10) {
                oscAddress = ("/bus/0" + QString::number(channelN) + oscAddress);
            } else {
                oscAddress = ("/bus/" + QString::number(channelN) + oscAddress);
            }
            break;
        case matrix:
            oscAddress = ("/mtx/0" + QString::number(channelN) + oscAddress);
            break;
        case lr:
            oscAddress = ("/main/st" + oscAddress);
            break;
        case mc:
            oscAddress = ("/main/m" + oscAddress);
            break;
        case dca:
            if (mtype == fader)
                oscAddress = ("/dca/" + QString::number(channelN) + "/fader");
            else if (mtype == on)
                oscAddress = ("/dca/" + QString::number(channelN) + "/on");
            break;
        case headamp:
            break;
        case cherror:
            debug->sendMessage("Channel type error " + QString::number(channelN),0);
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

MessageType MixFaceLibrary::getMessageType(QString message) {
    MessageType mtype = merror;
    if (message.contains(mtqre.stereoon)) mtype = stereoon;
    else if (message.contains(mtqre.monoon)) mtype = monoon;
    else if (message.contains(mtqre.mlevel)) mtype = mlevel;
    else if (message.contains(mtqre.fader)) mtype = fader;
    else if (message.contains(mtqre.pan)) mtype = pan;
    else if (message.contains(mtqre.on)) mtype = on;
    else if (message.contains(mtqre.solo)) mtype = solo;
    else if (message.contains(mtqre.sendlevel)) mtype = sendlevel;
    else if (message.contains(mtqre.sendpan)) mtype = sendpan;
    else if (message.contains(mtqre.sendpanfollow)) mtype = sendpanfollow;
    else if (message.contains(mtqre.sendtype)) mtype = sendtype;
    else if (message.contains(mtqre.sendon)) mtype = sendon;
    else if (message.contains(mtqre.dcaon)) mtype = on;
    else if (message.contains(mtqre.chlink)) mtype = chlink;
    else if (message.contains(mtqre.auxlink)) mtype = auxlink;
    else if (message.contains(mtqre.buslink)) mtype = buslink;
    else if (message.contains(mtqre.mtxlink)) mtype = mtxlink;
    else if (message.contains(mtqre.phantom)) mtype = phantom;
    else if (message.contains(mtqre.invert)) mtype = invert;
    else if (message.contains(mtqre.source)) mtype = source;
    else if (message.contains(mtqre.gain)) mtype = gain;
    else if (message.contains(mtqre.trim)) mtype = trim;
    else if (message.contains(mtqre.hpf)) mtype = hpf;
    else if (message.contains(mtqre.hpon)) mtype = hpon;
    else if (message.contains(mtqre.delayon)) mtype = delayon;
    else if (message.contains(mtqre.delaytime)) mtype = delaytime;
    else if (message.contains(mtqre.inserton)) mtype = inserton;
    else if (message.contains(mtqre.insertsel)) mtype = insertsel;
    else if (message.contains(mtqre.insertpos)) mtype = insertpos;
    else if (message.contains(mtqre.gateon)) mtype = gateon;
    else if (message.contains(mtqre.gatethr)) mtype = gatethr;
    else if (message.contains(mtqre.gaterange)) mtype = gaterange;
    else if (message.contains(mtqre.gatemode)) mtype = gatemode;
    else if (message.contains(mtqre.gateattack)) mtype = gateattack;
    else if (message.contains(mtqre.gatehold)) mtype = gatehold;
    else if (message.contains(mtqre.gaterelease)) mtype = gaterelease;
    else if (message.contains(mtqre.gatekeysrc)) mtype = gatekeysrc;
    else if (message.contains(mtqre.gatefilteron)) mtype = gatefilteron;
    else if (message.contains(mtqre.gatefiltertype)) mtype = gatefiltertype;
    else if (message.contains(mtqre.gatefilterf)) mtype = gatefilterf;
    else if (message.contains(mtqre.dynon)) mtype = dynon;
    else if (message.contains(mtqre.dynthr)) mtype = dynthr;
    else if (message.contains(mtqre.dynratio)) mtype = dynratio;
    else if (message.contains(mtqre.dynmix)) mtype = dynmix;
    else if (message.contains(mtqre.dynmgain)) mtype = dynmgain;
    else if (message.contains(mtqre.dynattack)) mtype = dynattack;
    else if (message.contains(mtqre.dynhold)) mtype = dynhold;
    else if (message.contains(mtqre.dynrelease)) mtype = dynrelease;
    else if (message.contains(mtqre.dynmode)) mtype = dynmode;
    else if (message.contains(mtqre.dynknee)) mtype = dynknee;
    else if (message.contains(mtqre.dynenv)) mtype = dynenv;
    else if (message.contains(mtqre.dyndet)) mtype = dyndet;
    else if (message.contains(mtqre.dynauto)) mtype = dynauto;
    else if (message.contains(mtqre.dynkeysrc)) mtype = dynkeysrc;
    else if (message.contains(mtqre.dynfilteron)) mtype = dynfilteron;
    else if (message.contains(mtqre.dynfiltertype)) mtype = dynfiltertype;
    else if (message.contains(mtqre.dynfilterf)) mtype = dynfilterf;
    else if (message.contains(mtqre.eq1type)) mtype = eq1type;
    else if (message.contains(mtqre.eq1g)) mtype = eq1g;
    else if (message.contains(mtqre.eq1f)) mtype = eq1f;
    else if (message.contains(mtqre.eq1q)) mtype = eq1q;
    else if (message.contains(mtqre.eq2type)) mtype = eq2type;
    else if (message.contains(mtqre.eq2g)) mtype = eq2g;
    else if (message.contains(mtqre.eq2f)) mtype = eq2f;
    else if (message.contains(mtqre.eq2q)) mtype = eq2q;
    else if (message.contains(mtqre.eq3type)) mtype = eq3type;
    else if (message.contains(mtqre.eq3g)) mtype = eq3g;
    else if (message.contains(mtqre.eq3f)) mtype = eq3f;
    else if (message.contains(mtqre.eq3q)) mtype = eq3q;
    else if (message.contains(mtqre.eq4type)) mtype = eq4type;
    else if (message.contains(mtqre.eq4g)) mtype = eq4g;
    else if (message.contains(mtqre.eq4f)) mtype = eq4f;
    else if (message.contains(mtqre.eq4q)) mtype = eq4q;
    else if (message.contains(mtqre.eq5type)) mtype = eq5type;
    else if (message.contains(mtqre.eq5g)) mtype = eq5g;
    else if (message.contains(mtqre.eq5f)) mtype = eq5f;
    else if (message.contains(mtqre.eq5q)) mtype = eq5q;
    else if (message.contains(mtqre.eq6type)) mtype = eq6type;
    else if (message.contains(mtqre.eq6g)) mtype = eq6g;
    else if (message.contains(mtqre.eq6f)) mtype = eq6f;
    else if (message.contains(mtqre.eq6q)) mtype = eq6q;
    else if (message.contains(mtqre.configicon)) mtype = configicon;
    else if (message.contains(mtqre.configcolor)) mtype = configcolor;
    else if (message.contains(mtqre.configname)) mtype = configname;
    return mtype;
}

ChannelType MixFaceLibrary::getChannelType(QString message) {
    ChannelType chtype = cherror;
    if (message.contains(chtqre.channel)) chtype = channel;
    else if (message.contains(chtqre.auxin)) chtype = auxin;
    else if (message.contains(chtqre.fxreturn)) chtype = fxreturn;
    else if (message.contains(chtqre.bus)) chtype = bus;
    else if (message.contains(chtqre.matrix)) chtype = matrix;
    else if (message.contains(chtqre.lr)) chtype = lr;
    else if (message.contains(chtqre.mc)) chtype = mc;
    else if (message.contains(chtqre.dca)) chtype = dca;
    return chtype;
}

ValueType MixFaceLibrary::getValueType(QString message) {
    ValueType vtype = verror;
    if (message.contains(vtqre.intvalue)) vtype = intvalue;
    else if (message.contains(vtqre.floatvalue)) vtype = floatvalue;
    else if (message.contains(vtqre.stringvalue)) vtype = stringvalue;
    else if (message.contains(vtqre.boolvalue)) vtype = boolvalue;
    return vtype;
}

int MixFaceLibrary::getChannelNumber(QString message) {
    int chN = 0;
    if (message.contains(QRegExp("/ch/"))) {
        message.remove("%A/ch/");
        message.resize(2);
        chN = message.toInt();
    } else if (message.contains(QRegExp("/auxin/"))) {
        message.remove("%A/auxin/");
        message.resize(2);
        chN = message.toInt();
    } else if (message.contains(QRegExp("/fxrtn/"))) {
        message.remove("%A/fxrtn/");
        message.resize(2);
        chN = message.toInt();
    } else if (message.contains(QRegExp("/bus/"))) {
        message.remove("%A/bus/");
        message.resize(2);
        chN = message.toInt();
    } else if (message.contains(QRegExp("/mtx/"))) {
        message.remove("%A/mtx/");
        message.resize(2);
        chN = message.toInt();
    } else if (message.contains(QRegExp("/main/st/"))) {
        chN = 0;
    } else if (message.contains(QRegExp("/main/m/"))) {
        chN = 0;
    } else if (message.contains(QRegExp("/dca/"))) {
        message.remove("%A/dca/");
        message.resize(1);
        chN = message.toInt();
    } else if (message.contains(QRegExp("/headamp/"))) {
        message.remove("%A/headamp/");
        message.resize(3);
        chN = message.toInt();
    } else if (message.contains(QRegExp("/-stat/solosw/"))) {
        message.remove("%A/-stat/solosw/");
        message.resize(2);
        chN = message.toInt();
    }
    return chN;
}

int MixFaceLibrary::getSendNumber(QString message) {
    int sendN = 0;
    if (message.contains(QRegExp("/ch/"))) {
        message.remove("%A/ch/");
        message.remove(0, 2);
        message.remove("/mix/");
        message.resize(2);
        sendN = message.toInt();
    } else if (message.contains(QRegExp("/auxin/"))) {
        message.remove("%A/auxin/");
        message.remove(0, 2);
        message.remove("/mix/");
        message.resize(2);
        sendN = message.toInt();
    } else if (message.contains(QRegExp("/fxrtn/"))) {
        message.remove("%A/fxrtn/");
        message.remove(0, 2);
        message.remove("/mix/");
        message.resize(2);
        sendN = message.toInt();
    } else if (message.contains(QRegExp("/bus/"))) {
        message.remove("%A/bus/");
        message.remove(0, 2);
        message.remove("/mix/");
        message.resize(2);
        sendN = message.toInt();
    } else if (message.contains(QRegExp("/main/st/"))) {
        message.remove("%A/main/st/mix/");
        message.resize(2);
        sendN = message.toInt();
    } else if (message.contains(QRegExp("/main/m/"))) {
        message.remove("%A/main/m/mix/");
        message.resize(2);
        sendN = message.toInt();
    }
    return sendN;
}

float MixFaceLibrary::getFloatValue(QString message) {
    float fval = 0;
    QString value = message;
    message.remove(QRegExp("%F.*"));
    value = value.remove(message);
    value.remove("%F");
    fval = value.toFloat();
    return fval;
}

int MixFaceLibrary::getIntValue(QString message) {
    int ival = 0;
    QString value = message;
    message.remove(QRegExp("%I.*"));
    value = value.remove(message);
    value.remove("%I");
    ival = value.toInt();
    return ival;
}

QString MixFaceLibrary::getStringValue(QString message) {
    QString value = message;
    message.remove(QRegExp("%S.*"));
    value = value.remove(message);
    value.remove("%S");
    return value;
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

