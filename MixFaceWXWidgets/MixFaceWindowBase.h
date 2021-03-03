///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/gdicmn.h>
#include <wx/button.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/scrolwin.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MixFaceWindowBase
///////////////////////////////////////////////////////////////////////////////
class MixFaceWindowBase : public wxFrame
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

		MixFaceWindowBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("MixFace"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1280,720 ), long style = wxDEFAULT_FRAME_STYLE );

		~MixFaceWindowBase();

};

