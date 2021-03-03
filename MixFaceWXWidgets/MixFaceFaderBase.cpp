///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "MixFaceFaderBase.h"

///////////////////////////////////////////////////////////////////////////

MixFaceFaderBase::MixFaceFaderBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	srcButton = new wxButton( this, wxID_ANY, wxT("SRC"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( srcButton, 0, wxALL|wxEXPAND, 0 );

	eqButton = new wxButton( this, wxID_ANY, wxT("EQ"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( eqButton, 0, wxALL|wxEXPAND, 0 );

	dynButton = new wxButton( this, wxID_ANY, wxT("DYN"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( dynButton, 0, wxALL|wxEXPAND, 0 );


	bSizer1->Add( bSizer2, 0, wxALL|wxEXPAND, 5 );

	muteButton = new wxToggleButton( this, wxID_ANY, wxT("MUTE"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( muteButton, 0, wxALL|wxEXPAND, 5 );

	panSlider = new wxSlider( this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	bSizer1->Add( panSlider, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );


	bSizer4->Add( 0, 0, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );

	bSizer5->SetMinSize( wxSize( 16,-1 ) );
	meterPanel1 = new singleDBMeterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	meterPanel1->SetMinSize( wxSize( 16,-1 ) );
	meterPanel1->SetMaxSize( wxSize( 16,-1 ) );

	bSizer5->Add( meterPanel1, 1, wxALL|wxEXPAND, 0 );

	meterPanel0 = new singleDBMeterPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	meterPanel0->SetMinSize( wxSize( 16,-1 ) );
	meterPanel0->SetMaxSize( wxSize( 16,-1 ) );

	bSizer5->Add( meterPanel0, 1, wxALL|wxEXPAND, 0 );


	bSizer4->Add( bSizer5, 3, wxALL|wxEXPAND, 0 );


	bSizer3->Add( bSizer4, 1, wxALL|wxEXPAND, 5 );

	faderSlider = new wxSlider( this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_INVERSE|wxSL_VERTICAL );
	bSizer3->Add( faderSlider, 1, wxALL|wxEXPAND|wxALIGN_CENTER, 5 );


	bSizer3->Add( 0, 0, 1, wxEXPAND, 5 );


	bSizer1->Add( bSizer3, 1, wxALL|wxEXPAND, 5 );

	soloButton = new wxToggleButton( this, wxID_ANY, wxT("SOLO"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( soloButton, 0, wxALL|wxEXPAND, 5 );

	iconButton = new wxBitmapButton( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 118,118 ), wxBU_AUTODRAW|0|wxBORDER_SIMPLE );
	bSizer1->Add( iconButton, 0, wxALL, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();
}

MixFaceFaderBase::~MixFaceFaderBase()
{
}
