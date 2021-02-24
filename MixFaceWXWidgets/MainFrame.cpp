#include "MainFrame.h"

MainFrame::MainFrame(wxWindow* parent)
    : MainFrameBaseClass(parent)
{    
    mainBoxSizer->Add(m_fader, 0, wxALL, WXC_FROM_DIP(5));
    
    drawPane = new singleDBMeterPanel(this);
    mainBoxSizer->Add(drawPane, 1, wxEXPAND);
    meterRenewTimer *m_timer = new meterRenewTimer(drawPane);
    m_timer->Start(10);
    m_demo_t = new meterDemoTimer(drawPane);
    m_demo_t->Start(10);
    
    Connect(m_fader->GetId(), wxEVT_COMMAND_SLIDER_UPDATED, (wxObjectEventFunction)&MainFrame::sliderUpdated);
}

MainFrame::~MainFrame()
{
    Disconnect(m_fader->GetId(), wxEVT_COMMAND_SLIDER_UPDATED, (wxObjectEventFunction)&MainFrame::sliderUpdated);
    drawPane->~singleDBMeterPanel();
}

void MainFrame::sliderUpdated(wxCommandEvent& event)
{
    __UNUSED_PARAM(event)
    drawPane->setValue((float)(m_fader->GetValue()) / 1000.f);
    m_demo_t->Stop();
}
