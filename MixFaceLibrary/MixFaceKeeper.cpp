#include "MixFaceKeeper.h"
MixFaceKeeper::MixFaceKeeper() : QObject(){}

void MixFaceKeeper::open(){
    emitopened();
}

void MixFaceKeeper::save(){
    emitsaved();
}

void MixFaceKeeper::emitopened(){
    emit opened();
}

void MixFaceKeeper::emitsaved(){
    emit saved();
}
