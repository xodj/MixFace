#include "MixFaceFader.h"

MixFaceFader::MixFaceFader(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
: MixFaceFaderBase(parent, id, pos, size, style)
{
    iconButton->SetBitmapHover(wxXmlResource::Get()->LoadBitmap(wxT("33B")));
}

MixFaceFader::~MixFaceFader()
{
}

