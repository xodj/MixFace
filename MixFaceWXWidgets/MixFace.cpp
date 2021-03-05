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
#include "Timer.hpp"

class MixFaceMain : public MixFaceMainBase
{
public:
    MixFaceMain(wxWindow* parent, DebugLibrary *debug_)
        : MixFaceMainBase(parent),
          debug(debug_) {
        debug->sendMessage("MixFaceMain::MixFaceMain Init...",0);
        mf_iconslist = new IconsList;
        scrollPanelSizer = new wxBoxSizer(wxHORIZONTAL);
        scrollPanel->SetSizer(scrollPanelSizer);
        for(int i = 0;i < 4; i++){
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

        Connect(settingsButton->GetId(),wxEVT_BUTTON,wxObjectEventFunction(&MixFaceMain::sliderUpdated));
    }
    virtual ~MixFaceMain(){}
    void sliderUpdated(wxCommandEvent& event)
    {
        debug->sendMessage("MixFaceMain::sliderUpdated" + std::to_string(mf_fader[0] != NULL),0);
        if(mf_fader[0] != NULL){
            mf_fader[0]->Hide();
            delete mf_fader[0];
            mf_fader[0] = NULL;
        } else {
            mf_fader[0] = new MixFaceFader(scrollPanel, wxID_ANY, wxDefaultPosition,
                                           wxDLG_UNIT(scrollPanel, wxSize(128, 480)),
                                           wxTAB_TRAVERSAL|wxBORDER_STATIC,
                                           mf_iconslist);
            scrollPanelSizer->Add(mf_fader[0], 0, wxALL|wxEXPAND);
            mf_fader[0]->SetMinSize(wxSize(128,480));
            mf_fader[0]->setIcon(0);
        }
    }
    void sliderUpdated0(wxCommandEvent& event)
    {
        __UNUSED_PARAM(event)
        mf_fader[0]->Hide();
        delete mf_fader[0];
    }

private:
    IconsList *mf_iconslist;
    wxBoxSizer *scrollPanelSizer;
    DebugLibrary *debug;
    MixFaceFader *mf_fader[80];
};

class MixFaceLogo : public MixFaceMainLogo
{
    public:

        MixFaceLogo(wxWindow* parent, wxWindowID id = wxID_ANY,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxSize( 1280,720 ),
                    long style = wxTAB_TRAVERSAL,
                    const wxString& name = wxEmptyString)
            :MixFaceMainLogo(parent, id, pos, size, style, name)
        {
            m_bitmapLogo->SetBitmap(wxBitmap(MainLogo().Bitmap("x32mixerlogo")));
        }
        ~MixFaceLogo(){}

};

class MixFaceWindow : public MixFaceWindowBase
{
public:
    MixFaceWindow(DebugLibrary *debug_)
        : MixFaceWindowBase(NULL),
          debug(debug_) {
        //logo = new MixFaceLogo(NULL);
        //bSizer->Add(logo, 0, wxALL|wxEXPAND, 0);
        parInit();

        /*IntervalThread *timer = new IntervalThread(1000);
        timer->connect(IntervalThread::interval_slot_t(&MixFaceWindow::parInit, this));
        timer->start();*/
    }
    virtual ~MixFaceWindow(){}
    void parInit()
    {
        debug->sendMessage("MixFaceWindow::parInit 1",0);
        MixFaceMain *mainPanel = new MixFaceMain(this, debug);
        debug->sendMessage("MixFaceWindow::parInit 2",0);
        bSizer->Add(mainPanel, 0, wxALL|wxEXPAND, 0);
        debug->sendMessage("MixFaceWindow::parInit 3",0);
        mainPanel->Show(true);
    }

private:
    MixFaceLogo *logo;
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
