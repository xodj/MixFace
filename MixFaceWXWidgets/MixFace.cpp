#include "MixFaceBaseObjects.h"
#include "MixFaceWindow.h"
#include <wx/app.h>
#include <wx/event.h>
#include <wx/image.h>

class MixFace : public wxApp
{
public:
    MixFace() {}
    virtual ~MixFace() {}

    DebugLibrary *debug;

    virtual bool OnInit()
    {
        debug = new DebugLibrary(argc, argv);
        debug->sendMessage("MixFace::OnInit Init MixFaceWindow...",5);

        // Add the common image handlers
        wxImage::AddHandler(new wxPNGHandler);
        wxImage::AddHandler(new wxJPEGHandler);

        MixFaceWindow *mf_window = new MixFaceWindow(debug);
        SetTopWindow(mf_window);
        return GetTopWindow()->Show();
    }
};

DECLARE_APP(MixFace)
IMPLEMENT_APP(MixFace)
