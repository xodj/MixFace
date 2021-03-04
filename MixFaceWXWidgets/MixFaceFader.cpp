#include "MixFaceFader.h"

MixFaceFader::MixFaceFader(wxWindow* parent, wxWindowID id,
                           const wxPoint& pos, const wxSize& size,
                           long style, IconsList *mf_iconslist_,
                           int id_)
: MixFaceFaderBase(parent, id, pos, size, style),
  id(id_), mf_iconslist(mf_iconslist_)
{
    if(mf_iconslist == NULL) mf_iconslist = new IconsList;

    const wxBitmap defIcon = mf_iconslist->Bitmap("1B");
    iconButton->SetBitmapHover(defIcon);
    iconButton->SetBitmapPressed(defIcon);

    /*singleDBMeterPanel *meterPanel1 = new singleDBMeterPanel(this, wxID_ANY,
                                                             wxDefaultPosition,
                                                             wxDefaultSize,
                                                             wxTAB_TRAVERSAL);
    meterPanel1->SetMinSize( wxSize( 16,-1 ) );
    meterPanel1->SetMaxSize( wxSize( 16,-1 ) );

    bSizer5->Add( meterPanel1, 1, wxALL|wxEXPAND, 0 );

    singleDBMeterPanel *meterPanel0 = new singleDBMeterPanel(this, wxID_ANY,
                                                             wxDefaultPosition,
                                                             wxDefaultSize,
                                                             wxTAB_TRAVERSAL);
    meterPanel0->SetMinSize( wxSize( 16,-1 ) );
    meterPanel0->SetMaxSize( wxSize( 16,-1 ) );

    bSizer5->Add( meterPanel0, 1, wxALL|wxEXPAND, 0 );
    meterRenewTimer *m_timer = new meterRenewTimer(meterPanel0);
    m_timer->Start(10);
    meterDemoTimer *m_demo_t = new meterDemoTimer(meterPanel0);
    m_demo_t->Start(30);
    meterRenewTimer *m_timer1 = new meterRenewTimer(meterPanel1);
    m_timer1->Start(10);
    meterDemoTimer *m_demo_t1 = new meterDemoTimer(meterPanel1);
    m_demo_t1->Start(60);*/
}

MixFaceFader::~MixFaceFader() {}

void MixFaceFader::setIcon(int idx)
{
    idx += 1;
    wxString bitMapName = to_string(idx) + "B";
    iconButton->SetBitmap(mf_iconslist->Bitmap(bitMapName));
    wxString bitMapNameHover = to_string(idx) + "W";
    iconButton->SetBitmapPressed(mf_iconslist->Bitmap(bitMapNameHover));
}

