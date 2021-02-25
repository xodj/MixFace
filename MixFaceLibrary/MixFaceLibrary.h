
#ifndef MIXFACELIBRARY_H
#define MIXFACELIBRARY_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include "MixFaceLinker.h"
#include "DebugLibrary.h"

enum MessageType {
    stereoon,  // ch/01/mix/st~~~,i~~
    monoon,    // ch/01/mix/mono~~~,i~~
    mlevel,    // ch/01/mix/mlevel~~~,f~~
    fader,     // ch/01/mix/fader~~~~,f~~
    pan,       // ch/01/mix/pan~~,f~~
    on,      // ch/01/mix/on~~~,i~~ 0 muted 1 on
    solo,      //-stat/solosw/01~~~~,i~~ 1 on 0 off

    chlink,    // config/chlink/1-2 1 on 0 off
    auxlink,
    buslink,
    mtxlink,

    phantom,   // headamp/000/phantom~~~~,i~~
    invert,    // ch/01/preamp/invert~~~~,i~~
    source,    // ch/01/config/source~~~~,i~~
    gain,      // headamp/000/gain~~~,f~~
    trim,      // ch/01/preamp/trim~~,f~~ USED FOR AUXES IF CONFIGURED

    hpf,       // ch/01/preamp/hpf~~~,f~~ 20Hz-400Hz (0-1)
    hpon,      // ch/01/preamp/hpon~~,i~~ on-off HPF
    delayon,   // ch/01/delay/on~,i~~
    delaytime, // ch/01/delay/time~~~,f~~
    inserton,  // ch/01/insert/on~~~~,i~~
    insertsel, // ch/01/insert/sel~~~,i~~
    insertpos, // ch/01/insert/pos~~~,i~~

    gateon,    // ch/01/gate/on~~,i~~
    gatethr,   // ch/01/gate/thr~,f~~
    gaterange, // ch/01/gate/range~~~,f~~
    gatemode,  // ch/01/gate/mode~~~~,i~~ 0-exp 2:1 1-exp 3:1 2-exp 4:1 3-Gate 4-Ducker
    gateattack,     // ch/01/gate/attack~~,f~~
    gatehold,       // ch/01/gate/hold~~~~,f~~
    gaterelease,    // ch/01/gate/release~,f~~
    gatekeysrc,     // ch/01/gate/keysrc~~,i~~ sidechain 0-self 1-64-idx
    gatefilteron,   // ch/01/gate/filter/on~~~,i~~
    gatefiltertype, // ch/01/gate/filter/type~,i~~ 0-8
    gatefilterf,    // ch/01/gate/filter/f~~~~,f~~

    dynon,          // ch/01/dyn/on~~~,i~~
    dynthr,         // ch/01/dyn/thr~~,f~~
    dynratio,       // ch/01/dyn/ratio~~~~,i~~ 0-11 (1.1-100)
    dynmix,         // ch/01/dyn/mix~~,f~~
    dynmgain,       // ch/01/dyn/mgain~~~~,f~~
    dynattack,      // ch/01/dyn/attack~~~,f~~
    dynhold,        // ch/01/dyn/hold~,f~~
    dynrelease,     // ch/01/dyn/release~~,f~~
    dynmode,        // ch/01/dyn/mode~,i~~ 0-Comp 1-Exp
    dynknee,        // ch/01/dyn/knee~,f~~ STEPS 0.2000 (6-steps)
    dynenv,         // ch/01/dyn/env~~,i~~ 0-Lin 1-Log
    dyndet,         // ch/01/dyn/det~~,i~~ 0-Peak 1-RMS
    dynauto,        // ch/01/dyn/auto~,i~~ autotime
    dynkeysrc,      // ch/01/dyn/keysrc~~~,i~~ sidechain 0-self 1-64-idx
    dynfilteron,    // ch/01/dyn/filter/on~~~~,i~~
    dynfiltertype,  // ch/01/dyn/filter/type~~,i~~ 0-8
    dynfilterf,     // ch/01/dyn/filter/f~,f~~ 20Hz-20000kHz

    eq1type, // ch/01/eq/1/type~~~~,i~~ 0-5 0-LCut 1-LShv 2-PEQ 3-VEQ 4-HShv 5-HCut
    eq1g,    // ch/01/eq/1/g~~~,f~~ -15db - +15db 0.5-Zero
    eq1f,    // ch/01/eq/1/f~~~,f~~ freq 20Hz-20kHz
    eq1q,    // ch/01/eq/1/q~~~,f~~ quality 1-0.3 0-10
    eq2type,
    eq2g,
    eq2f,
    eq2q,
    eq3type,
    eq3g,
    eq3f,
    eq3q,
    eq4type,
    eq4g,
    eq4f,
    eq4q,
    eq5type, // 5-6 only for bus,mtx,lr and mc
    eq5g,    // mtx, lr and mc enable for 13 types by eq1 and eq6
    eq5f,    // if use disables eq2 for eq1 and eq5 for eq6
    eq5q,
    eq6type,
    eq6g,
    eq6f,
    eq6q,

    sendlevel,     // ch/01/mix/01/level~,f~~
    sendpan,       // ch/01/mix/01/pan~~~,f~~
    sendpanfollow, // ch/01/mix/01/panFollow~,i~~
    sendtype,      // ch/01/mix/01/type~~,i~~ 0-5 0-Input 1-PreEQ 2-PostEQ 3-PreFader 4-PostFader 5-SubGroupp
    sendon, // ch/01/mix/01/on~~~~,i~~

    configicon, //ch/01/config/icon i 1-74
    configcolor,
    configname,
    merror
};

enum ChannelType {
    channel,
    auxin,
    fxreturn,
    bus,
    matrix,
    lr,
    mc,
    dca,
    headamp,
    cherror
};

enum ValueType {
    intvalue,
    floatvalue,
    stringvalue,
    boolvalue,
    verror
};

class MixFaceLinker;

class MixFaceLibrary : public QObject
{
    Q_OBJECT
public:
    struct x32db {
        int stereoon[80];
        int monoon[80];
        float mlevel[80];
        float fader[80];
        float pan[80];
        int on[80];
        int solo[80];

        int chlink[80];
        int auxlink[80];
        int buslink[80];
        int mtxlink[80];

        int phantom[80];
        int invert[80];
        int source[80];
        float gain[80];
        float trim[80];

        float hpf[80];
        int hpon[80];
        int delayon[80];
        float delaytime[80];
        int inserton[80];
        int insertsel[80];
        int insertpos[80];

        int gateon[80];
        float gatethr[80];
        float gaterange[80];
        int gatemode[80];
        float gateattack[80];
        float gatehold[80];
        float gaterelease[80];
        int gatekeysrc[80];
        int gatefilteron[80];
        int gatefiltertype[80];
        float gatefilterf[80];

        int dynon[80];
        float dynthr[80];
        int dynratio[80];
        float dynmix[80];
        float dynmgain[80];
        float dynattack[80];
        float dynhold[80];
        float dynrelease[80];
        int dynmode[80];
        float dynknee[80];
        int dynenv[80];
        int dyndet[80];
        int dynauto[80];
        int dynkeysrc[80];
        int dynfilteron[80];
        int dynfiltertype[80];
        float dynfilterf[80];

        int eq1type[80];
        float eq1g[80];
        float eq1f[80];
        float eq1q[80];
        int eq2type[80];
        float eq2g[80];
        float eq2f[80];
        float eq2q[80];
        int eq3type[80];
        float eq3g[80];
        float eq3f[80];
        float eq3q[80];
        int eq4type[80];
        float eq4g[80];
        float eq4f[80];
        float eq4q[80];
        int eq5type[80];
        float eq5g[80];
        float eq5f[80];
        float eq5q[80];
        int eq6type[80];
        float eq6g[80];
        float eq6f[80];
        float eq6q[80];

        float sendlevel[80][16];
        float sendpan[80][16];
        int sendpanfollow[80][16];
        int sendtype[80][16];
        int sendon[80][16];

        int configicon[80];
        int configcolor[80];
        QString configname[80];
    };
    x32db db;

    struct MessageTypeQRegExp {
        QRegExp stereoon = QRegExp("/mix/st");
        QRegExp monoon = QRegExp("/mono");
        QRegExp mlevel = QRegExp("/mlevel");
        QRegExp fader = QRegExp("/fader");
        QRegExp pan = QRegExp("/mix/pan");
        QRegExp on = QRegExp("/mix/on");
        QRegExp solo = QRegExp("/-stat/solosw/");

        QRegExp chlink = QRegExp("/config/chlink/");
        QRegExp auxlink = QRegExp("/config/auxlink/");
        QRegExp buslink = QRegExp("/config/buslink/");
        QRegExp mtxlink = QRegExp("/config/mtxlink/");

        QRegExp phantom = QRegExp("/phantom");
        QRegExp invert = QRegExp("/preamp/invert");
        QRegExp source = QRegExp("/config/source");
        QRegExp gain = QRegExp("/gain");
        QRegExp trim = QRegExp("/trim");

        QRegExp hpf = QRegExp("/hpf");
        QRegExp hpon = QRegExp("/hpon");
        QRegExp delayon = QRegExp("/delay/on");
        QRegExp delaytime = QRegExp("/delay/time");
        QRegExp inserton = QRegExp("/insert/on");
        QRegExp insertsel = QRegExp("/insert/sel");
        QRegExp insertpos = QRegExp("/insert/pos");

        QRegExp gateon = QRegExp("/gate/on");
        QRegExp gatethr = QRegExp("/gate/thr");
        QRegExp gaterange = QRegExp("/gate/range");
        QRegExp gatemode = QRegExp("/gate/mode");
        QRegExp gateattack = QRegExp("/gate/attack");
        QRegExp gatehold = QRegExp("/gate/hold");
        QRegExp gaterelease = QRegExp("/gate/release");
        QRegExp gatekeysrc = QRegExp("/gate/keysrc");
        QRegExp gatefilteron = QRegExp("/gate/filter/on");
        QRegExp gatefiltertype = QRegExp("/gate/filter/type");
        QRegExp gatefilterf = QRegExp("/gate/filter/f");

        QRegExp dynon = QRegExp("/dyn/on");
        QRegExp dynthr = QRegExp("/dyn/thr");
        QRegExp dynratio = QRegExp("/dyn/ratio");
        QRegExp dynmix = QRegExp("/dyn/mix");
        QRegExp dynmgain = QRegExp("/dyn/mgain");
        QRegExp dynattack = QRegExp("/dyn/attack");
        QRegExp dynhold = QRegExp("/dyn/hold");
        QRegExp dynrelease = QRegExp("/dyn/release");
        QRegExp dynmode = QRegExp("/dyn/mode");
        QRegExp dynknee = QRegExp("/dyn/knee");
        QRegExp dynenv = QRegExp("/dyn/env");
        QRegExp dyndet = QRegExp("/dyn/det");
        QRegExp dynauto = QRegExp("/dyn/auto");
        QRegExp dynkeysrc = QRegExp("/dyn/keysrc");
        QRegExp dynfilteron = QRegExp("/dyn/filter/on");
        QRegExp dynfiltertype = QRegExp("/dyn/filter/type");
        QRegExp dynfilterf = QRegExp("/dyn/filter/f");

        QRegExp eq1type = QRegExp("/eq/1/type");
        QRegExp eq1g = QRegExp("/eq/1/g");
        QRegExp eq1f = QRegExp("/eq/1/f");
        QRegExp eq1q = QRegExp("/eq/1/q");
        QRegExp eq2type = QRegExp("/eq/2/type");
        QRegExp eq2g = QRegExp("/eq/2/g");
        QRegExp eq2f = QRegExp("/eq/2/f");
        QRegExp eq2q = QRegExp("/eq/2/q");
        QRegExp eq3type = QRegExp("/eq/3/type");
        QRegExp eq3g = QRegExp("/eq/3/g");
        QRegExp eq3f = QRegExp("/eq/3/f");
        QRegExp eq3q = QRegExp("/eq/3/q");
        QRegExp eq4type = QRegExp("/eq/4/type");
        QRegExp eq4g = QRegExp("/eq/4/g");
        QRegExp eq4f = QRegExp("/eq/4/f");
        QRegExp eq4q = QRegExp("/eq/4/q");
        QRegExp eq5type = QRegExp("/eq/5/type");
        QRegExp eq5g = QRegExp("/eq/5/g");
        QRegExp eq5f = QRegExp("/eq/5/f");
        QRegExp eq5q = QRegExp("/eq/5/q");
        QRegExp eq6type = QRegExp("/eq/6type");
        QRegExp eq6g = QRegExp("/eq/6/g");
        QRegExp eq6f = QRegExp("/eq/6/f");
        QRegExp eq6q = QRegExp("/eq/6/q");

        QRegExp sendlevel = QRegExp("/mix/.*/level.*");
        QRegExp sendpan = QRegExp("/mix/.*/pan.*");
        QRegExp sendpanfollow = QRegExp("/mix/.*/panFollow.*");
        QRegExp sendtype = QRegExp("/mix/.*/type.*");
        QRegExp sendon = QRegExp("/mix/.*/on.*");

        QRegExp dcaon = QRegExp("/dca/.*/on.*");

        QRegExp configicon = QRegExp("/config/icon");
        QRegExp configcolor = QRegExp("/config/color");
        QRegExp configname = QRegExp("/config/name");
    };
    MessageTypeQRegExp mtqre;

    struct ChannelTypeQRegExp{
        QRegExp channel = QRegExp("/ch/");
        QRegExp auxin = QRegExp("/auxin/");
        QRegExp fxreturn = QRegExp("/fxrtn/");
        QRegExp bus = QRegExp("/bus/");
        QRegExp matrix = QRegExp("/mtx/");
        QRegExp lr = QRegExp("/main/st/");
        QRegExp mc = QRegExp("/main/m/");
        QRegExp dca = QRegExp("/dca/");
    };
    ChannelTypeQRegExp chtqre;

    struct ValueTypeQRegExp{
        QRegExp intvalue = QRegExp("%I");
        QRegExp floatvalue = QRegExp("%F");
        QRegExp stringvalue = QRegExp("%S");
        QRegExp boolvalue = QRegExp("%B");
    };
    ValueTypeQRegExp vtqre;

    explicit MixFaceLibrary(QObject *parent = nullptr, DebugLibrary *debug_ = nullptr);

    bool connectTo(QString hostNameString);
    void sendSyncMessages();
    void sendXremoteMessage();

    QString channelNameFromIdx(int idx);

    void processMessage(std::string message_);

    QString getOscAddress(MessageType mtype, ChannelType chtype, int channelN,
                          int sendN);
    ChannelType getChannelTypeFromIdx(int idx);
    int getChannelNumberFromIdx(int idx);
    MessageType getMessageType(QString message);
    ChannelType getChannelType(QString message);
    ValueType getValueType(QString message);
    int getChannelNumber(QString message);
    int getSendNumber(QString message);
    float getFloatValue(QString message);
    int getIntValue(QString message);
    QString getStringValue(QString message);
    int getIdxFromChNandChType(int chN, ChannelType chtype);

    MixFaceLinker *linker;

private:
    DebugLibrary *debug;
    void demoThread();

    QThread *thread;
    QTimer *sendRenewMessagesTimer;

    bool connected = false;
    bool demo = false;

signals:
    void valueChanged(MessageType mtype, int idx, int idy);
    void processMeter6(float preL_, float preR_, float gate, float comp);

};

#endif // MIXFACELIBRARY_H
