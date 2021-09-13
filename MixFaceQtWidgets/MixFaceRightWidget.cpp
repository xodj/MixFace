#include "MixFaceRightWidget.h"

MixFaceRightWidget::MixFaceRightWidget(float dpiRatio_,DebugLibrary *debug_,
                                       MixFaceFonts *mf_fonts_, QMainWindow *mainWindow_)
    : QWidget(),
      mainWindow(mainWindow_),
      mf_fonts(mf_fonts_),
      debug(debug_),
      dpiRatio(dpiRatio_)
{
    initRightAreaBar();
    initBusPopup();
    initAssignPopup();
}

void MixFaceRightWidget::initRightAreaBar(){
    this->setMaximumWidth(64*dpiRatio);
    this->setMinimumWidth(64*dpiRatio);
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setAlignment(Qt::AlignCenter);

    busButton = new QPushButton("LR");
    busButton->setMinimumSize(50*dpiRatio,40*dpiRatio);
    busButton->setMaximumSize(50*dpiRatio,40*dpiRatio);
    busButton->setStyleSheet("QPushButton {"
                             "color: rgb(255,255,255);"
                             "background-color: rgb(64, 64, 64);"
                             "border: 2px solid rgb(196,196,196);"
                             "border-radius: 0px;}"
                             "QPushButton:pressed {"
                             "background-color: rgba(128, 128, 128, 255);"
                             "}");
    busButton->setFont(mf_fonts->boldFont);
    vLayout->addWidget(busButton);
    QPushButton::connect(busButton, &QPushButton::clicked, this, &MixFaceRightWidget::busButtonClicked);

    assignButton = new QPushButton("A\nB\nC\nD\nE\nF");
    assignButton->setMinimumSize(50*dpiRatio,150*dpiRatio);
    assignButton->setMaximumSize(50*dpiRatio,150*dpiRatio);
    assignButton->setStyleSheet("QPushButton {"
                                "color: rgb(255,255,255);"
                                "background-color: rgb(64, 64, 64);"
                                "border: 2px solid rgb(196,196,196);"
                                "border-radius: 0px;}"
                                "QPushButton:pressed {"
                                "background-color: rgba(128, 128, 128, 255);"
                                "}");
    assignButton->setMaximumWidth(60);
    assignButton->setInputMethodHints(Qt::ImhMultiLine);
    assignButton->setFont(mf_fonts->boldFont);
    vLayout->addWidget(assignButton);
    QPushButton::connect(assignButton, &QPushButton::clicked,
                         this, &MixFaceRightWidget::assignButtonClicked);

    this->setLayout(vLayout);
    this->setStyleSheet("QWidget {"
                        "border: 2px solid rgb(0,0,0);"
                        "background-color: rgb(96,96,96);"
                        "color: rgb(96,96,96);"
                        "}");
}

void MixFaceRightWidget::initBusPopup(){

    busPopup = new QFrame();
    busPopup->setWindowFlag(Qt::Popup);
    busPopup->setFrameStyle(QAbstractScrollArea::WinPanel);
    busPopup->setFont(mf_fonts->nativeFont);
    busPopup->resize(1,1);
    busPopup->setStyleSheet("QFrame {"
                            "background-color: rgb(96, 96, 96);"
                            "border: 0px solid rgb(0,0,0);"
                            "border-radius: 0px;"
                            "}");

    QVBoxLayout *popupVLayout = new QVBoxLayout(busPopup);
    popupVLayout->setContentsMargins(1, 1, 1, 1);
    popupVLayout->setSpacing(1);

    QHBoxLayout *popupHLayout = new QHBoxLayout();
    popupHLayout->setContentsMargins(0, 0, 0, 0);
    popupHLayout->setSpacing(1);

    QPushButton *busAction = new QPushButton("LR");
    busAction->setProperty("idx",70);
    busAction->setProperty("idy",0);
    busAction->setStyleSheet("QPushButton {"
                             "color: rgb(255,255,255);"
                             "background-color: rgb(64, 64, 64);"
                             "border: 2px solid rgb(196,196,196);"
                             "border-radius: 0px;}"
                             "QPushButton:pressed {"
                             "background-color: rgba(128, 128, 128, 255);"
                             "}");
    busAction->setFont(mf_fonts->boldFont);
    busAction->setMinimumSize(70*dpiRatio,40*dpiRatio);
    busAction->setMaximumSize(70*dpiRatio,40*dpiRatio);
    popupHLayout->addWidget(busAction);
    QPushButton::connect(busAction, &QPushButton::clicked,
                         this, &MixFaceRightWidget::emitAssignMainFader);

    busAction = new QPushButton("M/C");
    busAction->setProperty("idx",71);
    busAction->setProperty("idy",17);
    busAction->setStyleSheet("QPushButton {"
                             "color: rgb(255,255,255);"
                             "background-color: rgb(64, 64, 64);"
                             "border: 2px solid rgb(196,196,196);"
                             "border-radius: 0px;}"
                             "QPushButton:pressed {"
                             "background-color: rgba(128, 128, 128, 255);"
                             "}");
    busAction->setFont(mf_fonts->boldFont);
    busAction->setMinimumSize(70*dpiRatio,40*dpiRatio);
    busAction->setMaximumSize(70*dpiRatio,40*dpiRatio);
    popupHLayout->addWidget(busAction);
    QPushButton::connect(busAction, &QPushButton::clicked,
                         this, &MixFaceRightWidget::emitAssignMainFader);

    popupVLayout->addLayout(popupHLayout);

    popupHLayout = new QHBoxLayout();
    popupHLayout->setContentsMargins(0, 0, 0, 0);
    popupHLayout->setSpacing(1);

    busAction = new QPushButton("Bus 1");
    busAction->setProperty("idx",48);
    busAction->setProperty("idy",1);
    busAction->setStyleSheet("QPushButton {"
                             "color: rgb(255,255,255);"
                             "background-color: rgb(64, 64, 64);"
                             "border: 2px solid rgb(196,196,196);"
                             "border-radius: 0px;}"
                             "QPushButton:pressed {"
                             "background-color: rgba(128, 128, 128, 255);"
                             "}");
    busAction->setFont(mf_fonts->nativeFont);
    busAction->setMinimumSize(70*dpiRatio,35*dpiRatio);
    busAction->setMaximumSize(70*dpiRatio,35*dpiRatio);
    popupHLayout->addWidget(busAction);
    QPushButton::connect(busAction, &QPushButton::clicked,
                         this, &MixFaceRightWidget::emitAssignMainFader);

    busAction = new QPushButton("Bus 2");
    busAction->setProperty("idx",49);
    busAction->setProperty("idy",2);
    busAction->setStyleSheet("QPushButton {"
                             "color: rgb(255,255,255);"
                             "background-color: rgb(64, 64, 64);"
                             "border: 2px solid rgb(196,196,196);"
                             "border-radius: 0px;}"
                             "QPushButton:pressed {"
                             "background-color: rgba(128, 128, 128, 255);"
                             "}");
    busAction->setFont(mf_fonts->nativeFont);
    busAction->setMinimumSize(70*dpiRatio,35*dpiRatio);
    busAction->setMaximumSize(70*dpiRatio,35*dpiRatio);
    popupHLayout->addWidget(busAction);
    QPushButton::connect(busAction, &QPushButton::clicked,
                         this, &MixFaceRightWidget::emitAssignMainFader);

    popupVLayout->addLayout(popupHLayout);

    popupHLayout = new QHBoxLayout();
    popupHLayout->setContentsMargins(0, 0, 0, 0);
    popupHLayout->setSpacing(1);

    busAction = new QPushButton("Bus 3");
    busAction->setProperty("idx",50);
    busAction->setProperty("idy",3);
    busAction->setStyleSheet("QPushButton {"
                             "color: rgb(255,255,255);"
                             "background-color: rgb(64, 64, 64);"
                             "border: 2px solid rgb(196,196,196);"
                             "border-radius: 0px;}"
                             "QPushButton:pressed {"
                             "background-color: rgba(128, 128, 128, 255);"
                             "}");
    busAction->setFont(mf_fonts->nativeFont);
    busAction->setMinimumSize(70*dpiRatio,35*dpiRatio);
    busAction->setMaximumSize(70*dpiRatio,35*dpiRatio);
    popupHLayout->addWidget(busAction);
    QPushButton::connect(busAction, &QPushButton::clicked,
                         this, &MixFaceRightWidget::emitAssignMainFader);

    busAction = new QPushButton("Bus 4");
    busAction->setProperty("idx",51);
    busAction->setProperty("idy",4);
    busAction->setStyleSheet("QPushButton {"
                             "color: rgb(255,255,255);"
                             "background-color: rgb(64, 64, 64);"
                             "border: 2px solid rgb(196,196,196);"
                             "border-radius: 0px;}"
                             "QPushButton:pressed {"
                             "background-color: rgba(128, 128, 128, 255);"
                             "}");
    busAction->setFont(mf_fonts->nativeFont);
    busAction->setMinimumSize(70*dpiRatio,35*dpiRatio);
    busAction->setMaximumSize(70*dpiRatio,35*dpiRatio);
    popupHLayout->addWidget(busAction);
    QPushButton::connect(busAction, &QPushButton::clicked,
                         this, &MixFaceRightWidget::emitAssignMainFader);

    popupVLayout->addLayout(popupHLayout);

    popupHLayout = new QHBoxLayout();
    popupHLayout->setContentsMargins(0, 0, 0, 0);
    popupHLayout->setSpacing(1);

    busAction = new QPushButton("Bus 5");
    busAction->setProperty("idx",52);
    busAction->setProperty("idy",5);
    busAction->setStyleSheet("QPushButton {"
                             "color: rgb(255,255,255);"
                             "background-color: rgb(64, 64, 64);"
                             "border: 2px solid rgb(196,196,196);"
                             "border-radius: 0px;}"
                             "QPushButton:pressed {"
                             "background-color: rgba(128, 128, 128, 255);"
                             "}");
    busAction->setFont(mf_fonts->nativeFont);
    busAction->setMinimumSize(70*dpiRatio,35*dpiRatio);
    busAction->setMaximumSize(70*dpiRatio,35*dpiRatio);
    popupHLayout->addWidget(busAction);
    QPushButton::connect(busAction, &QPushButton::clicked,
                         this, &MixFaceRightWidget::emitAssignMainFader);

    busAction = new QPushButton("Bus 6");
    busAction->setProperty("idx",53);
    busAction->setProperty("idy",6);
    busAction->setStyleSheet("QPushButton {"
                             "color: rgb(255,255,255);"
                             "background-color: rgb(64, 64, 64);"
                             "border: 2px solid rgb(196,196,196);"
                             "border-radius: 0px;}"
                             "QPushButton:pressed {"
                             "background-color: rgba(128, 128, 128, 255);"
                             "}");
    busAction->setFont(mf_fonts->nativeFont);
    busAction->setMinimumSize(70*dpiRatio,35*dpiRatio);
    busAction->setMaximumSize(70*dpiRatio,35*dpiRatio);
    popupHLayout->addWidget(busAction);
    QPushButton::connect(busAction, &QPushButton::clicked,
                         this, &MixFaceRightWidget::emitAssignMainFader);

    popupVLayout->addLayout(popupHLayout);

    popupHLayout = new QHBoxLayout();
    popupHLayout->setContentsMargins(0, 0, 0, 0);
    popupHLayout->setSpacing(1);

    busAction = new QPushButton("Bus 7");
    busAction->setProperty("idx",54);
    busAction->setProperty("idy",7);
    busAction->setStyleSheet("QPushButton {"
                             "color: rgb(255,255,255);"
                             "background-color: rgb(64, 64, 64);"
                             "border: 2px solid rgb(196,196,196);"
                             "border-radius: 0px;}"
                             "QPushButton:pressed {"
                             "background-color: rgba(128, 128, 128, 255);"
                             "}");
    busAction->setFont(mf_fonts->nativeFont);
    busAction->setMinimumSize(70*dpiRatio,35*dpiRatio);
    busAction->setMaximumSize(70*dpiRatio,35*dpiRatio);
    popupHLayout->addWidget(busAction);
    QPushButton::connect(busAction, &QPushButton::clicked,
                         this, &MixFaceRightWidget::emitAssignMainFader);

    busAction = new QPushButton("Bus 8");
    busAction->setProperty("idx",55);
    busAction->setProperty("idy",8);
    busAction->setStyleSheet("QPushButton {"
                             "color: rgb(255,255,255);"
                             "background-color: rgb(64, 64, 64);"
                             "border: 2px solid rgb(196,196,196);"
                             "border-radius: 0px;}"
                             "QPushButton:pressed {"
                             "background-color: rgba(128, 128, 128, 255);"
                             "}");
    busAction->setFont(mf_fonts->nativeFont);
    busAction->setMinimumSize(70*dpiRatio,35*dpiRatio);
    busAction->setMaximumSize(70*dpiRatio,35*dpiRatio);
    popupHLayout->addWidget(busAction);
    QPushButton::connect(busAction, &QPushButton::clicked,
                         this, &MixFaceRightWidget::emitAssignMainFader);

    popupVLayout->addLayout(popupHLayout);

    popupHLayout = new QHBoxLayout();
    popupHLayout->setContentsMargins(0, 0, 0, 0);
    popupHLayout->setSpacing(1);

    busAction = new QPushButton("Bus 9");
    busAction->setProperty("idx",56);
    busAction->setProperty("idy",9);
    busAction->setStyleSheet("QPushButton {"
                             "color: rgb(255,255,255);"
                             "background-color: rgb(64, 64, 64);"
                             "border: 2px solid rgb(196,196,196);"
                             "border-radius: 0px;}"
                             "QPushButton:pressed {"
                             "background-color: rgba(128, 128, 128, 255);"
                             "}");
    busAction->setFont(mf_fonts->nativeFont);
    busAction->setMinimumSize(70*dpiRatio,35*dpiRatio);
    busAction->setMaximumSize(70*dpiRatio,35*dpiRatio);
    popupHLayout->addWidget(busAction);
    QPushButton::connect(busAction, &QPushButton::clicked,
                         this, &MixFaceRightWidget::emitAssignMainFader);

    busAction = new QPushButton("Bus 10");
    busAction->setProperty("idx",57);
    busAction->setProperty("idy",10);
    busAction->setStyleSheet("QPushButton {"
                             "color: rgb(255,255,255);"
                             "background-color: rgb(64, 64, 64);"
                             "border: 2px solid rgb(196,196,196);"
                             "border-radius: 0px;}"
                             "QPushButton:pressed {"
                             "background-color: rgba(128, 128, 128, 255);"
                             "}");
    busAction->setFont(mf_fonts->nativeFont);
    busAction->setMinimumSize(70*dpiRatio,35*dpiRatio);
    busAction->setMaximumSize(70*dpiRatio,35*dpiRatio);
    popupHLayout->addWidget(busAction);
    QPushButton::connect(busAction, &QPushButton::clicked,
                         this, &MixFaceRightWidget::emitAssignMainFader);

    popupVLayout->addLayout(popupHLayout);

    popupHLayout = new QHBoxLayout();
    popupHLayout->setContentsMargins(0, 0, 0, 0);
    popupHLayout->setSpacing(1);

    busAction = new QPushButton("Bus 11");
    busAction->setProperty("idx",58);
    busAction->setProperty("idy",11);
    busAction->setStyleSheet("QPushButton {"
                             "color: rgb(255,255,255);"
                             "background-color: rgb(64, 64, 64);"
                             "border: 2px solid rgb(196,196,196);"
                             "border-radius: 0px;}"
                             "QPushButton:pressed {"
                             "background-color: rgba(128, 128, 128, 255);"
                             "}");
    busAction->setFont(mf_fonts->nativeFont);
    busAction->setMinimumSize(70*dpiRatio,35*dpiRatio);
    busAction->setMaximumSize(70*dpiRatio,35*dpiRatio);
    popupHLayout->addWidget(busAction);
    QPushButton::connect(busAction, &QPushButton::clicked,
                         this, &MixFaceRightWidget::emitAssignMainFader);

    busAction = new QPushButton("Bus 12");
    busAction->setProperty("idx",59);
    busAction->setProperty("idy",12);
    busAction->setStyleSheet("QPushButton {"
                             "color: rgb(255,255,255);"
                             "background-color: rgb(64, 64, 64);"
                             "border: 2px solid rgb(196,196,196);"
                             "border-radius: 0px;}"
                             "QPushButton:pressed {"
                             "background-color: rgba(128, 128, 128, 255);"
                             "}");
    busAction->setFont(mf_fonts->nativeFont);
    busAction->setMinimumSize(70*dpiRatio,35*dpiRatio);
    busAction->setMaximumSize(70*dpiRatio,35*dpiRatio);
    popupHLayout->addWidget(busAction);
    QPushButton::connect(busAction, &QPushButton::clicked,
                         this, &MixFaceRightWidget::emitAssignMainFader);

    popupVLayout->addLayout(popupHLayout);

    popupHLayout = new QHBoxLayout();
    popupHLayout->setContentsMargins(0, 0, 0, 0);
    popupHLayout->setSpacing(1);

    busAction = new QPushButton("Bus 13");
    busAction->setProperty("idx",60);
    busAction->setProperty("idy",13);
    busAction->setStyleSheet("QPushButton {"
                             "color: rgb(255,255,255);"
                             "background-color: rgb(64, 64, 64);"
                             "border: 2px solid rgb(196,196,196);"
                             "border-radius: 0px;}"
                             "QPushButton:pressed {"
                             "background-color: rgba(128, 128, 128, 255);"
                             "}");
    busAction->setFont(mf_fonts->nativeFont);
    busAction->setMinimumSize(70*dpiRatio,35*dpiRatio);
    busAction->setMaximumSize(70*dpiRatio,35*dpiRatio);
    popupHLayout->addWidget(busAction);
    QPushButton::connect(busAction, &QPushButton::clicked,
                         this, &MixFaceRightWidget::emitAssignMainFader);

    busAction = new QPushButton("Bus 14");
    busAction->setProperty("idx",61);
    busAction->setProperty("idy",14);
    busAction->setStyleSheet("QPushButton {"
                             "color: rgb(255,255,255);"
                             "background-color: rgb(64, 64, 64);"
                             "border: 2px solid rgb(196,196,196);"
                             "border-radius: 0px;}"
                             "QPushButton:pressed {"
                             "background-color: rgba(128, 128, 128, 255);"
                             "}");
    busAction->setFont(mf_fonts->nativeFont);
    busAction->setMinimumSize(70*dpiRatio,35*dpiRatio);
    busAction->setMaximumSize(70*dpiRatio,35*dpiRatio);
    popupHLayout->addWidget(busAction);
    QPushButton::connect(busAction, &QPushButton::clicked,
                         this, &MixFaceRightWidget::emitAssignMainFader);

    popupVLayout->addLayout(popupHLayout);

    popupHLayout = new QHBoxLayout();
    popupHLayout->setContentsMargins(0, 0, 0, 0);
    popupHLayout->setSpacing(1);

    busAction = new QPushButton("Bus 15");
    busAction->setProperty("idx",62);
    busAction->setProperty("idy",15);
    busAction->setStyleSheet("QPushButton {"
                             "color: rgb(255,255,255);"
                             "background-color: rgb(64, 64, 64);"
                             "border: 2px solid rgb(196,196,196);"
                             "border-radius: 0px;}"
                             "QPushButton:pressed {"
                             "background-color: rgba(128, 128, 128, 255);"
                             "}");
    busAction->setFont(mf_fonts->nativeFont);
    busAction->setMinimumSize(70*dpiRatio,35*dpiRatio);
    busAction->setMaximumSize(70*dpiRatio,35*dpiRatio);
    popupHLayout->addWidget(busAction);
    QPushButton::connect(busAction, &QPushButton::clicked,
                         this, &MixFaceRightWidget::emitAssignMainFader);

    busAction = new QPushButton("Bus 16");
    busAction->setProperty("idx",63);
    busAction->setProperty("idy",16);
    busAction->setStyleSheet("QPushButton {"
                             "color: rgb(255,255,255);"
                             "background-color: rgb(64, 64, 64);"
                             "border: 2px solid rgb(196,196,196);"
                             "border-radius: 0px;}"
                             "QPushButton:pressed {"
                             "background-color: rgba(128, 128, 128, 255);"
                             "}");
    busAction->setFont(mf_fonts->nativeFont);
    busAction->setMinimumSize(70*dpiRatio,35*dpiRatio);
    busAction->setMaximumSize(70*dpiRatio,35*dpiRatio);
    popupHLayout->addWidget(busAction);
    QPushButton::connect(busAction, &QPushButton::clicked,
                         this, &MixFaceRightWidget::emitAssignMainFader);

    popupVLayout->addLayout(popupHLayout);
}

void MixFaceRightWidget::initAssignPopup(){
    assignPopup = new QFrame();
    assignPopup->setWindowFlag(Qt::Popup);
    assignPopup->setFrameStyle(QAbstractScrollArea::WinPanel);
    assignPopup->setFont(mf_fonts->nativeFont);
    assignPopup->resize(1,1);
    assignPopup->setStyleSheet("QFrame {"
                               "background-color: rgb(96, 96, 96);"
                               "border: 0px solid rgb(0,0,0);"
                               "border-radius: 0px;"
                               "}");

    QVBoxLayout *popupVLayout = new QVBoxLayout(assignPopup);
    popupVLayout->setContentsMargins(1, 1, 1, 1);
    popupVLayout->setSpacing(1);

    QPushButton *busAction = new QPushButton("All");
    busAction->setProperty("idy",0);
    busAction->setStyleSheet("QPushButton {"
                             "color: rgb(255,255,255);"
                             "background-color: rgb(64, 64, 64);"
                             "border: 2px solid rgb(196,196,196);"
                             "border-radius: 0px;}"
                             "QPushButton:pressed {"
                             "background-color: rgba(128, 128, 128, 255);"
                             "}");
    busAction->setFont(mf_fonts->boldFont);
    busAction->setMinimumHeight(40*dpiRatio);
    busAction->setMaximumHeight(40*dpiRatio);
    popupVLayout->addWidget(busAction);
    QPushButton::connect(busAction, &QPushButton::clicked,
                         this, &MixFaceRightWidget::emitAssignScrollFaders);

    QHBoxLayout *popupHLayout = new QHBoxLayout();
    popupHLayout->setContentsMargins(0, 0, 0, 0);
    popupHLayout->setSpacing(1);

    busAction = new QPushButton("A");
    busAction->setProperty("idy",1);
    busAction->setStyleSheet("QPushButton {"
                             "color: rgb(255,255,255);"
                             "background-color: rgb(64, 64, 64);"
                             "border: 2px solid rgb(196,196,196);"
                             "border-radius: 0px;}"
                             "QPushButton:pressed {"
                             "background-color: rgba(128, 128, 128, 255);"
                             "}");
    busAction->setFont(mf_fonts->boldFont);
    busAction->setMinimumSize(70*dpiRatio,40*dpiRatio);
    busAction->setMaximumSize(70*dpiRatio,40*dpiRatio);
    popupHLayout->addWidget(busAction);
    QPushButton::connect(busAction, &QPushButton::clicked,
                         this, &MixFaceRightWidget::emitAssignScrollFaders);

    busAction = new QPushButton("B");
    busAction->setProperty("idy",2);
    busAction->setStyleSheet("QPushButton {"
                             "color: rgb(255,255,255);"
                             "background-color: rgb(64, 64, 64);"
                             "border: 2px solid rgb(196,196,196);"
                             "border-radius: 0px;}"
                             "QPushButton:pressed {"
                             "background-color: rgba(128, 128, 128, 255);"
                             "}");
    busAction->setFont(mf_fonts->boldFont);
    busAction->setMinimumSize(70*dpiRatio,40*dpiRatio);
    busAction->setMaximumSize(70*dpiRatio,40*dpiRatio);
    popupHLayout->addWidget(busAction);
    QPushButton::connect(busAction, &QPushButton::clicked,
                         this, &MixFaceRightWidget::emitAssignScrollFaders);

    popupVLayout->addLayout(popupHLayout);

    popupHLayout = new QHBoxLayout();
    popupHLayout->setContentsMargins(0, 0, 0, 0);
    popupHLayout->setSpacing(1);

    busAction = new QPushButton("C");
    busAction->setProperty("idy",3);
    busAction->setStyleSheet("QPushButton {"
                             "color: rgb(255,255,255);"
                             "background-color: rgb(64, 64, 64);"
                             "border: 2px solid rgb(196,196,196);"
                             "border-radius: 0px;}"
                             "QPushButton:pressed {"
                             "background-color: rgba(128, 128, 128, 255);"
                             "}");
    busAction->setFont(mf_fonts->boldFont);
    busAction->setMinimumSize(70*dpiRatio,40*dpiRatio);
    busAction->setMaximumSize(70*dpiRatio,40*dpiRatio);
    popupHLayout->addWidget(busAction);
    QPushButton::connect(busAction, &QPushButton::clicked,
                         this, &MixFaceRightWidget::emitAssignScrollFaders);

    busAction = new QPushButton("D");
    busAction->setProperty("idy",4);
    busAction->setStyleSheet("QPushButton {"
                             "color: rgb(255,255,255);"
                             "background-color: rgb(64, 64, 64);"
                             "border: 2px solid rgb(196,196,196);"
                             "border-radius: 0px;}"
                             "QPushButton:pressed {"
                             "background-color: rgba(128, 128, 128, 255);"
                             "}");
    busAction->setFont(mf_fonts->boldFont);
    busAction->setMinimumSize(70*dpiRatio,40*dpiRatio);
    busAction->setMaximumSize(70*dpiRatio,40*dpiRatio);
    popupHLayout->addWidget(busAction);
    QPushButton::connect(busAction, &QPushButton::clicked,
                         this, &MixFaceRightWidget::emitAssignScrollFaders);

    popupVLayout->addLayout(popupHLayout);

    popupHLayout = new QHBoxLayout();
    popupHLayout->setContentsMargins(0, 0, 0, 0);
    popupHLayout->setSpacing(1);
    busAction = new QPushButton("E");
    busAction->setProperty("idy",5);
    busAction->setStyleSheet("QPushButton {"
                             "color: rgb(255,255,255);"
                             "background-color: rgb(64, 64, 64);"
                             "border: 2px solid rgb(196,196,196);"
                             "border-radius: 0px;}"
                             "QPushButton:pressed {"
                             "background-color: rgba(128, 128, 128, 255);"
                             "}");
    busAction->setFont(mf_fonts->boldFont);
    busAction->setMinimumSize(70*dpiRatio,40*dpiRatio);
    busAction->setMaximumSize(70*dpiRatio,40*dpiRatio);
    popupHLayout->addWidget(busAction);
    QPushButton::connect(busAction, &QPushButton::clicked,
                         this, &MixFaceRightWidget::emitAssignScrollFaders);

    busAction = new QPushButton("F");
    busAction->setProperty("idy",6);
    busAction->setStyleSheet("QPushButton {"
                             "color: rgb(255,255,255);"
                             "background-color: rgb(64, 64, 64);"
                             "border: 2px solid rgb(196,196,196);"
                             "border-radius: 0px;}"
                             "QPushButton:pressed {"
                             "background-color: rgba(128, 128, 128, 255);"
                             "}");
    busAction->setFont(mf_fonts->boldFont);
    busAction->setMinimumSize(70*dpiRatio,40*dpiRatio);
    busAction->setMaximumSize(70*dpiRatio,40*dpiRatio);
    popupHLayout->addWidget(busAction);
    QPushButton::connect(busAction, &QPushButton::clicked,
                         this, &MixFaceRightWidget::emitAssignScrollFaders);

    popupVLayout->addLayout(popupHLayout);

    popupHLayout = new QHBoxLayout();
    popupHLayout->setContentsMargins(0, 0, 0, 0);
    popupHLayout->setSpacing(1);

    busAction = new QPushButton("Config");
    busAction->setStyleSheet("QPushButton {"
                             "color: rgb(255,255,255);"
                             "background-color: rgb(64, 64, 64);"
                             "border: 2px solid rgb(196,196,196);"
                             "border-radius: 0px;}"
                             "QPushButton:pressed {"
                             "background-color: rgba(128, 128, 128, 255);"
                             "}");
    busAction->setFont(mf_fonts->italicFont);
    busAction->setMinimumHeight(40*dpiRatio);
    busAction->setMaximumHeight(40*dpiRatio);
    popupHLayout->addWidget(busAction);
    QPushButton::connect(busAction, &QPushButton::clicked,
                         this, &MixFaceRightWidget::configButtonClicked);

    popupVLayout->addLayout(popupHLayout);
}

void MixFaceRightWidget::busButtonClicked(){
    if (busPopup->geometry().width()<10) {
        busPopup->show();
        busPopup->hide();
    }
    QRect rect = busPopup->geometry();
    const QRect buttonGeometry = busButton->geometry();
    const QRect windowGeometry = mainWindow->geometry();
    int height = rect.height();
    int width = rect.width();
    rect.setX(windowGeometry.right()-buttonGeometry.left()-width+1);
    rect.setY(buttonGeometry.top()+windowGeometry.top()+(32*dpiRatio));
    rect.setWidth(width);
    rect.setHeight(height);
    busPopup->setGeometry(rect);

    if (busPopup->isHidden()) busPopup->show();
}

void MixFaceRightWidget::assignButtonClicked(){
    if (assignPopup->geometry().width()<10) {
        assignPopup->show();
        assignPopup->hide();
    }

    QRect rect = assignPopup->geometry();
    const QRect buttonGeometry = assignButton->geometry();
    const QRect windowGeometry = mainWindow->geometry();
    int height = rect.height();
    int width = rect.width();
    rect.setX(windowGeometry.right()-buttonGeometry.left()-width+1);
    rect.setY(buttonGeometry.top()+windowGeometry.top()+(32*dpiRatio));
    rect.setWidth(width);
    rect.setHeight(height);
    assignPopup->setGeometry(rect);

    if (assignPopup->isHidden()) assignPopup->show();
}

void MixFaceRightWidget::configButtonClicked(){}

void MixFaceRightWidget::emitAssignMainFader(){
    int idx = reinterpret_cast<QAction *>(sender())->property("idx").toInt();
    int idy = reinterpret_cast<QAction *>(sender())->property("idy").toInt();
    emit assignMainFader(idx, idy);
}

void MixFaceRightWidget::emitAssignScrollFaders(){
    int idy = reinterpret_cast<QAction *>(sender())->property("idy").toInt();
    emit assignScrollFaders(idy);
}
