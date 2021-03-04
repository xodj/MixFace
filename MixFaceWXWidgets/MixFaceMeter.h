#include <wx/dcmemory.h>
#include <wx/wx.h>
#include <wx/timer.h>
#include <chrono>

#define CLAMP(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#define peakDecayRate .000365f
#define peakHoldMS 3000
#define tickHoldMS 1500

class singleDBMeterPanel : public wxPanel {
public:    
    singleDBMeterPanel(wxWindow *parent,
                       wxWindowID winid = wxID_ANY,
                       const wxPoint& pos = wxDefaultPosition,
                       const wxSize& size = wxDefaultSize,
                       long style = wxTAB_TRAVERSAL | wxNO_BORDER,
                       const wxString& name = wxPanelNameStr);
    ~singleDBMeterPanel();
    void repaint() { paintEvent(); }

    void setValue(float value_) {
        if (value_ > 1) value_ = 1.f;
        if (value_ < 0) value_ = 0.f;
        currentPeak = value_;
    }

private:

    void paintEvent() {
        wxClientDC dc(this);
        render(dc);
    }
    
    void paintEvent(wxPaintEvent& evt) {
        __UNUSED_PARAM(evt)
        wxClientDC dc(this);
        render(dc);
    }
    
    void paintEvent(wxSizeEvent& evt) {
        __UNUSED_PARAM(evt)
        wxClientDC dc(this);
        render(dc);
    }

    void render(wxDC& drawControl);
    void renderBackground();
    wxBitmap renderPeak(int redraw);
    wxBitmap renderMeter(int redraw);
    void calculateDecay(int redraw);

    float redAspect = 0.1f;
    float yellowAspect = 0.15f;
    int redSize;
    int yellowSize;
    int greenSize;
    int meterHSize;
    wxColor bgColor = wxColor(227, 227, 227);
    wxColor blackColor = wxColor(0, 0, 0);
    wxColor greenColor = wxColor(55, 239, 70);
    wxColor yellowColor = wxColor(245, 209, 70);
    wxColor redColor = wxColor(245, 70, 70);
    wxColor lgreenColor = wxColor(45, 137, 52);
    wxColor lyellowColor = wxColor(140, 122, 52);
    wxColor lredColor = wxColor(140, 52, 52);
    int panelWSize;
    int panelHSize = -1;
    int borders = 0;
    int meterWSize = 16;
    int peakHSize = 8;
    float currentPeak = 0.f;
    float displayPeak = 0.f;
    int lastRedraw = 0;
    int lastTickRedraw = 0;
    int lastTickSize = 0;
    float lastTickPeak = 0.f;
    int lastPeakRedraw = 0;
    wxBitmap backgroundFill;
    wxBitmap backgroundMeter;
    int frame = 0;
    bool drawFrame = true;
};

class meterDemoTimer : public wxTimer {
private:
    singleDBMeterPanel *m_meter;
    float demoValue = .0f;
    bool range = true;
    
public:
    inline meterDemoTimer(singleDBMeterPanel *m_meter_) : wxTimer() {
        Init();
        SetOwner(this);
        m_meter = m_meter_;
    }
    
    virtual void Notify()
    {
        if (demoValue >= 1.f) {
            range = false;
        } else if (demoValue <= 0.f) {
            range = true;
        }
        
        m_meter->setValue(demoValue);
        
        if (range == true)
            demoValue += 0.01f;
        else
            demoValue -= 0.01f;
    }
};

class meterRenewTimer : public wxTimer {
private:
    singleDBMeterPanel *m_meter;
    
public:
    inline meterRenewTimer(singleDBMeterPanel *m_meter_) : wxTimer() {
        Init();
        SetOwner(this);
        m_meter = m_meter_;
    }
    
    virtual void Notify(){ m_meter->repaint(); }
};
