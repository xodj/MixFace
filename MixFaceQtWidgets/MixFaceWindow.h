#ifndef MIXFACEWINDOW_H
#define MIXFACEWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QTimer>
#include <cmath>
#include "MixFaceFaderWidget.h"
#include "MixFaceSourceWidget.h"
#include "MixFaceDynamicsWidget.h"
#include "MixFaceEqualizerWidget.h"
#include "MixFaceFonts.h"
#include "MixFaceEnumTypes.h"
#include "MixFaceLibrary.h"
#include "MixFaceLinker.h"
#include "MixFaceListener.h"
#include "DebugLibrary.hpp"
#include "MixFaceTopWidget.h"
#include "MixFaceRightWidget.h"
#include "MixFaceIconPicker.h"

class MixFaceDemoTimer;

class ReturnThread;

class MixFaceWindow : public QMainWindow
{
    Q_OBJECT

public:
    MixFaceWindow(DebugLibrary *debug_);
    ~MixFaceWindow();

signals:
    void initiated();
    void resizeSignal();

private:
    void initUI();
    void connection();

    void slotConnected(bool state);
    void threadConnected(bool state);

    void slotValueChanged(int imtype, int idx, int idy);
    void threadValueChanged(int imtype, int idx, int idy);

    void slotMeter1(float *array);
    void threadMeter1(float *array);
    void slotMeter2(float *array);
    void threadMeter2(float *array);
    void slotMeter3(float *array);
    void threadMeter3(float *array);

    QScrollBar *bar;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QScrollArea *scrollArea;
    QScrollArea *srcArea;
    QHBoxLayout *srcLayout;
    QScrollArea *mainArea;

    MixFaceTopWidget *mf_topArea;
    MixFaceRightWidget *mf_rightArea;
    QWidget *controlAreaWidget;
    QWidget *mainAreaWidget;

    FaderWidget *scrollWidget[80];
    FaderWidget *mainWidget[80];

    MixFaceIconPicker *mf_picker;

    FaderWidget *srcFader;
    SourceWidget *srcWidget;
    DynamicsWidget *dynWidget;
    EqualizerWidget *eqWidget;

    QPushButton *busButton;
    QFrame *busPopup;
    QPushButton *assignButton;
    QFrame *assignPopup;

    bool connected = false;

    int currentIdy = 0;
    MixMode m_mode = mCtrl;
    SendMode s_mode = sendLR;
    bool assignedFader[80][7];
    AssignedFaderMode af_mode = ALL;

    void muteClicked(int value);
    void panChanged(float value);
    void faderChanged(float value);
    void soloClicked(int value);

    void initControlAreaWidgets();
    void initControlAreaWidgetIdx(int idx, FaderType ftype);

    void initMainAreaWidgets();
    void initMainAreaWidgetIdx(int idx);

    void assignMainFader(int idx, int idy);
    void assignScrollFaders(int idx);

    void iconButtonClicked();
    void logoChanged(int idx, int value);
    void colorChanged(int idx, int value);
    void nameChanged(int idx, QString value);
    void buttonSrcClicked();
    void buttonEqClicked();
    void buttonDynClicked();

    void windowRenew();
    void sendSyncMessages(){ mf_library->threadSendSyncMessages(); }

    float dpiRatio;

    MixFaceLibrary *mf_library;
    EnumTypes mf_types;
    MixFaceFonts *mf_fonts;
    DebugLibrary *debug;
    QApplication *mixFace;
    MixFaceMetersTimer *mf_metersTimer;
    MixFaceDemoTimer *mf_demoTimer;
    ReturnThread *returnThread;

protected:
    void resizeEvent( QResizeEvent *e ) override;
};

class MixFaceDemoTimer : public QTimer {
    Q_OBJECT

public:

    void AddFader(FaderWidget *meter) {
        faders.push_back(meter);
    }

    void RemoveFader(FaderWidget *meter) {
        faders.removeOne(meter);
    }
    bool connected = false;

protected:
    void timerEvent(QTimerEvent *event) override {
        Q_UNUSED(event)
        if(!connected){
            for(int i = 0;i < 4;i++){
                if(demoissum[i]&&demoStep[i]==1)
                    demoissum[i] = false;
                else
                    if (!demoissum[i]&&demoStep[i]==-110)
                        demoissum[i] = true;

                if(demoissum[i])
                    demoStep[i] += 0.25;
                else
                    demoStep[i] -= 0.25;
                demoValue[i] = pow(10,demoStep[i]/20);
            }

            for(FaderWidget *fader:faders)
                fader->setMeter(demoValue[0], demoValue[1]);
        } else
            this->ResetValues();
    }

    void ResetValues(){
        for(FaderWidget *fader:faders)
            fader->setMeter(.0f, .0f);
        this->stop();
    }

    float demoStep[4] = {-90, -60, -45, -15};
    bool demoissum[4] = {true, true, true, true};
    float demoValue[4] = {.0f, .0f, .0f, .0f};

    QList<FaderWidget *> faders;
};

class ReturnThread : public QThread {
    Q_OBJECT
public:
    ReturnThread(QObject *parent = nullptr) : QThread(parent){};
    void connectedSlot(bool state){
        emit connectedSignal(state);
    };
    void valueSlot(int imtype, int idx, int idy){
        emit valueSignal(imtype, idx, idy);
    };
    void meter1Slot(float *array){
        emit meter1Signal(array);
    };
    void meter2Slot(float *array){
        emit meter2Signal(array);
    };
    void meter3Slot(float *array){
        emit meter3Signal(array);
    };

signals:
    void connectedSignal(bool state);
    void valueSignal(int imtype, int idx, int idy);
    void meter1Signal(float *array);
    void meter2Signal(float *array);
    void meter3Signal(float *array);
};
#endif
