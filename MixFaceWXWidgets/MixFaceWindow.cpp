#include "MixFaceWindow.h"

MixFaceWindow::MixFaceWindow(wxWindow* parent, DebugLibrary *debug_)
    : MixFaceWindowBase(parent),
      debug(debug_)
{
    wxBoxSizer* scrollPanelSizer = new wxBoxSizer(wxHORIZONTAL);
    scrollPanel->SetSizer(scrollPanelSizer);
    for(int i = 0;i < 32; i++){
        mf_fader[i] = new MixFaceFader(scrollPanel, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(scrollPanel, wxSize(128,-1)), wxTAB_TRAVERSAL|wxBORDER_STATIC);
        scrollPanelSizer->Add(mf_fader[i], 0, wxALL|wxEXPAND);
        mf_fader[i]->SetMinSize(wxSize(128,480));
    }
    MixFaceFader *mainPanelFader = new MixFaceFader(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(128,-1)), wxTAB_TRAVERSAL|wxBORDER_STATIC);
    mainPanelSizer->Add(mainPanelFader, 0, wxALL|wxEXPAND);
    mainPanelFader->SetMinSize(wxSize(128,-1));
    mainPanelFader->SetMaxSize(wxSize(128,-1));
}

MixFaceWindow::~MixFaceWindow()
{
}

