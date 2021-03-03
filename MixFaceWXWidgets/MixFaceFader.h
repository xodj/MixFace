#ifndef MIXFACEFADER_H
#define MIXFACEFADER_H
#include "MixFaceFaderBase.h"

class MixFaceFader : public MixFaceFaderBase
{
public:
    MixFaceFader(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(128,600), long style = wxTAB_TRAVERSAL);
    virtual ~MixFaceFader();
};
#endif // MIXFACEFADER_H
