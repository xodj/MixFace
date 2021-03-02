#include "MixFaceWindow.h"

MixFaceWindow::MixFaceWindow(wxWindow* parent, DebugLibrary *debug_)
    : MixFaceWindowBase(parent),
      debug(debug_)
{
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

