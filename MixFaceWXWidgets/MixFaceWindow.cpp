#include "MixFaceWindow.h"

MixFaceWindow::MixFaceWindow(DebugLibrary *debug_)
    : MixFaceWindowBase(NULL),
      debug(debug_)
{
    /*IconsList *mf_iconslist = new IconsList;
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
    mainPanelFader->setIcon(73);*/
}

MixFaceWindow::~MixFaceWindow() {}
