#include "MixFaceWindow.h"

MixFaceWindow::MixFaceWindow(wxWindow* parent, DebugLibrary *debug_)
    : MixFaceWindowBase(parent),
      debug(debug_)
{
    wxBoxSizer* scrollPanelSizer = new wxBoxSizer(wxHORIZONTAL);
    scrollPanel->SetSizer(scrollPanelSizer);
    for(int i = 0;i < 128; i++){
        mf_fader[i] = new MixFaceFader(scrollPanel, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(scrollPanel, wxSize(128,-1)), wxTAB_TRAVERSAL|wxBORDER_STATIC);
        scrollPanelSizer->Add(mf_fader[i], 0, wxALL|wxEXPAND, WXC_FROM_DIP(0));
        mf_fader[i]->SetMinSize(wxSize(128,480));
    }
    /*mainPanelFader = new MixFaceFader(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(128,-1)), wxTAB_TRAVERSAL|wxBORDER_STATIC);
    botSizer->Add(mainPanel, 0, wxALL|wxEXPAND, WXC_FROM_DIP(0));
    mainPanelFader->SetMinSize(wxSize(128,-1));*/
    /*wxBoxSizer* mainPanelSizer = new wxBoxSizer(wxVERTICAL);
    mainPanel->SetSizer(mainPanelSizer);

    singleDBMeterPanel *drawPane = new singleDBMeterPanel(this);
    mainPanelSizer->Add(drawPane, 1, wxEXPAND);

    meterRenewTimer *m_timer = new meterRenewTimer(drawPane);
    m_timer->Start(10);
    meterDemoTimer *m_demo_t = new meterDemoTimer(drawPane);
    m_demo_t->Start(30);*/
}

MixFaceWindow::~MixFaceWindow()
{
}

