///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "MixFaceBaseObjects.h"

///////////////////////////////////////////////////////////////////////////

MixFaceWindowBase::MixFaceWindowBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetFont( wxFont( 14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );
	this->SetForegroundColour( wxColour( 240, 240, 240 ) );
	this->SetBackgroundColour( wxColour( 96, 96, 96 ) );

	bSizer = new wxBoxSizer( wxVERTICAL );


	this->SetSizer( bSizer );
	this->Layout();

	this->Centre( wxBOTH );
}

MixFaceWindowBase::~MixFaceWindowBase()
{
}

MixFaceMainLogo::MixFaceMainLogo( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	this->SetForegroundColour( wxColour( 240, 240, 240 ) );
	this->SetBackgroundColour( wxColour( 96, 96, 96 ) );

}

MixFaceMainLogo::~MixFaceMainLogo()
{
}

MixFaceMainBase::MixFaceMainBase( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name )
{
	this->SetForegroundColour( wxColour( 240, 240, 240 ) );
	this->SetBackgroundColour( wxColour( 96, 96, 96 ) );

	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );

	topPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1,32 ), wxTAB_TRAVERSAL|wxBORDER_STATIC );
	topPanel->SetFont( wxFont( 8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );


	bSizer2->Add( 0, 0, 1, wxEXPAND, 5 );

	settingsButton = new wxButton( topPanel, wxID_ANY, wxT("Settings"), wxDefaultPosition, wxSize( 64,28 ), 0 );
	bSizer2->Add( settingsButton, 0, wxLEFT|wxTOP|wxBOTTOM, 4 );


	bSizer2->Add( 8, 0, 0, wxEXPAND, 5 );


	topPanel->SetSizer( bSizer2 );
	topPanel->Layout();
	bSizer11->Add( topPanel, 0, wxALL|wxEXPAND, 0 );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );

	scrollPanel = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_STATIC|wxHSCROLL );
	scrollPanel->SetScrollRate( 5, 5 );
	bSizer3->Add( scrollPanel, 1, wxALL|wxEXPAND, 0 );

	mainPanelSizer = new wxBoxSizer( wxHORIZONTAL );

	mainPanelSizer->SetMinSize( wxSize( 128,-1 ) );

	bSizer3->Add( mainPanelSizer, 0, wxALL|wxEXPAND, 0 );

	rightPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 64,-1 ), wxTAB_TRAVERSAL|wxBORDER_STATIC );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );


	bSizer4->Add( 0, 0, 1, wxEXPAND, 5 );

	busButton = new wxButton( rightPanel, wxID_ANY, wxT("LR"), wxDefaultPosition, wxSize( 50,40 ), 0 );
	bSizer4->Add( busButton, 0, wxALL|wxALIGN_CENTER, 1 );

	assignButton = new wxButton( rightPanel, wxID_ANY, wxT("A\nB\nC\nD\nE\nF"), wxDefaultPosition, wxSize( 50,150 ), 0 );
	bSizer4->Add( assignButton, 0, wxALL|wxALIGN_CENTER, 0 );


	bSizer4->Add( 0, 0, 1, wxEXPAND, 5 );


	rightPanel->SetSizer( bSizer4 );
	rightPanel->Layout();
	bSizer3->Add( rightPanel, 0, wxALL|wxEXPAND, 0 );


	bSizer11->Add( bSizer3, 1, wxALL|wxEXPAND, 0 );


	this->SetSizer( bSizer11 );
	this->Layout();
}

MixFaceMainBase::~MixFaceMainBase()
{
}

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

	bSizer5 = new wxBoxSizer( wxHORIZONTAL );

	bSizer5->SetMinSize( wxSize( 16,-1 ) );

	bSizer4->Add( bSizer5, 3, wxALL|wxEXPAND, 0 );


	bSizer3->Add( bSizer4, 1, wxALL|wxEXPAND, 5 );

	faderSlider = new wxSlider( this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_INVERSE|wxSL_VERTICAL );
	bSizer3->Add( faderSlider, 1, wxALL|wxEXPAND|wxALIGN_CENTER, 5 );


	bSizer3->Add( 0, 0, 1, wxEXPAND, 5 );


	bSizer1->Add( bSizer3, 1, wxALL|wxEXPAND, 5 );

	soloButton = new wxToggleButton( this, wxID_ANY, wxT("SOLO"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( soloButton, 0, wxALL|wxEXPAND, 5 );

	iconButton = new wxBitmapButton( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 118,118 ), wxBU_AUTODRAW|wxBORDER_NONE|wxBORDER_SIMPLE );
	iconButton->SetForegroundColour( wxColour( 240, 240, 240 ) );
	iconButton->SetBackgroundColour( wxColour( 96, 96, 96 ) );

	bSizer1->Add( iconButton, 0, wxALL, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();
}

MixFaceFaderBase::~MixFaceFaderBase()
{
}
