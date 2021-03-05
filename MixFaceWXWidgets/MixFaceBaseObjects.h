///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/sizer.h>
#include <wx/gdicmn.h>
#include <wx/string.h>
#include <wx/frame.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/scrolwin.h>
#include <wx/tglbtn.h>
#include <wx/slider.h>
#include <wx/bmpbuttn.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MixFaceWindowBase
///////////////////////////////////////////////////////////////////////////////
class MixFaceWindowBase : public wxFrame
{
	private:

	protected:

	public:
		wxBoxSizer* bSizer;

		MixFaceWindowBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("MixFace"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1280,720 ), long style = wxDEFAULT_FRAME_STYLE );

		~MixFaceWindowBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class MixFaceMainLogo
///////////////////////////////////////////////////////////////////////////////
class MixFaceMainLogo : public wxPanel
{
	private:

	protected:
		wxStaticBitmap* m_bitmapLogo;

	public:

		MixFaceMainLogo( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1280,720 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~MixFaceMainLogo();

};

///////////////////////////////////////////////////////////////////////////////
/// Class MixFaceMainBase
///////////////////////////////////////////////////////////////////////////////
class MixFaceMainBase : public wxPanel
{
	private:

	protected:
		wxPanel* topPanel;
		wxButton* settingsButton;
		wxScrolledWindow* scrollPanel;
		wxBoxSizer* mainPanelSizer;
		wxPanel* rightPanel;
		wxButton* busButton;
		wxButton* assignButton;

	public:

		MixFaceMainBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1280,720 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~MixFaceMainBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class MixFaceFaderBase
///////////////////////////////////////////////////////////////////////////////
class MixFaceFaderBase : public wxPanel
{
	private:

	protected:
		wxButton* srcButton;
		wxButton* eqButton;
		wxButton* dynButton;
		wxToggleButton* muteButton;
		wxSlider* panSlider;
		wxBoxSizer* bSizer5;
		wxToggleButton* soloButton;
		wxBitmapButton* iconButton;

	public:
		wxSlider* faderSlider;

		MixFaceFaderBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 128,600 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~MixFaceFaderBase();

};

