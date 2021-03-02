#include "MixFaceWindow.h"
#include <wx/app.h>
#include <wx/event.h>
#include <wx/image.h>

class MainApp : public wxApp
{
public:
    MainApp() {}
    virtual ~MainApp() {}

    DebugLibrary *debug;

    virtual bool OnInit()
    {
        debug = new DebugLibrary(argc, argv);
        debug->sendMessage("MainApp::OnInit Init MixFaceWindow...",5);
        // Add the common image handlers
        wxImage::AddHandler(new wxPNGHandler);
        wxImage::AddHandler(new wxJPEGHandler);

        MixFaceWindow* mf_window = new MixFaceWindow(NULL, debug);
        SetTopWindow(mf_window);
        return GetTopWindow()->Show();
    }
};

DECLARE_APP(MainApp)
IMPLEMENT_APP(MainApp)
