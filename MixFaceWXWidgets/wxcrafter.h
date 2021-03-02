#ifndef _MIXFACEGNU_MIXFACEWXWIDGETS_WXCRAFTER_BASE_CLASSES_H
#define _MIXFACEGNU_MIXFACEWXWIDGETS_WXCRAFTER_BASE_CLASSES_H

// clang-format off
#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/frame.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/slider.h>
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>

#ifdef WXC_FROM_DIP
#undef WXC_FROM_DIP
#endif

#if wxVERSION_NUMBER >= 3100
#define WXC_FROM_DIP(x) wxWindow::FromDIP(x, NULL)
#else
#define WXC_FROM_DIP(x) x
#endif

extern void wxC9ED9InitBitmapResources();

static bool bBitmapLoaded = false;

class MainFrameBaseClass : public wxFrame
{
protected:
    wxSlider* m_fader;
    wxBoxSizer* mainBoxSizer;
    
public:
    wxSlider* GetFader(){return m_fader;}
    
    MainFrameBaseClass(wxWindow* parent,
        wxWindowID id = wxID_ANY,
        const wxString& title = _("My Frame"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxSize(400, 300),
        long style = wxCAPTION | wxRESIZE_BORDER | wxMAXIMIZE_BOX | wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCLOSE_BOX)
        : wxFrame(parent, id, title, pos, size, style) {
    if(!bBitmapLoaded) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxC9ED9InitBitmapResources();
        bBitmapLoaded = true;
    }
    this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DLIGHT));
    this->SetForegroundColour(wxColour(wxT("rgb(0,0,0)")));

    mainBoxSizer = new wxBoxSizer(wxHORIZONTAL);
    this->SetSizer(mainBoxSizer);
    
    m_fader = new wxSlider(this, wxID_ANY, 0, 0, 1000, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)),
        wxSL_INVERSE | wxSL_VERTICAL);

    m_fader->SetMinSize(wxSize(-1, 999999));

    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DLIGHT));
    SetForegroundColour(wxColour(wxT("rgb(0,0,0)")));
    SetName(wxT("MainFrameBaseClass"));
    SetMinClientSize(wxSize(10, 10));
    SetSize(wxDLG_UNIT(this, wxSize(400, 300)));
    if(GetSizer()) {
        GetSizer()->Fit(this);
    }
    if(GetParent()) {
        CentreOnParent(wxVERTICAL);
    } else {
        CentreOnScreen(wxVERTICAL);
    }
    if(!wxPersistenceManager::Get().Find(this)) {
        wxPersistenceManager::Get().RegisterAndRestore(this);
    } else {
        wxPersistenceManager::Get().Restore(this);
    }
}
    virtual ~MainFrameBaseClass(){}
};

#endif
