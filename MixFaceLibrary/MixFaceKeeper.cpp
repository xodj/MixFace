#include "MixFaceKeeper.h"
MixFaceKeeper::MixFaceKeeper(){
    boost::thread *keeperThread = new boost::thread{};
    keeperThread->join();
}
