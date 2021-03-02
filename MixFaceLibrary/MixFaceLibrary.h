#ifndef MIXFACELIBRARY_H
#define MIXFACELIBRARY_H

#include <iostream>
#include <string>
#include <boost/thread.hpp>
#include <boost/signals2.hpp>
#include "Timer.hpp"
#include "MixFaceLinker.h"
#include "DebugLibrary.hpp"

using namespace std;
typedef boost::signals2::signal<void(int, int, int)> signal_thr_int;
typedef signal_thr_int::slot_type signal_type_thr_int;

enum MessageType {
    stereoon = 0,  // ch/01/mix/st~~~,i~~
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
    channel = 0,
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
    intvalue = 0,
    floatvalue,
    stringvalue,
    boolvalue,
    verror
};

enum SendType {
    st_level = 0,
    st_pan,
    st_panfollow,
    st_type,
    st_on,
    st_error
};

class MixFaceLinker;

class MixFaceLibrary {
public:
    signal_thr_int valueChanged;

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
        string configname[80];
    };
    x32db db;

    struct MessageTypeStruct {
        const char *stereoon = ("/mix/st");
        const char *monoon = ("/mono");
        const char *mlevel = ("/mlevel");
        const char *fader = ("/fader");
        const char *pan = ("/mix/pan");
        const char *on = ("/mix/on");
        const char *solo = ("/-stat/solosw/");

        const char *chlink = ("/config/chlink/");
        const char *auxlink = ("/config/auxlink/");
        const char *buslink = ("/config/buslink/");
        const char *mtxlink = ("/config/mtxlink/");

        const char *phantom = ("/phantom");
        const char *invert = ("/preamp/invert");
        const char *source = ("/config/source");
        const char *gain = ("/gain");
        const char *trim = ("/trim");

        const char *hpf = ("/hpf");
        const char *hpon = ("/hpon");
        const char *delayon = ("/delay/on");
        const char *delaytime = ("/delay/time");
        const char *inserton = ("/insert/on");
        const char *insertsel = ("/insert/sel");
        const char *insertpos = ("/insert/pos");

        const char *gateon = ("/gate/on");
        const char *gatethr = ("/gate/thr");
        const char *gaterange = ("/gate/range");
        const char *gatemode = ("/gate/mode");
        const char *gateattack = ("/gate/attack");
        const char *gatehold = ("/gate/hold");
        const char *gaterelease = ("/gate/release");
        const char *gatekeysrc = ("/gate/keysrc");
        const char *gatefilteron = ("/gate/filter/on");
        const char *gatefiltertype = ("/gate/filter/type");
        const char *gatefilterf = ("/gate/filter/f");

        const char *dynon = ("/dyn/on");
        const char *dynthr = ("/dyn/thr");
        const char *dynratio = ("/dyn/ratio");
        const char *dynmix = ("/dyn/mix");
        const char *dynmgain = ("/dyn/mgain");
        const char *dynattack = ("/dyn/attack");
        const char *dynhold = ("/dyn/hold");
        const char *dynrelease = ("/dyn/release");
        const char *dynmode = ("/dyn/mode");
        const char *dynknee = ("/dyn/knee");
        const char *dynenv = ("/dyn/env");
        const char *dyndet = ("/dyn/det");
        const char *dynauto = ("/dyn/auto");
        const char *dynkeysrc = ("/dyn/keysrc");
        const char *dynfilteron = ("/dyn/filter/on");
        const char *dynfiltertype = ("/dyn/filter/type");
        const char *dynfilterf = ("/dyn/filter/f");

        const char *eq1type = ("/eq/1/type");
        const char *eq1g = ("/eq/1/g");
        const char *eq1f = ("/eq/1/f");
        const char *eq1q = ("/eq/1/q");
        const char *eq2type = ("/eq/2/type");
        const char *eq2g = ("/eq/2/g");
        const char *eq2f = ("/eq/2/f");
        const char *eq2q = ("/eq/2/q");
        const char *eq3type = ("/eq/3/type");
        const char *eq3g = ("/eq/3/g");
        const char *eq3f = ("/eq/3/f");
        const char *eq3q = ("/eq/3/q");
        const char *eq4type = ("/eq/4/type");
        const char *eq4g = ("/eq/4/g");
        const char *eq4f = ("/eq/4/f");
        const char *eq4q = ("/eq/4/q");
        const char *eq5type = ("/eq/5/type");
        const char *eq5g = ("/eq/5/g");
        const char *eq5f = ("/eq/5/f");
        const char *eq5q = ("/eq/5/q");
        const char *eq6type = ("/eq/6type");
        const char *eq6g = ("/eq/6/g");
        const char *eq6f = ("/eq/6/f");
        const char *eq6q = ("/eq/6/q");

        const char *send = ("/mix/");
        const char *sendlevel = ("/level");
        const char *sendpan = ("/pan");
        const char *sendpanfollow = ("/panFollow");
        const char *sendtype = ("/type");
        const char *sendon = ("/on");

        const char *dca = ("/dca/");
        const char *dcaon = ("/on");

        const char *configicon = ("/config/icon");
        const char *configcolor = ("/config/color");
        const char *configname = ("/config/name");
    };
    MessageTypeStruct msgTypeStr;

    struct ChannelTypeStruct{
        const char *channel = ("/ch/");
        const char *auxin = ("/auxin/");
        const char *fxreturn = ("/fxrtn/");
        const char *bus = ("/bus/");
        const char *matrix = ("/mtx/");
        const char *lr = ("/main/st/");
        const char *mc = ("/main/m/");
        const char *dca = ("/dca/");
    };
    ChannelTypeStruct chTypeStr;

    struct ValueTypeStruct{
        const char *intvalue = "%I";
        const char *floatvalue = "%F";
        const char *stringvalue = "%S";
        const char *boolvalue = "%B";
    };
    ValueTypeStruct valTypeStr;

    struct BusTypeStruct {
        const char *ch = ("%A/ch/");
        const char *auxin = ("%A/auxin/");
        const char *fxrtn = ("%A/fxrtn/");
        const char *bus = ("%A/bus/");
        const char *mtx = ("%A/mtx/");
        const char *mainst = ("%A/main/st/");
        const char *mainm = ("%A/main/m/");
        const char *dca = ("%A/dca/");
        const char *headamp = ("%A/headamp/");
        const char *statsolosw = ("%A/-stat/solosw/");

        const char *mix = ("/mix/");
    };
    BusTypeStruct busTypeStr;

    MixFaceLibrary(DebugLibrary *debug_ = nullptr);

    bool connectTo(string hostNameString);
    void sendSyncMessages();
    void sendXremoteMessage();

    string channelNameFromIdx(int idx);

    void processMessage(string message);

    string getOscAddress(MessageType mtype, ChannelType chtype, int channelN,
                          int sendN);
    ChannelType getChannelTypeFromIdx(int idx);
    int getChannelNumberFromIdx(int idx);
    MessageType getMessageType(string message);
    ChannelType getChannelType(string message);
    ValueType getValueType(string message);
    int getChannelNumber(string message);
    int getSendNumber(string message);
    float getFloatValue(string message);
    int getIntValue(string message);
    string getStringValue(string message);
    int getIdxFromChNandChType(int chN, ChannelType chtype);

    MixFaceLinker *linker;

private:
    DebugLibrary *debug;
    IntervalThread *sendRenewMessagesTimer;
    bool connected = false;

};

#endif // MIXFACELIBRARY_H
