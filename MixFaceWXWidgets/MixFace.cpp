#include <wx/app.h>
#include <wx/event.h>
#include <wx/image.h>
#include <iostream>
#include <boost/signals2.hpp>
#include <boost/thread.hpp>

#include "DebugLibrary.hpp"
//#include "MixFaceLibrary.h"
#include "MixFaceBaseObjects.h"
#include "MixFaceResources.h"
#include "MixFaceFader.h"

typedef boost::signals2::signal<void()> signal_mainPanel;
typedef signal_mainPanel::slot_type signal_type_mainPanel;

class MixFaceMain : public MixFaceMainBase
{
public:
    MixFaceMain(wxWindow* parent, DebugLibrary *debug_)
        : MixFaceMainBase(parent),
          debug(debug_) {
        IconsList *mf_iconslist = new IconsList;
        wxBoxSizer* scrollPanelSizer = new wxBoxSizer(wxHORIZONTAL);
        scrollPanel->SetSizer(scrollPanelSizer);
        for(int i = 0;i < 74; i++){
            mf_fader[i] = new MixFaceFader(scrollPanel, wxID_ANY, wxDefaultPosition,
                                           wxDLG_UNIT(scrollPanel, wxSize(128,-1)),
                                           wxTAB_TRAVERSAL|wxBORDER_STATIC,
                                           mf_iconslist);
            scrollPanelSizer->Add(mf_fader[i], 0, wxALL|wxEXPAND);
            mf_fader[i]->SetMinSize(wxSize(128,480));
            mf_fader[i]->setIcon(i);
        }
        MixFaceFader *mainPanelFader = new MixFaceFader(this, wxID_ANY, wxDefaultPosition,
                                                        wxDLG_UNIT(this, wxSize(128,-1)),
                                                        wxTAB_TRAVERSAL|wxBORDER_STATIC,
                                                        mf_iconslist);
        mainPanelSizer->Add(mainPanelFader, 0, wxALL|wxEXPAND);
        mainPanelFader->SetMinSize(wxSize(128,-1));
        mainPanelFader->SetMaxSize(wxSize(128,-1));
        mainPanelFader->setIcon(73);
        boost::this_thread::sleep_for(boost::chrono::milliseconds{2000});
        slot_mainPanel();
    }
    virtual ~MixFaceMain(){}
    signal_mainPanel slot_mainPanel;

private:
    DebugLibrary *debug;
    MixFaceFader *mf_fader[80];
};

class MixFaceWindow : public MixFaceWindowBase
{
public:
    MixFaceWindow(DebugLibrary *debug_)
        : MixFaceWindowBase(NULL),
          debug(debug_) {
        mainPanel = new MixFaceMain(this, debug);
        bSizer->Add(mainPanel, 0, wxALL|wxEXPAND, 0);
    }
    virtual ~MixFaceWindow(){}

private:
    MixFaceMain *mainPanel;
    DebugLibrary *debug;
};

class MixFace : public wxApp
{
public:
    MixFace() {}
    virtual ~MixFace() {}

    DebugLibrary *debug;

    virtual bool OnInit()
    {
        debug = new DebugLibrary(argc, argv);
        debug->sendMessage("MixFace::OnInit Init MixFaceWindow...",5);

        // Add the common image handlers
        wxImage::AddHandler(new wxPNGHandler);
        wxImage::AddHandler(new wxJPEGHandler);

        MixFaceWindow *mf_window = new MixFaceWindow(debug);
        SetTopWindow(mf_window);
        return GetTopWindow()->Show();
    }
};

DECLARE_APP(MixFace)
IMPLEMENT_APP(MixFace)
