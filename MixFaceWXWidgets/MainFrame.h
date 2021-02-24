#ifndef MAINFRAME_H
#define MAINFRAME_H
#include "dbMeter.h"
#include "wxcrafter.h"

class MainFrame : public MainFrameBaseClass
{
public:
    MainFrame(wxWindow* parent);
    virtual ~MainFrame();

private:
    void sliderUpdated(wxCommandEvent& event);

    singleDBMeterPanel * drawPane;
    int value = 0;
    meterDemoTimer *m_demo_t;
};
#endif // MAINFRAME_H
