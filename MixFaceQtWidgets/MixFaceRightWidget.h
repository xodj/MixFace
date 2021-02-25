#ifndef MIXFACERIGHTWIDGET_H
#define MIXFACERIGHTWIDGET_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QFrame>
#include <QVBoxLayout>
#include <QAbstractScrollArea>
#include <QAction>
#include "MixFaceEnumTypes.h"
#include "MixFaceFonts.h"
#include "DebugLibrary.hpp"

class MixFaceRightWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MixFaceRightWidget(float dpiRatio_,DebugLibrary *debug_, MixFaceFonts *mf_fonts_, QMainWindow *mainWindow_);

    QPushButton *busButton;
    QPushButton *assignButton;

signals:
    void assignMainFader(int idx, int idy);
    void assignScrollFaders(int idy);

private:
    void initRightAreaBar();
    void initBusPopup();
    void initAssignPopup();

    void busButtonClicked();
    void assignButtonClicked();
    void configButtonClicked();

    void emitAssignMainFader();
    void emitAssignScrollFaders();

    QMainWindow *mainWindow;
    MixFaceFonts *mf_fonts;
    DebugLibrary *debug;
    float dpiRatio = 1;

    QFrame *busPopup;
    QFrame *assignPopup;

    int currentIdy = 0;
    MixMode m_mode = mCtrl;
    SendMode s_mode = sendLR;
    bool assignedFader[80][7];
    AssignedFaderMode af_mode = ALL;
};

#endif // MIXFACERIGHTWIDGET_H
