#ifndef MIXFACEWINDOW_H
#define MIXFACEWINDOW_H
#include <iostream>
#include <boost/signals2.hpp>

#include "MixFaceBaseObjects.h"
//#include "dbMeter.h"
#include "MixFaceFader.h"
#include "MixFaceResources.h"

#include "DebugLibrary.hpp"
//#include "MixFaceLibrary.h"

class MixFaceWindow : public MixFaceWindowBase
{
public:
    MixFaceWindow(DebugLibrary *debug_);
    virtual ~MixFaceWindow();

private:
    DebugLibrary *debug;
    MixFaceFader *mf_fader[80];
};
#endif // MIXFACEWINDOW_H
