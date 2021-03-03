#include "MixFaceFader.h"

MixFaceFader::MixFaceFader(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
: MixFaceFaderBase(parent, id, pos, size, style)
{
    meterRenewTimer *m_timer = new meterRenewTimer(meterPanel0);
    m_timer->Start(10);
    meterDemoTimer *m_demo_t = new meterDemoTimer(meterPanel0);
    m_demo_t->Start(30);
    meterRenewTimer *m_timer1 = new meterRenewTimer(meterPanel1);
    m_timer1->Start(10);
    meterDemoTimer *m_demo_t1 = new meterDemoTimer(meterPanel1);
    m_demo_t1->Start(60);
}

MixFaceFader::~MixFaceFader()
{
}

