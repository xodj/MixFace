#include "MixFaceKeeper.h"
#include "MixFaceStatic.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/property_tree/detail/ptree_implementation.hpp>

MixFaceKeeper::MixFaceKeeper(DebugLibrary *debug)
    : keeperThread(new boost::thread{}), debug(debug), db(new x32db)
{
    debug->sendMessage("MixFaceKeeper::MixFaceKeeper",5);
    for (int idx=0;idx<80;idx++){
        db->stereoon[idx] = 1;
        db->monoon[idx] = 0;
        db->mlevel[idx] = 0;
        db->fader[idx] = 0.75;
        db->pan[idx] = 0.5;
        db->on[idx] = 1;
        db->solo[idx] = 0;

        db->chlink[idx] = 0;
        db->auxlink[idx] = 0;
        db->buslink[idx] = 0;
        db->mtxlink[idx] = 0;

        db->phantom[idx] = 0;
        db->invert[idx] = 0;
        db->source[idx] = 0;
        db->gain[idx] = 0.5;
        db->trim[idx] = 0.5;

        db->hpf[idx] = 0;
        db->hpon[idx] = 0;
        db->delayon[idx] = 0;
        db->delaytime[idx] = 0;
        db->inserton[idx] = 0;
        db->insertsel[idx] = 0;
        db->insertpos[idx] = 0;

        db->gateon[idx] = 0;
        db->gatethr[idx] = 0.f;
        db->gaterange[idx] = 1.f;
        db->gatemode[idx] = 3;
        db->gateattack[idx] = 0.0083f;
        db->gatehold[idx] = 0.8800f;
        db->gaterelease[idx] = 0.7900f;
        db->gatekeysrc[idx] = 0;
        db->gatefilteron[idx] = 0;
        db->gatefiltertype[idx] = 6;
        db->gatefilterf[idx] = 0.5650f;

        db->dynon[idx] = 0;
        db->dynthr[idx] = 1.f;
        db->dynratio[idx] = 5;
        db->dynmix[idx] = 1.f;
        db->dynmgain[idx] = 0;
        db->dynattack[idx] = 0.0834f;
        db->dynhold[idx] = 0.5400f;
        db->dynrelease[idx] = 0.5100f;
        db->dynmode[idx] = 0;
        db->dynknee[idx] = 0.2f;
        db->dynenv[idx] = 1;
        db->dyndet[idx] = 0;
        db->dynauto[idx] = 0;
        db->dynkeysrc[idx] = 0;
        db->dynfilteron[idx] = 0;
        db->dynfiltertype[idx] = 6;
        db->dynfilterf[idx] = 0.5650f;

        db->eq1type[idx] = 1;
        db->eq1g[idx] = 0.5000f;
        db->eq1f[idx] = 0.2000f;
        db->eq1q[idx] = 0.4648f;
        db->eq2type[idx] = 2;
        db->eq2g[idx] = 0.5000f;
        db->eq2f[idx] = 0.3000f;
        db->eq2q[idx] = 0.4648f;
        db->eq3type[idx] = 2;
        db->eq3g[idx] = 0.5000f;
        db->eq3f[idx] = 0.4650f;
        db->eq3q[idx] = 0.4648f;
        db->eq4type[idx] = 2;
        db->eq4g[idx] = 0.5000f;
        db->eq4f[idx] = 0.6650f;
        db->eq4q[idx] = 0.4648f;
        db->eq5type[idx] = 2;
        db->eq5g[idx] = 0.5000f;
        db->eq5f[idx] = 0.8000f;
        db->eq5q[idx] = 0.4648f;
        db->eq6type[idx] = 4;
        db->eq6g[idx] = 0.5000f;
        db->eq6f[idx] = 0.9000f;
        db->eq6q[idx] = 0.4648f;

        for(int idy=0;idy<16;idy++){
            db->sendlevel[idx][idy] = 0;
            db->sendpan[idx][idy] = 0.5;
            db->sendpanfollow[idx][idy] = 0;
            db->sendtype[idx][idy] = 5;
            db->sendon[idx][idy] = 1;
        }

        db->configicon[idx] = 0;
        db->configcolor[idx] = 0;
        db->configname[idx] = channelNameFromIdx(idx);
    }
    loadX32DB();
}

using namespace boost::property_tree;

void MixFaceKeeper::loadX32DB(){
    ifstream configfile("MixFaceConfig.dat");
    if(configfile.is_open()){
        ptree cptree;
        read_info(configfile, cptree);
        boost::optional<int> keysolo = cptree.
                get_optional<int>("stat.keysolo");
        if(keysolo.has_value())
            db->keysolo = keysolo.get();
        for (int idx=0;idx<80;idx++){
            boost::optional<int> stereoon = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".stereoon");
            if(stereoon.has_value())
                db->stereoon[idx] = stereoon.get();
            boost::optional<int> monoon = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".monoon");
            if(monoon.has_value())
                db->monoon[idx] = monoon.get();
            boost::optional<float> mlevel = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".mlevel");
            if(mlevel.has_value())
                db->mlevel[idx] = mlevel.get();
            boost::optional<float> fader = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".fader");
            if(fader.has_value())
                db->fader[idx] = fader.get();
            boost::optional<float> pan = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".pan");
            if(pan.has_value())
                db->pan[idx] = pan.get();
            boost::optional<int> on = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".on");
            if(on.has_value())
                db->on[idx] = on.get();
            boost::optional<int> solo = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".solo");
            if(solo.has_value())
                db->solo[idx] = solo.get();
            boost::optional<int> chlink = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".chlink");
            if(chlink.has_value())
                db->chlink[idx] = chlink.get();
            boost::optional<int> auxlink = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".auxlink");
            if(auxlink.has_value())
                db->auxlink[idx] = auxlink.get();
            boost::optional<int> buslink = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".buslink");
            if(buslink.has_value())
                db->buslink[idx] = buslink.get();
            boost::optional<int> mtxlink = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".mtxlink");
            if(mtxlink.has_value())
                db->mtxlink[idx] = mtxlink.get();
            boost::optional<int> phantom = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".phantom");
            if(phantom.has_value())
                db->phantom[idx] = phantom.get();
            boost::optional<int> invert = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".invert");
            if(invert.has_value())
                db->invert[idx] = invert.get();
            boost::optional<int> source = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".source");
            if(source.has_value())
                db->source[idx] = source.get();
            boost::optional<float> gain = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".gain");
            if(gain.has_value())
                db->gain[idx] = gain.get();
            boost::optional<float> trim = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".trim");
            if(trim.has_value())
                db->trim[idx] = trim.get();
            boost::optional<float> hpf = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".hpf");
            if(hpf.has_value())
                db->hpf[idx] = hpf.get();
            boost::optional<int> hpon = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".hpon");
            if(hpon.has_value())
                db->hpon[idx] = hpon.get();
            boost::optional<int> delayon = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".delayon");
            if(delayon.has_value())
                db->delayon[idx] = delayon.get();
            boost::optional<float> delaytime = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".delaytime");
            if(delaytime.has_value())
                db->delaytime[idx] = delaytime.get();
            boost::optional<int> inserton = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".inserton");
            if(inserton.has_value())
                db->inserton[idx] = inserton.get();
            boost::optional<int> insertsel = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".insertsel");
            if(insertsel.has_value())
                db->insertsel[idx] = insertsel.get();
            boost::optional<int> insertpos = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".insertpos");
            if(insertpos.has_value())
                db->insertpos[idx] = insertpos.get();
            boost::optional<int> gateon = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".gateon");
            if(gateon.has_value())
                db->gateon[idx] = gateon.get();
            boost::optional<float> gatethr = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".gatethr");
            if(gatethr.has_value())
                db->gatethr[idx] = gatethr.get();
            boost::optional<float> gaterange = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".gaterange");
            if(gaterange.has_value())
                db->gaterange[idx] = gaterange.get();
            boost::optional<int> gatemode = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".gatemode");
            if(gatemode.has_value())
                db->gatemode[idx] = gatemode.get();
            boost::optional<float> gateattack = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".gateattack");
            if(gateattack.has_value())
                db->gateattack[idx] = gateattack.get();
            boost::optional<float> gatehold = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".gatehold");
            if(gatehold.has_value())
                db->gatehold[idx] = gatehold.get();
            boost::optional<float> gaterelease = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".gaterelease");
            if(gaterelease.has_value())
                db->gaterelease[idx] = gaterelease.get();
            boost::optional<int> gatekeysrc = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".gatekeysrc");
            if(gatekeysrc.has_value())
                db->gatekeysrc[idx] = gatekeysrc.get();
            boost::optional<int> gatefilteron = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".gatefilteron");
            if(gatefilteron.has_value())
                db->gatefilteron[idx] = gatefilteron.get();
            boost::optional<int> gatefiltertype = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".gatefiltertype");
            if(gatefiltertype.has_value())
                db->gatefiltertype[idx] = gatefiltertype.get();
            boost::optional<float> gatefilterf = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".gatefilterf");
            if(gatefilterf.has_value())
                db->gatefilterf[idx] = gatefilterf.get();
            boost::optional<int> dynon = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".dynon");
            if(dynon.has_value())
                db->dynon[idx] = dynon.get();
            boost::optional<float> dynthr = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".dynthr");
            if(dynthr.has_value())
                db->dynthr[idx] = dynthr.get();
            boost::optional<int> dynratio = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".dynratio");
            if(dynratio.has_value())
                db->dynratio[idx] = dynratio.get();
            boost::optional<float> dynmix = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".dynmix");
            if(dynmix.has_value())
                db->dynmix[idx] = dynmix.get();
            boost::optional<float> dynmgain = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".dynmgain");
            if(dynmgain.has_value())
                db->dynmgain[idx] = dynmgain.get();
            boost::optional<float> dynattack = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".dynattack");
            if(dynattack.has_value())
                db->dynattack[idx] = dynattack.get();
            boost::optional<float> dynhold = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".dynhold");
            if(dynhold.has_value())
                db->dynhold[idx] = dynhold.get();
            boost::optional<float> dynrelease = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".dynrelease");
            if(dynrelease.has_value())
                db->dynrelease[idx] = dynrelease.get();
            boost::optional<int> dynmode = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".dynmode");
            if(dynmode.has_value())
                db->dynmode[idx] = dynmode.get();
            boost::optional<float> dynknee = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".dynknee");
            if(dynknee.has_value())
                db->dynknee[idx] = dynknee.get();
            boost::optional<int> dynenv = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".dynenv");
            if(dynenv.has_value())
                db->dynenv[idx] = dynenv.get();
            boost::optional<int> dyndet = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".dyndet");
            if(dyndet.has_value())
                db->dyndet[idx] = dyndet.get();
            boost::optional<int> dynauto = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".dynauto");
            if(dynauto.has_value())
                db->dynauto[idx] = dynauto.get();
            boost::optional<int> dynkeysrc = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".dynkeysrc");
            if(dynkeysrc.has_value())
                db->dynkeysrc[idx] = dynkeysrc.get();
            boost::optional<int> dynfilteron = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".dynfilteron");
            if(dynfilteron.has_value())
                db->dynfilteron[idx] = dynfilteron.get();
            boost::optional<int> dynfiltertype = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".dynfiltertype");
            if(dynfiltertype.has_value())
                db->dynfiltertype[idx] = dynfiltertype.get();
            boost::optional<float> dynfilterf = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".dynfilterf");
            if(dynfilterf.has_value())
                db->dynfilterf[idx] = dynfilterf.get();
            boost::optional<int> eq1type = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".eq1type");
            if(eq1type.has_value())
                db->eq1type[idx] = eq1type.get();
            boost::optional<float> eq1g = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".eq1g");
            if(eq1g.has_value())
                db->eq1g[idx] = eq1g.get();
            boost::optional<float> eq1f = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".eq1f");
            if(eq1f.has_value())
                db->eq1f[idx] = eq1f.get();
            boost::optional<float> eq1q = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".eq1q");
            if(eq1q.has_value())
                db->eq1q[idx] = eq1q.get();
            boost::optional<int> eq2type = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".eq2type");
            if(eq2type.has_value())
                db->eq2type[idx] = eq2type.get();
            boost::optional<float> eq2g = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".eq2g");
            if(eq2g.has_value())
                db->eq2g[idx] = eq2g.get();
            boost::optional<float> eq2f = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".eq2f");
            if(eq2f.has_value())
                db->eq2f[idx] = eq2f.get();
            boost::optional<float> eq2q = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".eq2q");
            if(eq2q.has_value())
                db->eq2q[idx] = eq2q.get();
            boost::optional<int> eq3type = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".eq3type");
            if(eq3type.has_value())
                db->eq3type[idx] = eq3type.get();
            boost::optional<float> eq3g = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".eq3g");
            if(eq3g.has_value())
                db->eq3g[idx] = eq3g.get();
            boost::optional<float> eq3f = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".eq3f");
            if(eq3f.has_value())
                db->eq3f[idx] = eq3f.get();
            boost::optional<float> eq3q = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".eq3q");
            if(eq3q.has_value())
                db->eq3q[idx] = eq3q.get();
            boost::optional<int> eq4type = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".eq4type");
            if(eq4type.has_value())
                db->eq4type[idx] = eq4type.get();
            boost::optional<float> eq4g = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".eq4g");
            if(eq4g.has_value())
                db->eq4g[idx] = eq4g.get();
            boost::optional<float> eq4f = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".eq4f");
            if(eq4f.has_value())
                db->eq4f[idx] = eq4f.get();
            boost::optional<float> eq4q = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".eq4q");
            if(eq4q.has_value())
                db->eq4q[idx] = eq4q.get();
            boost::optional<int> eq5type = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".eq5type");
            if(eq5type.has_value())
                db->eq5type[idx] = eq5type.get();
            boost::optional<float> eq5g = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".eq5g");
            if(eq5g.has_value())
                db->eq5g[idx] = eq5g.get();
            boost::optional<float> eq5f = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".eq5f");
            if(eq5f.has_value())
                db->eq5f[idx] = eq5f.get();
            boost::optional<float> eq5q = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".eq5q");
            if(eq5q.has_value())
                db->eq5q[idx] = eq5q.get();
            boost::optional<int> eq6type = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".eq6type");
            if(eq6type.has_value())
                db->eq6type[idx] = eq6type.get();
            boost::optional<float> eq6g = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".eq6g");
            if(eq6g.has_value())
                db->eq6g[idx] = eq6g.get();
            boost::optional<float> eq6f = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".eq6f");
            if(eq6f.has_value())
                db->eq6f[idx] = eq6f.get();
            boost::optional<float> eq6q = cptree.
                    get_optional<float>("channel.id" + to_string(idx) + ".eq6q");
            if(eq6q.has_value())
                db->eq6q[idx] = eq6q.get();
            boost::optional<int> configicon = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".configicon");
            if(configicon.has_value())
                db->configicon[idx] = configicon.get();
            boost::optional<int> configcolor = cptree.
                    get_optional<int>("channel.id" + to_string(idx) + ".configcolor");
            if(configcolor.has_value())
                db->configcolor[idx] = configcolor.get();
            boost::optional<std::string> configname = cptree.
                    get_optional<std::string>("channel.id" + to_string(idx) + ".configname");
            if(configname.has_value())
                db->configname[idx] = configname.get();
            for (int idy=0;idy<16;idy++){
                boost::optional<float> sendlevel = cptree.
                        get_optional<float>("channel.id" + to_string(idx)
                                            + ".sendlevel.id" + to_string(idy));
                if(sendlevel.has_value())
                    db->sendlevel[idx][idy] = sendlevel.get();
                boost::optional<float> sendpan = cptree.
                        get_optional<float>("channel.id" + to_string(idx)
                                            + ".sendpan.id" + to_string(idy));
                if(sendpan.has_value())
                    db->sendpan[idx][idy] = sendpan.get();
                boost::optional<float> sendpanfollow = cptree.
                        get_optional<float>("channel.id" + to_string(idx)
                                            + ".sendpanfollow.id" + to_string(idy));
                if(sendpanfollow.has_value())
                    db->sendpanfollow[idx][idy] = sendpanfollow.get();
                boost::optional<float> sendtype = cptree.
                        get_optional<float>("channel.id" + to_string(idx)
                                            + ".sendtype.id" + to_string(idy));
                if(sendtype.has_value())
                    db->sendtype[idx][idy] = sendtype.get();
                boost::optional<float> sendon = cptree.
                        get_optional<float>("channel.id" + to_string(idx)
                                            + ".sendon.id" + to_string(idy));
                if(sendon.has_value())
                    db->sendon[idx][idy] = sendon.get();
            }
        }
        debug->sendMessage("MixFaceKeeper::loadX32DB - Config loaded.", 5);
    }
}

void MixFaceKeeper::saveX32DB(){
    keeperThread->join();
    ptree cptree;
    cptree.put("config.lastusedip", "127.0.0.1");
    cptree.put("stat.keysolo", db->keysolo);
    for (int idx=0;idx<80;idx++){
        cptree.put("channel.id" + to_string(idx) + ".stereoon", db->stereoon[idx]);
        cptree.put("channel.id" + to_string(idx) + ".monoon", db->monoon[idx]);
        cptree.put("channel.id" + to_string(idx) + ".mlevel", db->mlevel[idx]);
        cptree.put("channel.id" + to_string(idx) + ".fader", db->fader[idx]);
        cptree.put("channel.id" + to_string(idx) + ".pan", db->pan[idx]);
        cptree.put("channel.id" + to_string(idx) + ".on", db->on[idx]);
        cptree.put("channel.id" + to_string(idx) + ".solo", db->solo[idx]);
        cptree.put("channel.id" + to_string(idx) + ".chlink", db->chlink[idx]);
        cptree.put("channel.id" + to_string(idx) + ".auxlink", db->auxlink[idx]);
        cptree.put("channel.id" + to_string(idx) + ".buslink", db->buslink[idx]);
        cptree.put("channel.id" + to_string(idx) + ".mtxlink", db->mtxlink[idx]);
        cptree.put("channel.id" + to_string(idx) + ".phantom", db->phantom[idx]);
        cptree.put("channel.id" + to_string(idx) + ".invert", db->invert[idx]);
        cptree.put("channel.id" + to_string(idx) + ".source", db->source[idx]);
        cptree.put("channel.id" + to_string(idx) + ".gain", db->gain[idx]);
        cptree.put("channel.id" + to_string(idx) + ".trim", db->trim[idx]);
        cptree.put("channel.id" + to_string(idx) + ".hpf", db->hpf[idx]);
        cptree.put("channel.id" + to_string(idx) + ".hpon", db->hpon[idx]);
        cptree.put("channel.id" + to_string(idx) + ".delayon", db->delayon[idx]);
        cptree.put("channel.id" + to_string(idx) + ".delaytime", db->delaytime[idx]);
        cptree.put("channel.id" + to_string(idx) + ".inserton", db->inserton[idx]);
        cptree.put("channel.id" + to_string(idx) + ".insertsel", db->insertsel[idx]);
        cptree.put("channel.id" + to_string(idx) + ".insertpos", db->insertpos[idx]);
        cptree.put("channel.id" + to_string(idx) + ".gateon", db->gateon[idx]);
        cptree.put("channel.id" + to_string(idx) + ".gatethr", db->gatethr[idx]);
        cptree.put("channel.id" + to_string(idx) + ".gaterange", db->gaterange[idx]);
        cptree.put("channel.id" + to_string(idx) + ".gatemode", db->gatemode[idx]);
        cptree.put("channel.id" + to_string(idx) + ".gateattack", db->gateattack[idx]);
        cptree.put("channel.id" + to_string(idx) + ".gatehold", db->gatehold[idx]);
        cptree.put("channel.id" + to_string(idx) + ".gaterelease", db->gaterelease[idx]);
        cptree.put("channel.id" + to_string(idx) + ".gatekeysrc", db->gatekeysrc[idx]);
        cptree.put("channel.id" + to_string(idx) + ".gatefilteron", db->gatefilteron[idx]);
        cptree.put("channel.id" + to_string(idx) + ".gatefiltertype", db->gatefiltertype[idx]);
        cptree.put("channel.id" + to_string(idx) + ".gatefilterf", db->gatefilterf[idx]);
        cptree.put("channel.id" + to_string(idx) + ".dynon", db->dynon[idx]);
        cptree.put("channel.id" + to_string(idx) + ".dynthr", db->dynthr[idx]);
        cptree.put("channel.id" + to_string(idx) + ".dynratio", db->dynratio[idx]);
        cptree.put("channel.id" + to_string(idx) + ".dynmix", db->dynmix[idx]);
        cptree.put("channel.id" + to_string(idx) + ".dynmgain", db->dynmgain[idx]);
        cptree.put("channel.id" + to_string(idx) + ".dynattack", db->dynattack[idx]);
        cptree.put("channel.id" + to_string(idx) + ".dynhold", db->dynhold[idx]);
        cptree.put("channel.id" + to_string(idx) + ".dynrelease", db->dynrelease[idx]);
        cptree.put("channel.id" + to_string(idx) + ".dynmode", db->dynmode[idx]);
        cptree.put("channel.id" + to_string(idx) + ".dynknee", db->dynknee[idx]);
        cptree.put("channel.id" + to_string(idx) + ".dynenv", db->dynenv[idx]);
        cptree.put("channel.id" + to_string(idx) + ".dyndet", db->dyndet[idx]);
        cptree.put("channel.id" + to_string(idx) + ".dynauto", db->dynauto[idx]);
        cptree.put("channel.id" + to_string(idx) + ".dynkeysrc", db->dynkeysrc[idx]);
        cptree.put("channel.id" + to_string(idx) + ".dynfilteron", db->dynfilteron[idx]);
        cptree.put("channel.id" + to_string(idx) + ".dynfiltertype", db->dynfiltertype[idx]);
        cptree.put("channel.id" + to_string(idx) + ".dynfilterf", db->dynfilterf[idx]);
        cptree.put("channel.id" + to_string(idx) + ".eq1type", db->eq1type[idx]);
        cptree.put("channel.id" + to_string(idx) + ".eq1g", db->eq1g[idx]);
        cptree.put("channel.id" + to_string(idx) + ".eq1f", db->eq1f[idx]);
        cptree.put("channel.id" + to_string(idx) + ".eq1q", db->eq1q[idx]);
        cptree.put("channel.id" + to_string(idx) + ".eq2type", db->eq2type[idx]);
        cptree.put("channel.id" + to_string(idx) + ".eq2g", db->eq2g[idx]);
        cptree.put("channel.id" + to_string(idx) + ".eq2f", db->eq2f[idx]);
        cptree.put("channel.id" + to_string(idx) + ".eq2q", db->eq2q[idx]);
        cptree.put("channel.id" + to_string(idx) + ".eq3type", db->eq3type[idx]);
        cptree.put("channel.id" + to_string(idx) + ".eq3g", db->eq3g[idx]);
        cptree.put("channel.id" + to_string(idx) + ".eq3f", db->eq3f[idx]);
        cptree.put("channel.id" + to_string(idx) + ".eq3q", db->eq3q[idx]);
        cptree.put("channel.id" + to_string(idx) + ".eq4type", db->eq4type[idx]);
        cptree.put("channel.id" + to_string(idx) + ".eq4g", db->eq4g[idx]);
        cptree.put("channel.id" + to_string(idx) + ".eq4f", db->eq4f[idx]);
        cptree.put("channel.id" + to_string(idx) + ".eq4q", db->eq4q[idx]);
        cptree.put("channel.id" + to_string(idx) + ".eq5type", db->eq5type[idx]);
        cptree.put("channel.id" + to_string(idx) + ".eq5g", db->eq5g[idx]);
        cptree.put("channel.id" + to_string(idx) + ".eq5f", db->eq5f[idx]);
        cptree.put("channel.id" + to_string(idx) + ".eq5q", db->eq5q[idx]);
        cptree.put("channel.id" + to_string(idx) + ".eq6type", db->eq6type[idx]);
        cptree.put("channel.id" + to_string(idx) + ".eq6g", db->eq6g[idx]);
        cptree.put("channel.id" + to_string(idx) + ".eq6f", db->eq6f[idx]);
        cptree.put("channel.id" + to_string(idx) + ".eq6q", db->eq6q[idx]);
        cptree.put("channel.id" + to_string(idx) + ".configicon", db->configicon[idx]);
        cptree.put("channel.id" + to_string(idx) + ".configcolor", db->configcolor[idx]);
        cptree.put("channel.id" + to_string(idx) + ".configname", db->configname[idx]);
        for (int idy=0;idy<16;idy++){
            cptree.put("channel.id" + to_string(idx) + ".sendlevel.id" + to_string(idy),
                             db->sendlevel[idx][idy]);
            cptree.put("channel.id" + to_string(idx) + ".sendpan.id" + to_string(idy),
                             db->sendpan[idx][idy]);
            cptree.put("channel.id" + to_string(idx) + ".sendpanfollow.id" + to_string(idy),
                             db->sendpanfollow[idx][idy]);
            cptree.put("channel.id" + to_string(idx) + ".sendtype.id" + to_string(idy),
                             db->sendtype[idx][idy]);
            cptree.put("channel.id" + to_string(idx) + ".sendon.id" + to_string(idy),
                             db->sendon[idx][idy]);
        }
    }
    ofstream configfile("MixFaceConfig.dat");
    write_info(configfile, cptree);
    configfile.close();
    debug->sendMessage("MixFaceKeeper::saveX32DB - Config saved.", 5);
}
