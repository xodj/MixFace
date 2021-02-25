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
#include "DebugLibrary.h"
#include "MixFaceTopWidget.h"
#include "MixFaceRightWidget.h"
#include "MixFaceIconPicker.h"

class MixFaceWindow : public QMainWindow
{
    Q_OBJECT

public:
    MixFaceWindow(QApplication *mixFace_, DebugLibrary *debug_);
    ~MixFaceWindow();

signals:
    void initiated();
    void resizeSignal();

public slots:
    void valueChanged(int imtype, int idx, int idy);


private:
    void connection();
    void initUI();

    QScrollBar *bar;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QScrollArea *topArea;
    QScrollArea *scrollArea;
    QScrollArea *srcArea;
    QHBoxLayout *srcLayout;
    QScrollArea *mainArea;
    QScrollArea *rightArea;

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

    void initRightAreaBar();
    void initTopAreaBar();

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

    void metersDemo();
    float demoStep = -90;
    bool demo = true;
    bool demoissum = true;

    float dpiRatio;

    MixFaceLibrary *mf_library;
    EnumTypes mf_types;
    MixFaceFonts *mf_fonts;
    DebugLibrary *debug;
    QApplication *mixFace;

protected:
    void resizeEvent( QResizeEvent *e ) override;
};
#endif
