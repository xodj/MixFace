#include "MixFaceMeter.h"

singleDBMeterPanel::singleDBMeterPanel(wxWindow *parent,
                                       wxWindowID winid,
                                       const wxPoint& pos ,
                                       const wxSize& size ,
                                       long style,
                                       const wxString& name)
    : wxPanel(parent, winid, pos, size, style, name)
{
    SetSize((meterWSize + (borders * 2)),-1);
    panelWSize = (meterWSize + (borders * 2));
    Connect(wxEVT_PAINT, wxPaintEventHandler(singleDBMeterPanel::paintEvent));
    Connect(wxEVT_SIZE, wxSizeEventHandler(singleDBMeterPanel::paintEvent));
}

singleDBMeterPanel::~singleDBMeterPanel() {
    Disconnect(wxEVT_PAINT, wxPaintEventHandler(singleDBMeterPanel::paintEvent));
    Disconnect(wxEVT_SIZE, wxSizeEventHandler(singleDBMeterPanel::paintEvent));
}

void singleDBMeterPanel::render(wxDC& drawControl) {
    int redraw = std::chrono::duration_cast<std::chrono::milliseconds>
            (std::chrono::system_clock::now().time_since_epoch() %
             std::chrono::seconds{ 1000 }).count();
    calculateDecay(redraw);
    int hsize = GetSize().GetHeight();
    if (hsize != panelHSize) {
        panelHSize = hsize;
        renderBackground();
        drawControl.DrawBitmap(backgroundFill, 0, panelHSize - borders);
    }
    drawControl.DrawBitmap(renderPeak(redraw), borders, borders);
    drawControl.DrawBitmap(renderMeter(redraw), borders, borders + peakHSize - 1);
}

void singleDBMeterPanel::renderBackground(){
    backgroundFill.Create(panelWSize, borders);
    wxMemoryDC backgroundFillDC(backgroundFill);
    backgroundFillDC.SetBrush(wxBrush(bgColor));
    backgroundFillDC.SetPen(wxPen(bgColor, 1));
    backgroundFillDC.DrawRectangle(0, 0, panelWSize, borders);

    meterHSize = panelHSize - (borders * 2) - peakHSize;
    redSize = int(meterHSize * redAspect);
    yellowSize = int(meterHSize * yellowAspect);
    greenSize = int(meterHSize - redSize - yellowSize);

    backgroundMeter.Create(meterWSize, meterHSize);
    wxMemoryDC backgroundMeterDC(backgroundMeter);
    backgroundMeterDC.SetBrush(wxBrush(lgreenColor));
    backgroundMeterDC.SetPen(wxPen(lgreenColor, 1));
    backgroundMeterDC.DrawRectangle(0, meterHSize, meterWSize, - greenSize);
    backgroundMeterDC.SetBrush(wxBrush(lyellowColor));
    backgroundMeterDC.SetPen(wxPen(lyellowColor, 1));
    backgroundMeterDC.DrawRectangle(0, yellowSize + redSize, meterWSize, - yellowSize);
    backgroundMeterDC.SetBrush(wxBrush(lredColor));
    backgroundMeterDC.SetPen(wxPen(lredColor, 1));
    backgroundMeterDC.DrawRectangle(0, redSize, meterWSize, - redSize);
}
    
wxBitmap singleDBMeterPanel::renderPeak(int redraw){
    if (currentPeak == 1.f)
        lastPeakRedraw = redraw;
    wxBitmap peak;
    peak.Create(meterWSize, peakHSize);
    wxMemoryDC drawControl(peak);
    if (currentPeak == 1.f || (lastPeakRedraw + peakHoldMS) > redraw){
        drawControl.SetBrush(*wxRED_BRUSH);
        if (drawFrame){
            drawControl.SetPen(*wxBLACK_PEN);
        } else {
            drawControl.SetPen(*wxRED_PEN);
        }
        drawControl.DrawRectangle(0, 0, meterWSize, peakHSize);
    } else {
        drawControl.SetBrush(wxBrush(lredColor));
        if (drawFrame){
            drawControl.SetPen(*wxBLACK_PEN);
        } else {
            drawControl.SetPen(wxPen(lredColor));
        }
        drawControl.DrawRectangle(0, 0, meterWSize, peakHSize);
    }
    return peak;
}
    
wxBitmap singleDBMeterPanel::renderMeter(int redraw){
    wxBitmap meter;
    meter.Create(meterWSize, meterHSize);
    wxMemoryDC drawControl(meter);
    drawControl.DrawBitmap(backgroundMeter, 0, 0);
    int peakSize = meterHSize * displayPeak;
    if (peakSize <= greenSize){
        drawControl.SetBrush(wxBrush(greenColor));
        drawControl.SetPen(wxPen(greenColor, 1));
        drawControl.DrawRectangle(0, meterHSize, meterWSize, - peakSize);
    } else if (peakSize <= (greenSize + yellowSize)) {
        drawControl.SetBrush(wxBrush(greenColor));
        drawControl.SetPen(wxPen(greenColor, 1));
        drawControl.DrawRectangle(0, meterHSize , meterWSize, - greenSize);
        drawControl.SetBrush(wxBrush(yellowColor));
        drawControl.SetPen(wxPen(yellowColor, 1));
        drawControl.DrawRectangle(0, meterHSize - greenSize, meterWSize,
                                  - (peakSize - greenSize));
    } else {
        drawControl.SetBrush(wxBrush(greenColor));
        drawControl.SetPen(wxPen(greenColor, 1));
        drawControl.DrawRectangle(0, meterHSize, meterWSize, - greenSize);
        drawControl.SetBrush(wxBrush(yellowColor));
        drawControl.SetPen(wxPen(yellowColor, 1));
        drawControl.DrawRectangle(0, meterHSize - greenSize, meterWSize, - yellowSize);
        drawControl.SetBrush(wxBrush(redColor));
        drawControl.SetPen(wxPen(redColor, 1));
        drawControl.DrawRectangle(0, meterHSize - greenSize - yellowSize,
                                  meterWSize, - (peakSize - greenSize - yellowSize));
    }
    //Draw peak tick
    if (peakSize > lastTickSize || (lastTickRedraw + tickHoldMS) < redraw) {
        lastTickRedraw = redraw;
        lastTickSize = peakSize;
    }
    drawControl.SetBrush(*wxYELLOW_BRUSH);
    drawControl.SetPen(*wxYELLOW_PEN);
    drawControl.DrawLine(0, meterHSize - lastTickSize, meterWSize,
                         meterHSize - lastTickSize);
    //Draw frame
    if (drawFrame){
        drawControl.SetBrush(*wxTRANSPARENT_BRUSH);
        drawControl.SetPen(*wxBLACK_PEN);
        drawControl.DrawRectangle(0, 0, meterWSize, meterHSize);
    }
    return meter;
}
    
void singleDBMeterPanel::calculateDecay(int redraw) {
    if (currentPeak >= displayPeak || displayPeak == 0.f)
        displayPeak = currentPeak;
    else {
        float decay = float(peakDecayRate * (redraw - lastRedraw));
        displayPeak = CLAMP(displayPeak - decay, currentPeak, 1.);
        }
    lastRedraw = redraw;
}
