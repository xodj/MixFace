#ifndef MIXFACEFADER_H
#define MIXFACEFADER_H
#include <iostream>
#include "MixFaceBaseObjects.h"
#include "MixFaceResources.h"
#include "MixFaceMeter.h"

using namespace std;

class MixFaceFader : public MixFaceFaderBase
{
public:
    MixFaceFader(wxWindow* parent, wxWindowID id = wxID_ANY,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxSize(128,600),
                 long style = wxTAB_TRAVERSAL,
                 IconsList *mf_iconslist_ = NULL,
                 int id_ = 0);
    virtual ~MixFaceFader();

    void setIcon(int idx);
    void setId(int id_){ id = id_; }
    int getId(){ return id; }

private:
    int id;
    IconsList *mf_iconslist;
};
#endif // MIXFACEFADER_H
