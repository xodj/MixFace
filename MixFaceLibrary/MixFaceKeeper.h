#ifndef MIXFACEKEEPER_H
#define MIXFACEKEEPER_H

#include <boost/thread.hpp>
#include <boost/signals2.hpp>
#include "DebugLibrary.hpp"
#include "MixFaceStatic.h"

class MixFaceKeeper {
public:
    MixFaceKeeper(DebugLibrary *debug = new DebugLibrary);

    x32db *getX32DB() { return db; }
    void loadX32DB();
    void saveX32DB();

private:
    x32db *db;

    DebugLibrary *debug;
    boost::thread *keeperThread;
};

#endif // MIXFACEKEEPER_H
