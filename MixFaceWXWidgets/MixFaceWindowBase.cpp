///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "MixFaceWindowBase.h"

///////////////////////////////////////////////////////////////////////////

MixFaceWindowBase::MixFaceWindowBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetFont( wxFont( 14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );
	this->SetBackgroundColour( wxColour( 96, 96, 96 ) );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	topPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1,32 ), wxTAB_TRAVERSAL|wxBORDER_STATIC );
	topPanel->SetFont( wxFont( 8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );

	settingsButton = new wxButton( topPanel, wxID_ANY, wxT("Settings"), wxDefaultPosition, wxSize( 64,28 ), 0|wxBORDER_NONE|wxBORDER_STATIC );
	settingsButton->SetForegroundColour( wxColour( 255, 255, 255 ) );
	settingsButton->SetBackgroundColour( wxColour( 96, 96, 96 ) );

	bSizer2->Add( settingsButton, 0, wxLEFT|wxTOP|wxBOTTOM, 4 );


	topPanel->SetSizer( bSizer2 );
	topPanel->Layout();
	bSizer1->Add( topPanel, 0, wxALL|wxEXPAND, 0 );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );

	scrollPanel = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_STATIC|wxHSCROLL );
	scrollPanel->SetScrollRate( 5, 5 );
	bSizer3->Add( scrollPanel, 1, wxALL|wxEXPAND, 0 );

	mainPanel = new MixFaceFaderBase( this, wxID_ANY, wxDefaultPosition, wxSize( 128,-1 ), wxTAB_TRAVERSAL|wxBORDER_STATIC );
	bSizer3->Add( mainPanel, 0, wxALL|wxEXPAND, 0 );

	rightPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 64,-1 ), wxTAB_TRAVERSAL|wxBORDER_STATIC );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );

	busButton = new wxButton( rightPanel, wxID_ANY, wxT("LR"), wxDefaultPosition, wxSize( 50,40 ), 0|wxBORDER_NONE );
	busButton->SetForegroundColour( wxColour( 255, 255, 255 ) );
	busButton->SetBackgroundColour( wxColour( 96, 96, 96 ) );

	bSizer4->Add( busButton, 0, wxALL|wxALIGN_CENTER, 1 );

	assignButton = new wxButton( rightPanel, wxID_ANY, wxT("A\nB\nC\nD\nE\nF"), wxDefaultPosition, wxSize( 50,150 ), 0|wxBORDER_NONE );
	assignButton->SetForegroundColour( wxColour( 255, 255, 255 ) );
	assignButton->SetBackgroundColour( wxColour( 96, 96, 96 ) );

	bSizer4->Add( assignButton, 0, wxALL|wxALIGN_CENTER, 0 );


	rightPanel->SetSizer( bSizer4 );
	rightPanel->Layout();
	bSizer3->Add( rightPanel, 0, wxALL|wxEXPAND, 0 );


	bSizer1->Add( bSizer3, 1, wxALL|wxEXPAND, 0 );


	this->SetSizer( bSizer1 );
	this->Layout();

	this->Centre( wxBOTH );
}

MixFaceWindowBase::~MixFaceWindowBase()
{
}
