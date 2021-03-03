///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include "dbMeter.h"
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
#include <wx/tglbtn.h>
#include <wx/slider.h>
#include <wx/panel.h>
#include <wx/bmpbuttn.h>

///////////////////////////////////////////////////////////////////////////


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
		singleDBMeterPanel* meterPanel1;
		singleDBMeterPanel* meterPanel0;
		wxSlider* faderSlider;
		wxToggleButton* soloButton;
		wxBitmapButton* iconButton;

	public:

		MixFaceFaderBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 128,600 ), long style = wxTAB_TRAVERSAL, const wxString& name = wxEmptyString );
		~MixFaceFaderBase();

};

