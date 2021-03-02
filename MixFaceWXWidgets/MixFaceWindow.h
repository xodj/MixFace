#ifndef MIXFACEWINDOW_H
#define MIXFACEWINDOW_H
#include <iostream>
#include <boost/signals2.hpp>

#include "MixFaceWindowBase.h"
//#include "dbMeter.h"

#include "DebugLibrary.hpp"
#include "MixFaceLibrary.h"

class MixFaceWindow : public MixFaceWindowBase
{
public:
    MixFaceWindow(wxWindow* parent, DebugLibrary *debug_);
    virtual ~MixFaceWindow();

private:
    DebugLibrary *debug;
};
#endif // MIXFACEWINDOW_H
