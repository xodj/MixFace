#include "MixFaceDynamicsWidget.h"
#include "MixFaceStatic.h"

#include <QDebug>

DynamicsWidget::DynamicsWidget(float dpiRatio, MixFaceFonts *m_fonts_)
    : m_fonts(m_fonts_), dpiRatio(dpiRatio)
{
    //QSS
    lineStyle = "QLineEdit {"
                "color: rgb(255,255,255);"
                "background-color: rgb(128, 128, 128);"
                "border: 1px solid rgb(32,32,32);"
                "border-radius: 0px;"
                "}"
                "QLineEdit::disabled {"
                "color: rgb(128,128,128);"
                "background-color: rgb(64, 64, 64);"
                "border: 1px solid rgb(128,128,128);"
                "border-radius: 0px;"
                "}";
    sliderStyle = "QSlider {"
                  "background-color: rgba(0, 0, 0, 0);"
                  "}"
                  "QSlider::groove:vertical\n"
                  "{\n"
                  "background-color: rgba(0, 0, 0, 0);\n"
                  "border:none;\n"
                  "width: 8px;\n"
                  "}\n"
                  "\n"
                  "QSlider::sub-page\n"
                  "{\n"
                  "background: rgb(64, 64, 64);\n"
                  "border: 1px solid rgb(0,0,0);\n"
                  "border-radius: 4px;\n"
                  "margin-top: " + QString::number(11*dpiRatio) + "px;\n"
                  "}\n"
                  "QSlider::add-page\n"
                  "{\n"
                  "background: rgb(96, 96, 255);\n"
                  "border: 1px solid rgb(0,0,0);\n"
                  "border-radius: 4px;\n"
                  "margin-bottom: " + QString::number(11*dpiRatio) + "px;\n"
                  "}\n"
                  "QSlider::handle\n"
                  "{\n"
                  "background-color: qlineargradient(spread:pad, x1:0, y1:0.006, x2:0, y2:1, stop:0.025 rgba(96, 96, 96, 255), stop:0.065 rgba(64, 64, 64, 255), stop:0.1 rgba(96, 96, 96, 255), stop:0.48 rgba(196, 196, 196, 255), stop:0.481 rgba(0, 0, 0, 255), stop:0.519 rgba(0, 0, 0, 255), stop:0.52 rgba(196, 196, 196, 255), stop:0.9 rgba(96, 96, 96, 255), stop:0.935 rgba(64, 64, 64, 255), stop:0.975 rgba(96, 96, 96, 255));\n"
                  "border: 1px solid rgb(32,32,32);\n"
                  "border-radius: 4px;\n"
                  "height: " + QString::number(22*dpiRatio) + "px;\n"
                  "margin: 0 -" + QString::number(9*dpiRatio) + "px;\n"
                  "}"
                  "QSlider::handle:pressed\n"
                  "{\n"
                  "background-color: qlineargradient(spread:pad, x1:0, y1:0.006, x2:0, y2:1, stop:0.025 rgba(96, 96, 96, 255), stop:0.065 rgba(64, 64, 64, 255), stop:0.1 rgba(96, 96, 96, 255), stop:0.48 rgba(196, 196, 196, 255), stop:0.481 rgba(0, 0, 0, 255), stop:0.519 rgba(0, 0, 0, 255), stop:0.52 rgba(196, 196, 196, 255), stop:0.9 rgba(96, 96, 96, 255), stop:0.935 rgba(64, 64, 64, 255), stop:0.975 rgba(96, 96, 96, 255));\n"
                  "border: 1px solid rgb(223,223,223);\n"
                  "border-radius: 4px;\n"
                  "height: " + QString::number(22*dpiRatio) + "px;\n"
                  "margin: 0 -" + QString::number(9*dpiRatio) + "px;\n"
                  "}"
                  "QSlider::sub-page:disabled\n"
                  "{\n"
                  "background: rgb(32, 32, 32);\n"
                  "border: 1px solid rgb(128,128,128);\n"
                  "border-radius: 4px;\n"
                  "margin-top: " + QString::number(11*dpiRatio) + "px;\n"
                  "}\n"
                  "QSlider::add-page:disabled\n"
                  "{\n"
                  "background: rgb(48, 48, 128);\n"
                  "border: 1px solid rgb(128,128,128);\n"
                  "border-radius: 4px;\n"
                  "margin-bottom: " + QString::number(11*dpiRatio) + "px;\n"
                  "}\n"
                  "QSlider::handle:disabled\n"
                  "{\n"
                  "background-color: qlineargradient(spread:pad, x1:0, y1:0.006, x2:0, y2:1, stop:0.025 rgba(48, 48, 48, 255), stop:0.065 rgba(32, 32, 32, 255), stop:0.1 rgba(48, 48, 48, 255), stop:0.48 rgba(98, 98, 98, 255), stop:0.481 rgba(0, 0, 0, 255), stop:0.519 rgba(0, 0, 0, 255), stop:0.52 rgba(98, 98, 98, 255), stop:0.9 rgba(48, 48, 48, 255), stop:0.935 rgba(32, 32, 32, 255), stop:0.975 rgba(48, 48, 48, 255));\n"
                  "border: 1px solid rgb(128,128,128);\n"
                  "border-radius: 4px;\n"
                  "height: " + QString::number(22*dpiRatio) + "px;\n"
                  "margin: 0 -" + QString::number(9*dpiRatio) + "px;\n"
                  "}";
    buttonStyle = "QPushButton {"
                  "color: rgb(0,0,0);"
                  "background-color: rgb(196, 196, 196);"
                  "border: 1px solid rgb(32,32,32);"
                  "border-radius: 0px;"
                  "}"
                  "QPushButton:pressed {"
                  "background-color: rgb(128, 128, 128);"
                  "}"
                  "QPushButton:checked {"
                  "background-color: rgb(128, 255, 128);"
                  "}"
                  "QPushButton:checked:pressed {"
                  "background-color: rgb(128, 196, 128);"
                  "}";

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(0, 0, 0, 0);
    vlayout->setSpacing(0);

    QFrame *compFrame = compWidget();
    vlayout->addWidget(compFrame);

    QFrame *gateFrame = gateWidget();
    vlayout->addWidget(gateFrame);

    this->setLayout(vlayout);
    this->setStyleSheet("background-color: rgb(96, 96, 96);");

    connectSignals();

    tablesRenew = new tablesTimer;
    tablesRenew->addGainPaint(compGain);
    tablesRenew->addGainPaint(gateGain);
    tablesRenew->start(30);
}

DynamicsWidget::~DynamicsWidget(){
    disconnectSignals();
    tablesRenew->stop();
    delete tablesRenew;
}

QFrame *DynamicsWidget::compWidget(){
    QFrame *frame = new QFrame;
    frame->setFrameShape(QFrame::Box);
    frame->setFrameShadow(QFrame::Plain);
    frame->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,
                                     QSizePolicy::MinimumExpanding));

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->setSpacing(0);
    //Activate Button
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(8, 16, 8, 16);
    vlayout->setSpacing(4);

    compActivePushButton = new QPushButton("Active");
    compActivePushButton->setCheckable(true);
    compActivePushButton->setMinimumSize(96*dpiRatio, 36*dpiRatio);
    compActivePushButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    compActivePushButton->setStyleSheet("QPushButton {"
                          "color: rgb(0,0,0);"
                          "background-color: rgb(196, 196, 196);"
                          "border: 2px solid rgb(32,32,32);"
                          "border-radius: 0px;}"
                          "QPushButton:pressed {"
                          "background-color: rgb(128, 128, 128);}"
                          "QPushButton:checked {"
                          "background-color: rgb(128, 255, 128);}"
                          "QPushButton:checked:pressed {"
                          "background-color: rgb(128, 196, 128);}");
    compActivePushButton->setFont(m_fonts->boldFont);
    vlayout->addWidget(compActivePushButton);

    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    vlayout->addItem(verticalSpacer);

    vlayout->addWidget(compRadioWidget());

    hlayout->addLayout(vlayout);
    //Comp graphics
    QHBoxLayout *hlayout_ = new QHBoxLayout;
    hlayout_->setContentsMargins(10, 10, 10, 10);
    hlayout_->setSpacing(0);
    compGain = new compGainPaint(dpiRatio);
    compGain->setSizePolicy(QSizePolicy::MinimumExpanding,
                            QSizePolicy::MinimumExpanding);
    hlayout_->addWidget(compGain);
    hlayout->addLayout(hlayout_);

    hlayout->addWidget(compGainWidget());
    hlayout->addWidget(compEnvWidget());
    hlayout->addWidget(compFltrWidget());

    frame->setLayout(hlayout);
    return frame;
}

QWidget *DynamicsWidget::compRadioWidget(){
    QWidget *widget = new QWidget;

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(4, 4, 4, 4);
    vlayout->setSpacing(4);
    //Comp/EXP
    QHBoxLayout *radiohlayout = new QHBoxLayout;
    radiohlayout->setContentsMargins(0, 0, 0, 0);
    radiohlayout->setSpacing(0);
    compType = new QButtonGroup;
    compTypeRadio = new QRadioButton("Comp");
    compTypeRadio->setStyleSheet("QRadioButton {color: rgb(255,255,255);}");
    compTypeRadio->setFont(m_fonts->boldFont8);
    compType->addButton(compTypeRadio);
    compTypeRadio->setChecked(true);
    radiohlayout->addWidget(compTypeRadio);
    expTypeRadio = new QRadioButton("Exp");
    expTypeRadio->setStyleSheet("QRadioButton {color: rgb(255,255,255);}");
    expTypeRadio->setFont(m_fonts->boldFont8);
    compType->addButton(expTypeRadio);
    radiohlayout->addWidget(expTypeRadio);

    vlayout->addLayout(radiohlayout);
    //Lin/Log
    radiohlayout = new QHBoxLayout;
    radiohlayout->setContentsMargins(0, 0, 0, 0);
    radiohlayout->setSpacing(0);
    compLinLogType = new QButtonGroup;
    compLinRadio = new QRadioButton("Lin");
    compLinRadio->setStyleSheet("QRadioButton {color: rgb(255,255,255);}");
    compLinRadio->setFont(m_fonts->boldFont8);
    compLinLogType->addButton(compLinRadio);
    radiohlayout->addWidget(compLinRadio);
    compLogRadio = new QRadioButton("Log");
    compLogRadio->setStyleSheet("QRadioButton {color: rgb(255,255,255);}");
    compLogRadio->setFont(m_fonts->boldFont8);
    compLinLogType->addButton(compLogRadio);
    compLogRadio->setChecked(true);
    radiohlayout->addWidget(compLogRadio);

    vlayout->addLayout(radiohlayout);
    //PEAK/RMS
    radiohlayout = new QHBoxLayout;
    radiohlayout->setContentsMargins(0, 0, 0, 0);
    radiohlayout->setSpacing(0);
    compPeakRmsType = new QButtonGroup;
    compPeakRadio = new QRadioButton("Peak");
    compPeakRadio->setStyleSheet("QRadioButton {color: rgb(255,255,255);}");
    compPeakRadio->setFont(m_fonts->boldFont8);
    compPeakRmsType->addButton(compPeakRadio);
    compPeakRadio->setChecked(true);
    radiohlayout->addWidget(compPeakRadio);
    compRmsRadio = new QRadioButton("RMS");
    compRmsRadio->setStyleSheet("QRadioButton {color: rgb(255,255,255);}");
    compRmsRadio->setFont(m_fonts->boldFont8);
    compPeakRmsType->addButton(compRmsRadio);
    radiohlayout->addWidget(compRmsRadio);

    vlayout->addLayout(radiohlayout);

    widget->setLayout(vlayout);
    return widget;
}

QWidget *DynamicsWidget::compGainWidget(){
    QWidget *widget = new QWidget;
    widget->setSizePolicy(QSizePolicy::MinimumExpanding,
                            QSizePolicy::Minimum);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(4, 4, 4, 4);
    vlayout->setSpacing(4);

    QLabel *label = new QLabel("Gain");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setAlignment(Qt::AlignCenter);
    vlayout->addWidget(label);

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setContentsMargins(4, 4, 4, 4);
    hlayout->setSpacing(4);
    //Knee
    QVBoxLayout *vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);

    kneeLine = new QLineEdit("0");
    kneeLine->setStyleSheet(lineStyle);
    kneeLine->setAlignment(Qt::AlignCenter);
    kneeLine->setMinimumWidth(48*dpiRatio);
    kneeLine->setMaximumWidth(48*dpiRatio);
    kneeLine->setFont(m_fonts->boldFont8);
    vlayout_->addWidget(kneeLine);

    QHBoxLayout *sliderslayout = new QHBoxLayout;
    sliderslayout->setContentsMargins(0, 0, 0, 0);
    sliderslayout->setSpacing(0);
    kneeSlider = new QSlider;
    kneeSlider->setOrientation(Qt::Orientation::Vertical);
    kneeSlider->setMinimumSize(32*dpiRatio, 32*dpiRatio);
    kneeSlider->setStyleSheet(sliderStyle);
    kneeSlider->setMaximum(5);
    kneeSlider->setValue(0);
    kneeSlider->setPageStep(0);
    sliderslayout->addWidget(kneeSlider);
    vlayout_->addLayout(sliderslayout);

    label = new QLabel("Knee");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setAlignment(Qt::AlignCenter);
    vlayout_->addWidget(label);

    hlayout->addLayout(vlayout_);
    //Treshold
    vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);


    tresholdLine = new QLineEdit("-16.5 db");
    tresholdLine->setStyleSheet(lineStyle);
    tresholdLine->setAlignment(Qt::AlignCenter);
    tresholdLine->setMinimumWidth(48*dpiRatio);
    tresholdLine->setMaximumWidth(48*dpiRatio);
    tresholdLine->setFont(m_fonts->boldFont8);
    vlayout_->addWidget(tresholdLine);

    sliderslayout = new QHBoxLayout;
    sliderslayout->setContentsMargins(0, 0, 0, 0);
    sliderslayout->setSpacing(0);
    tresholdSlider = new QSlider;
    tresholdSlider->setOrientation(Qt::Orientation::Vertical);
    tresholdSlider->setMinimumSize(32*dpiRatio, 32*dpiRatio);
    tresholdSlider->setStyleSheet(sliderStyle);
    tresholdSlider->setMaximum(120);
    tresholdSlider->setValue(87);
    tresholdSlider->setPageStep(0);
    sliderslayout->addWidget(tresholdSlider);
    vlayout_->addLayout(sliderslayout);

    label = new QLabel("Treshold");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setAlignment(Qt::AlignCenter);
    vlayout_->addWidget(label);

    hlayout->addLayout(vlayout_);
    //Ratio
    vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);


    ratioLine = new QLineEdit("2.5");
    ratioLine->setStyleSheet(lineStyle);
    ratioLine->setAlignment(Qt::AlignCenter);
    ratioLine->setMinimumWidth(48*dpiRatio);
    ratioLine->setMaximumWidth(48*dpiRatio);
    ratioLine->setFont(m_fonts->boldFont8);
    vlayout_->addWidget(ratioLine);

    sliderslayout = new QHBoxLayout;
    sliderslayout->setContentsMargins(0, 0, 0, 0);
    sliderslayout->setSpacing(0);
    ratioSlider = new QSlider;
    ratioSlider->setOrientation(Qt::Orientation::Vertical);
    ratioSlider->setMinimumSize(32*dpiRatio, 32*dpiRatio);
    ratioSlider->setStyleSheet(sliderStyle);
    ratioSlider->setMaximum(11);
    ratioSlider->setValue(4);
    ratioSlider->setPageStep(0);
    sliderslayout->addWidget(ratioSlider);
    vlayout_->addLayout(sliderslayout);

    label = new QLabel("Ratio");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setAlignment(Qt::AlignCenter);
    vlayout_->addWidget(label);


    hlayout->addLayout(vlayout_);
    //Mix
    vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);


    mixLine = new QLineEdit("100 %");
    mixLine->setStyleSheet(lineStyle);
    mixLine->setAlignment(Qt::AlignCenter);
    mixLine->setMinimumWidth(48*dpiRatio);
    mixLine->setMaximumWidth(48*dpiRatio);
    mixLine->setFont(m_fonts->boldFont8);
    vlayout_->addWidget(mixLine);

    sliderslayout = new QHBoxLayout;
    sliderslayout->setContentsMargins(0, 0, 0, 0);
    sliderslayout->setSpacing(0);
    mixSlider = new QSlider;
    mixSlider->setOrientation(Qt::Orientation::Vertical);
    mixSlider->setMinimumSize(32*dpiRatio, 32*dpiRatio);
    mixSlider->setStyleSheet(sliderStyle);
    mixSlider->setMaximum(20);
    mixSlider->setValue(20);
    mixSlider->setPageStep(0);
    sliderslayout->addWidget(mixSlider);
    vlayout_->addLayout(sliderslayout);

    label = new QLabel("Mix");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setAlignment(Qt::AlignCenter);
    vlayout_->addWidget(label);


    hlayout->addLayout(vlayout_);
    //Gain
    vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);


    gainLine = new QLineEdit("0 db");
    gainLine->setStyleSheet(lineStyle);
    gainLine->setAlignment(Qt::AlignCenter);
    gainLine->setMinimumWidth(48*dpiRatio);
    gainLine->setMaximumWidth(48*dpiRatio);
    gainLine->setFont(m_fonts->boldFont8);
    vlayout_->addWidget(gainLine);

    sliderslayout = new QHBoxLayout;
    sliderslayout->setContentsMargins(0, 0, 0, 0);
    sliderslayout->setSpacing(0);
    gainSlider = new QSlider;
    gainSlider->setOrientation(Qt::Orientation::Vertical);
    gainSlider->setMinimumSize(32*dpiRatio, 32*dpiRatio);
    gainSlider->setStyleSheet(sliderStyle);
    gainSlider->setMaximum(48);
    gainSlider->setValue(0);
    gainSlider->setPageStep(0);
    sliderslayout->addWidget(gainSlider);
    vlayout_->addLayout(sliderslayout);

    label = new QLabel("Gain");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setAlignment(Qt::AlignCenter);
    vlayout_->addWidget(label);

    hlayout->addLayout(vlayout_);
    //end

    vlayout->addLayout(hlayout);

    widget->setLayout(vlayout);

    return widget;
}

QWidget *DynamicsWidget::compEnvWidget(){
    QWidget *widget = new QWidget;
    widget->setSizePolicy(QSizePolicy::Maximum,
                            QSizePolicy::Minimum);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(4, 4, 4, 4);
    vlayout->setSpacing(4);

    QLabel *label = new QLabel("Envelope");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setAlignment(Qt::AlignCenter);
    vlayout->addWidget(label);
    //Auto button
    QHBoxLayout *hlayout_ = new QHBoxLayout;
    hlayout_->setContentsMargins(0, 0, 0, 0);
    hlayout_->setSpacing(0);
    compAutoPushButton = new QPushButton("Auto Time");
    compAutoPushButton->setCheckable(true);
    compAutoPushButton->setMinimumSize(96*dpiRatio, 24*dpiRatio);
    compAutoPushButton->setMaximumSize(96*dpiRatio, 24*dpiRatio);
    compAutoPushButton->setStyleSheet(buttonStyle);
    compAutoPushButton->setFont(m_fonts->boldFont8);
    hlayout_->addWidget(compAutoPushButton);
    vlayout->addLayout(hlayout_);
    //Envelope faders
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setContentsMargins(4, 4, 4, 4);
    hlayout->setSpacing(4);
    //Attack
    QVBoxLayout *vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);


    compAttackLine = new QLineEdit("0");
    compAttackLine->setStyleSheet(lineStyle);
    compAttackLine->setAlignment(Qt::AlignCenter);
    compAttackLine->setMinimumWidth(48*dpiRatio);
    compAttackLine->setMaximumWidth(48*dpiRatio);
    compAttackLine->setFont(m_fonts->boldFont8);
    vlayout_->addWidget(compAttackLine);

    QHBoxLayout *sliderslayout = new QHBoxLayout;
    sliderslayout->setContentsMargins(0, 0, 0, 0);
    sliderslayout->setSpacing(0);
    compAttackSlider = new QSlider;
    compAttackSlider->setOrientation(Qt::Orientation::Vertical);
    compAttackSlider->setMinimumSize(32*dpiRatio, 32*dpiRatio);
    compAttackSlider->setStyleSheet(sliderStyle);
    compAttackSlider->setMaximum(120);
    compAttackSlider->setValue(0);
    compAttackSlider->setPageStep(0);
    sliderslayout->addWidget(compAttackSlider);
    vlayout_->addLayout(sliderslayout);

    label = new QLabel("Attack");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setAlignment(Qt::AlignCenter);
    vlayout_->addWidget(label);


    hlayout->addLayout(vlayout_);
    //Hold
    vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);


    compHoldLine = new QLineEdit("0");
    compHoldLine->setStyleSheet(lineStyle);
    compHoldLine->setAlignment(Qt::AlignCenter);
    compHoldLine->setMinimumWidth(48*dpiRatio);
    compHoldLine->setMaximumWidth(48*dpiRatio);
    compHoldLine->setFont(m_fonts->boldFont8);
    vlayout_->addWidget(compHoldLine);

    sliderslayout = new QHBoxLayout;
    sliderslayout->setContentsMargins(0, 0, 0, 0);
    sliderslayout->setSpacing(0);
    compHoldSlider = new QSlider;
    compHoldSlider->setOrientation(Qt::Orientation::Vertical);
    compHoldSlider->setMinimumSize(32*dpiRatio, 32*dpiRatio);
    compHoldSlider->setStyleSheet(sliderStyle);
    compHoldSlider->setMaximum(100);
    compHoldSlider->setValue(0);
    compHoldSlider->setPageStep(0);
    sliderslayout->addWidget(compHoldSlider);
    vlayout_->addLayout(sliderslayout);

    label = new QLabel("Hold");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setAlignment(Qt::AlignCenter);
    vlayout_->addWidget(label);


    hlayout->addLayout(vlayout_);
    //Release
    vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);


    compReleaseLine = new QLineEdit("0");
    compReleaseLine->setStyleSheet(lineStyle);
    compReleaseLine->setAlignment(Qt::AlignCenter);
    compReleaseLine->setMinimumWidth(48*dpiRatio);
    compReleaseLine->setMaximumWidth(48*dpiRatio);
    compReleaseLine->setFont(m_fonts->boldFont8);
    vlayout_->addWidget(compReleaseLine);

    sliderslayout = new QHBoxLayout;
    sliderslayout->setContentsMargins(0, 0, 0, 0);
    sliderslayout->setSpacing(0);
    compReleaseSlider = new QSlider;
    compReleaseSlider->setOrientation(Qt::Orientation::Vertical);
    compReleaseSlider->setMinimumSize(32*dpiRatio, 32*dpiRatio);
    compReleaseSlider->setStyleSheet(sliderStyle);
    compReleaseSlider->setMaximum(100);
    compReleaseSlider->setValue(0);
    compReleaseSlider->setPageStep(0);
    sliderslayout->addWidget(compReleaseSlider);
    vlayout_->addLayout(sliderslayout);

    label = new QLabel("Release");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setAlignment(Qt::AlignCenter);
    vlayout_->addWidget(label);

    hlayout->addLayout(vlayout_);
    //End
    vlayout->addLayout(hlayout);

    widget->setLayout(vlayout);

    return widget;
}

QWidget *DynamicsWidget::compFltrWidget(){
    QWidget *widget = new QWidget;
    widget->setSizePolicy(QSizePolicy::Maximum,
                            QSizePolicy::Minimum);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(4, 4, 4, 4);
    vlayout->setSpacing(4);

    QLabel *label = new QLabel("Side Chain Filter");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setAlignment(Qt::AlignCenter);
    vlayout->addWidget(label);
    //side chain
    QHBoxLayout *hlayout_ = new QHBoxLayout;
    hlayout_->setContentsMargins(0, 0, 0, 0);
    hlayout_->setSpacing(0);

    label = new QLabel("Key");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setAlignment(Qt::AlignCenter);
    hlayout_->addWidget(label);

    compFilterSourceComboBox = new QComboBox;
    compFilterSourceComboBox->setMinimumSize(96*dpiRatio, 24*dpiRatio);
    compFilterSourceComboBox->setMaximumSize(96*dpiRatio, 24*dpiRatio);
    compFilterSourceComboBox->addItem(QString("Self"), 0);
    for(int i = 1;i < 33;i++)
        compFilterSourceComboBox->addItem(QString("Channel " + QString::number(i)), i);
    for(int i = 1;i < 9;i++)
        compFilterSourceComboBox->addItem(QString("Aux " + QString::number(i)), i + 32);
    compFilterSourceComboBox->addItem(QString("Fx Rtn 1L"), 41);
    compFilterSourceComboBox->addItem(QString("Fx Rtn 1R"), 42);
    compFilterSourceComboBox->addItem(QString("Fx Rtn 2L"), 43);
    compFilterSourceComboBox->addItem(QString("Fx Rtn 2R"), 44);
    compFilterSourceComboBox->addItem(QString("Fx Rtn 3L"), 45);
    compFilterSourceComboBox->addItem(QString("Fx Rtn 3R"), 46);
    compFilterSourceComboBox->addItem(QString("Fx Rtn 4L"), 47);
    compFilterSourceComboBox->addItem(QString("Fx Rtn 4R"), 48);
    for(int i = 1;i < 17;i++)
        compFilterSourceComboBox->addItem(QString("Bus " + QString::number(i)), i + 48);    
    hlayout_->addWidget(compFilterSourceComboBox);

    vlayout->addLayout(hlayout_);

    //Envelope faders
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setContentsMargins(4, 4, 4, 4);
    hlayout->setSpacing(4);

    //Filter,solo buttons
    QVBoxLayout *vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);

    compFilterPushButton = new QPushButton("Filter");
    compFilterPushButton->setCheckable(true);
    compFilterPushButton->setMinimumSize(48*dpiRatio, 24*dpiRatio);
    compFilterPushButton->setMaximumSize(48*dpiRatio, 24*dpiRatio);
    compFilterPushButton->setStyleSheet(buttonStyle);
    compFilterPushButton->setFont(m_fonts->boldFont8);
    vlayout_->addWidget(compFilterPushButton);

    compFilterSoloPushButton = new QPushButton("SOLO");
    compFilterSoloPushButton->setCheckable(true);
    compFilterSoloPushButton->setMinimumSize(48*dpiRatio, 24*dpiRatio);
    compFilterSoloPushButton->setMaximumSize(48*dpiRatio, 24*dpiRatio);
    compFilterSoloPushButton->setStyleSheet("QPushButton {"
                          "color: rgb(0,0,0);"
                          "background-color: rgb(196, 196, 196);"
                          "border: 1px solid rgb(32,32,32);"
                          "border-radius: 0px;}"
                          "QPushButton:pressed {"
                          "background-color: rgb(128, 128, 128);}"
                          "QPushButton:checked {"
                          "background-color: rgb(255, 222, 128);}"
                          "QPushButton:checked:pressed {"
                          "background-color: rgb(196, 163, 95);}");
    compFilterSoloPushButton->setFont(m_fonts->boldFont8);
    vlayout_->addWidget(compFilterSoloPushButton);

    QSpacerItem *verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    vlayout_->addItem(verticalSpacer);

    hlayout->addLayout(vlayout_);
    /*filtertype*/
    vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);


    compFilterTypeLine = new QLineEdit("0");
    compFilterTypeLine->setStyleSheet(lineStyle);
    compFilterTypeLine->setAlignment(Qt::AlignCenter);
    compFilterTypeLine->setMinimumWidth(48*dpiRatio);
    compFilterTypeLine->setMaximumWidth(48*dpiRatio);
    compFilterTypeLine->setFont(m_fonts->boldFont8);
    vlayout_->addWidget(compFilterTypeLine);

    QHBoxLayout *sliderslayout = new QHBoxLayout;
    sliderslayout->setContentsMargins(0, 0, 0, 0);
    sliderslayout->setSpacing(0);
    compFilterTypeSlider = new QSlider;
    compFilterTypeSlider->setOrientation(Qt::Orientation::Vertical);
    compFilterTypeSlider->setMinimumSize(32*dpiRatio, 32*dpiRatio);
    compFilterTypeSlider->setStyleSheet(sliderStyle);
    compFilterTypeSlider->setMaximum(8);
    compFilterTypeSlider->setValue(0);
    compFilterTypeSlider->setPageStep(0);
    sliderslayout->addWidget(compFilterTypeSlider);
    vlayout_->addLayout(sliderslayout);

    label = new QLabel("Type");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setAlignment(Qt::AlignCenter);
    vlayout_->addWidget(label);

    hlayout->addLayout(vlayout_);
    /*filterfreq*/
    vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);

    compFilterFreqLine = new QLineEdit("0");
    compFilterFreqLine->setStyleSheet(lineStyle);
    compFilterFreqLine->setAlignment(Qt::AlignCenter);
    compFilterFreqLine->setMinimumWidth(48*dpiRatio);
    compFilterFreqLine->setMaximumWidth(48*dpiRatio);
    compFilterFreqLine->setFont(m_fonts->boldFont8);
    vlayout_->addWidget(compFilterFreqLine);

    sliderslayout = new QHBoxLayout;
    sliderslayout->setContentsMargins(0, 0, 0, 0);
    sliderslayout->setSpacing(0);
    compFilterFreqSlider = new QSlider;
    compFilterFreqSlider->setOrientation(Qt::Orientation::Vertical);
    compFilterFreqSlider->setMinimumSize(32*dpiRatio, 32*dpiRatio);
    compFilterFreqSlider->setStyleSheet(sliderStyle);
    compFilterFreqSlider->setMaximum(200);
    compFilterFreqSlider->setValue(0);
    compFilterFreqSlider->setPageStep(0);
    sliderslayout->addWidget(compFilterFreqSlider);
    vlayout_->addLayout(sliderslayout);

    label = new QLabel("Freq");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setAlignment(Qt::AlignCenter);
    vlayout_->addWidget(label);

    hlayout->addLayout(vlayout_);
    //End
    vlayout->addLayout(hlayout);

    widget->setLayout(vlayout);

    return widget;
}

QFrame *DynamicsWidget::gateWidget(){
    QFrame *frame = new QFrame;
    frame->setFrameShape(QFrame::Box);
    frame->setFrameShadow(QFrame::Plain);
    frame->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,
                                     QSizePolicy::MinimumExpanding));

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->setSpacing(0);
    //Activate Button
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(8, 16, 8, 16);
    vlayout->setSpacing(4);

    gateActivePushButton = new QPushButton("Active");
    gateActivePushButton->setCheckable(true);
    gateActivePushButton->setMinimumSize(96*dpiRatio, 36*dpiRatio);
    gateActivePushButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    gateActivePushButton->setStyleSheet("QPushButton {"
                          "color: rgb(0,0,0);"
                          "background-color: rgb(196, 196, 196);"
                          "border: 2px solid rgb(32,32,32);"
                          "border-radius: 0px;}"
                          "QPushButton:pressed {"
                          "background-color: rgb(128, 128, 128);}"
                          "QPushButton:checked {"
                          "background-color: rgb(128, 255, 128);}"
                          "QPushButton:checked:pressed {"
                          "background-color: rgb(128, 196, 128);}");
    gateActivePushButton->setFont(m_fonts->boldFont12);
    vlayout->addWidget(gateActivePushButton);

    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    vlayout->addItem(verticalSpacer);

    vlayout->addWidget(gateRadioWidget());

    hlayout->addLayout(vlayout);
    //Comp graphics
    QHBoxLayout *hlayout_ = new QHBoxLayout;
    hlayout_->setContentsMargins(10, 10, 10, 10);
    hlayout_->setSpacing(0);
    gateGain = new gateGainPaint(dpiRatio);
    gateGain->setSizePolicy(QSizePolicy::MinimumExpanding,
                            QSizePolicy::MinimumExpanding);
    hlayout_->addWidget(gateGain);
    hlayout->addLayout(hlayout_);

    hlayout->addWidget(gateGainWidget());
    hlayout->addWidget(gateEnvWidget());
    hlayout->addWidget(gateFltrWidget());

    frame->setLayout(hlayout);
    return frame;
}

QWidget *DynamicsWidget::gateRadioWidget(){
    QWidget *widget = new QWidget;

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(4, 4, 4, 4);
    vlayout->setSpacing(4);

    gateType = new QButtonGroup;

    gateTypeRadioExp2 = new QRadioButton("Exp 2:1");
    gateTypeRadioExp2->setStyleSheet("QRadioButton {color: rgb(255,255,255);}");
    gateTypeRadioExp2->setFont(m_fonts->boldFont8);

    gateType->addButton(gateTypeRadioExp2);
    vlayout->addWidget(gateTypeRadioExp2);

    gateTypeRadioExp3 = new QRadioButton("Exp 3:1");
    gateTypeRadioExp3->setStyleSheet("QRadioButton {color: rgb(255,255,255);}");
    gateTypeRadioExp3->setFont(m_fonts->boldFont8);

    gateType->addButton(gateTypeRadioExp3);
    vlayout->addWidget(gateTypeRadioExp3);

    gateTypeRadioExp4 = new QRadioButton("Exp 4:1");
    gateTypeRadioExp4->setStyleSheet("QRadioButton {color: rgb(255,255,255);}");
    gateTypeRadioExp4->setFont(m_fonts->boldFont8);

    gateType->addButton(gateTypeRadioExp4);
    vlayout->addWidget(gateTypeRadioExp4);

    gateTypeRadioGate = new QRadioButton("Gate");
    gateTypeRadioGate->setStyleSheet("QRadioButton {color: rgb(255,255,255);}");
    gateTypeRadioGate->setFont(m_fonts->boldFont8);
    gateTypeRadioGate->setChecked(true);

    gateType->addButton(gateTypeRadioGate);
    vlayout->addWidget(gateTypeRadioGate);

    gateTypeRadioDucker = new QRadioButton("Ducker");
    gateTypeRadioDucker->setStyleSheet("QRadioButton {color: rgb(255,255,255);}");
    gateTypeRadioDucker->setFont(m_fonts->boldFont8);

    gateType->addButton(gateTypeRadioDucker);
    vlayout->addWidget(gateTypeRadioDucker);

    widget->setLayout(vlayout);
    return widget;
}

QWidget *DynamicsWidget::gateGainWidget(){
    QWidget *widget = new QWidget;
    widget->setSizePolicy(QSizePolicy::MinimumExpanding,
                            QSizePolicy::Minimum);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(4, 4, 4, 4);
    vlayout->setSpacing(4);

    QLabel *label = new QLabel("Gain");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setAlignment(Qt::AlignCenter);
    vlayout->addWidget(label);

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setContentsMargins(4, 4, 4, 4);
    hlayout->setSpacing(4);
    //Treshold
    QVBoxLayout *vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);

    gateTresholdLine = new QLineEdit("0");
    gateTresholdLine->setStyleSheet(lineStyle);
    gateTresholdLine->setAlignment(Qt::AlignCenter);
    gateTresholdLine->setMinimumWidth(48*dpiRatio);
    gateTresholdLine->setMaximumWidth(48*dpiRatio);
    gateTresholdLine->setFont(m_fonts->boldFont8);
    vlayout_->addWidget(gateTresholdLine);

    QHBoxLayout *sliderslayout = new QHBoxLayout;
    sliderslayout->setContentsMargins(0, 0, 0, 0);
    sliderslayout->setSpacing(0);
    gateTresholdSlider = new QSlider;
    gateTresholdSlider->setOrientation(Qt::Orientation::Vertical);
    gateTresholdSlider->setMinimumSize(32*dpiRatio, 32*dpiRatio);
    gateTresholdSlider->setStyleSheet(sliderStyle);
    gateTresholdSlider->setMaximum(160);
    gateTresholdSlider->setValue(0);
    gateTresholdSlider->setPageStep(0);
    sliderslayout->addWidget(gateTresholdSlider);
    vlayout_->addLayout(sliderslayout);

    label = new QLabel("Treshold");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setAlignment(Qt::AlignCenter);
    vlayout_->addWidget(label);

    hlayout->addLayout(vlayout_);
    //Range
    vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);


    gateRangeLine = new QLineEdit("2.5");
    gateRangeLine->setStyleSheet(lineStyle);
    gateRangeLine->setAlignment(Qt::AlignCenter);
    gateRangeLine->setMinimumWidth(48*dpiRatio);
    gateRangeLine->setMaximumWidth(48*dpiRatio);
    gateRangeLine->setFont(m_fonts->boldFont8);
    vlayout_->addWidget(gateRangeLine);

    sliderslayout = new QHBoxLayout;
    sliderslayout->setContentsMargins(0, 0, 0, 0);
    sliderslayout->setSpacing(0);
    gateRangeSlider = new QSlider;
    gateRangeSlider->setOrientation(Qt::Orientation::Vertical);
    gateRangeSlider->setMinimumSize(32*dpiRatio, 32*dpiRatio);
    gateRangeSlider->setStyleSheet(sliderStyle);
    gateRangeSlider->setMaximum(57);
    gateRangeSlider->setValue(0);
    gateRangeSlider->setPageStep(0);
    sliderslayout->addWidget(gateRangeSlider);
    vlayout_->addLayout(sliderslayout);

    label = new QLabel("Range");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setAlignment(Qt::AlignCenter);
    vlayout_->addWidget(label);

    hlayout->addLayout(vlayout_);
    //end

    vlayout->addLayout(hlayout);

    widget->setLayout(vlayout);

    return widget;
}

QWidget *DynamicsWidget::gateEnvWidget(){
    QWidget *widget = new QWidget;
    widget->setSizePolicy(QSizePolicy::Maximum,
                            QSizePolicy::Minimum);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(4, 4, 4, 4);
    vlayout->setSpacing(4);

    QLabel *label = new QLabel("Envelope");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setAlignment(Qt::AlignCenter);
    vlayout->addWidget(label);
    //Envelope faders
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setContentsMargins(4, 4, 4, 4);
    hlayout->setSpacing(4);
    //Attack
    QVBoxLayout *vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);

    gateAttackLine = new QLineEdit("0");
    gateAttackLine->setStyleSheet(lineStyle);
    gateAttackLine->setAlignment(Qt::AlignCenter);
    gateAttackLine->setMinimumWidth(48*dpiRatio);
    gateAttackLine->setMaximumWidth(48*dpiRatio);
    gateAttackLine->setFont(m_fonts->boldFont8);
    vlayout_->addWidget(gateAttackLine);

    QHBoxLayout *sliderslayout = new QHBoxLayout;
    sliderslayout->setContentsMargins(0, 0, 0, 0);
    sliderslayout->setSpacing(0);
    gateAttackSlider = new QSlider;
    gateAttackSlider->setOrientation(Qt::Orientation::Vertical);
    gateAttackSlider->setMinimumSize(32*dpiRatio, 32*dpiRatio);
    gateAttackSlider->setStyleSheet(sliderStyle);
    gateAttackSlider->setMaximum(120);
    gateAttackSlider->setValue(0);
    gateAttackSlider->setPageStep(0);
    sliderslayout->addWidget(gateAttackSlider);
    vlayout_->addLayout(sliderslayout);

    label = new QLabel("Attack");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setAlignment(Qt::AlignCenter);
    vlayout_->addWidget(label);


    hlayout->addLayout(vlayout_);
    //Hold
    vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);


    gateHoldLine = new QLineEdit("0");
    gateHoldLine->setStyleSheet(lineStyle);
    gateHoldLine->setAlignment(Qt::AlignCenter);
    gateHoldLine->setMinimumWidth(48*dpiRatio);
    gateHoldLine->setMaximumWidth(48*dpiRatio);
    gateHoldLine->setFont(m_fonts->boldFont8);
    vlayout_->addWidget(gateHoldLine);

    sliderslayout = new QHBoxLayout;
    sliderslayout->setContentsMargins(0, 0, 0, 0);
    sliderslayout->setSpacing(0);
    gateHoldSlider = new QSlider;
    gateHoldSlider->setOrientation(Qt::Orientation::Vertical);
    gateHoldSlider->setMinimumSize(32*dpiRatio, 32*dpiRatio);
    gateHoldSlider->setStyleSheet(sliderStyle);
    gateHoldSlider->setMaximum(100);
    gateHoldSlider->setValue(0);
    gateHoldSlider->setPageStep(0);
    sliderslayout->addWidget(gateHoldSlider);
    vlayout_->addLayout(sliderslayout);

    label = new QLabel("Hold");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setAlignment(Qt::AlignCenter);
    vlayout_->addWidget(label);


    hlayout->addLayout(vlayout_);
    //Release
    vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);


    gateReleaseLine = new QLineEdit("0");
    gateReleaseLine->setStyleSheet(lineStyle);
    gateReleaseLine->setAlignment(Qt::AlignCenter);
    gateReleaseLine->setMinimumWidth(48*dpiRatio);
    gateReleaseLine->setMaximumWidth(48*dpiRatio);
    gateReleaseLine->setFont(m_fonts->boldFont8);
    vlayout_->addWidget(gateReleaseLine);

    sliderslayout = new QHBoxLayout;
    sliderslayout->setContentsMargins(0, 0, 0, 0);
    sliderslayout->setSpacing(0);
    gateReleaseSlider = new QSlider;
    gateReleaseSlider->setOrientation(Qt::Orientation::Vertical);
    gateReleaseSlider->setMinimumSize(32*dpiRatio, 32*dpiRatio);
    gateReleaseSlider->setStyleSheet(sliderStyle);
    gateReleaseSlider->setMaximum(100);
    gateReleaseSlider->setValue(0);
    gateReleaseSlider->setPageStep(0);
    sliderslayout->addWidget(gateReleaseSlider);
    vlayout_->addLayout(sliderslayout);

    label = new QLabel("Release");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setAlignment(Qt::AlignCenter);
    vlayout_->addWidget(label);

    hlayout->addLayout(vlayout_);
    //End
    vlayout->addLayout(hlayout);

    widget->setLayout(vlayout);

    return widget;
}

QWidget *DynamicsWidget::gateFltrWidget(){
    QWidget *widget = new QWidget;
    widget->setSizePolicy(QSizePolicy::Maximum,
                            QSizePolicy::Minimum);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(4, 4, 4, 4);
    vlayout->setSpacing(4);

    QLabel *label = new QLabel("Side Chain Filter");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setAlignment(Qt::AlignCenter);
    vlayout->addWidget(label);
    //side chain
    QHBoxLayout *hlayout_ = new QHBoxLayout;
    hlayout_->setContentsMargins(0, 0, 0, 0);
    hlayout_->setSpacing(0);

    label = new QLabel("Key");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setAlignment(Qt::AlignCenter);
    hlayout_->addWidget(label);

    gateFilterSourceComboBox = new QComboBox;
    gateFilterSourceComboBox->setMinimumSize(96*dpiRatio, 24*dpiRatio);
    gateFilterSourceComboBox->setMaximumSize(96*dpiRatio, 24*dpiRatio);
    gateFilterSourceComboBox->addItem(QString("Self"), 0);
    for(int i = 1;i < 33;i++)
        gateFilterSourceComboBox->addItem(QString("Channel " + QString::number(i)), i);
    for(int i = 1;i < 9;i++)
        gateFilterSourceComboBox->addItem(QString("Aux " + QString::number(i)), i + 32);
    gateFilterSourceComboBox->addItem(QString("Fx Rtn 1L"), 41);
    gateFilterSourceComboBox->addItem(QString("Fx Rtn 1R"), 42);
    gateFilterSourceComboBox->addItem(QString("Fx Rtn 2L"), 43);
    gateFilterSourceComboBox->addItem(QString("Fx Rtn 2R"), 44);
    gateFilterSourceComboBox->addItem(QString("Fx Rtn 3L"), 45);
    gateFilterSourceComboBox->addItem(QString("Fx Rtn 3R"), 46);
    gateFilterSourceComboBox->addItem(QString("Fx Rtn 4L"), 47);
    gateFilterSourceComboBox->addItem(QString("Fx Rtn 4R"), 48);
    for(int i = 1;i < 17;i++)
        gateFilterSourceComboBox->addItem(QString("Bus " + QString::number(i)), i + 48);
    hlayout_->addWidget(gateFilterSourceComboBox);

    vlayout->addLayout(hlayout_);

    //Envelope faders
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setContentsMargins(4, 4, 4, 4);
    hlayout->setSpacing(4);

    //Filter,solo buttons
    QVBoxLayout *vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);

    gateFilterPushButton = new QPushButton("Filter");
    gateFilterPushButton->setCheckable(true);
    gateFilterPushButton->setMinimumSize(48*dpiRatio, 24*dpiRatio);
    gateFilterPushButton->setMaximumSize(48*dpiRatio, 24*dpiRatio);
    gateFilterPushButton->setStyleSheet(buttonStyle);
    gateFilterPushButton->setFont(m_fonts->boldFont8);
    vlayout_->addWidget(gateFilterPushButton);

    gateFilterSoloPushButton = new QPushButton("SOLO");
    gateFilterSoloPushButton->setCheckable(true);
    gateFilterSoloPushButton->setMinimumSize(48*dpiRatio, 24*dpiRatio);
    gateFilterSoloPushButton->setMaximumSize(48*dpiRatio, 24*dpiRatio);
    gateFilterSoloPushButton->setStyleSheet("QPushButton {"
                          "color: rgb(0,0,0);"
                          "background-color: rgb(196, 196, 196);"
                          "border: 1px solid rgb(32,32,32);"
                          "border-radius: 0px;}"
                          "QPushButton:pressed {"
                          "background-color: rgb(128, 128, 128);}"
                          "QPushButton:checked {"
                          "background-color: rgb(255, 222, 128);}"
                          "QPushButton:checked:pressed {"
                          "background-color: rgb(196, 163, 95);}");
    gateFilterSoloPushButton->setFont(m_fonts->boldFont8);
    vlayout_->addWidget(gateFilterSoloPushButton);

    QSpacerItem *verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    vlayout_->addItem(verticalSpacer);

    hlayout->addLayout(vlayout_);
    /*filtertype*/
    vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);


    gateFilterTypeLine = new QLineEdit("0");
    gateFilterTypeLine->setStyleSheet(lineStyle);
    gateFilterTypeLine->setAlignment(Qt::AlignCenter);
    gateFilterTypeLine->setMinimumWidth(48*dpiRatio);
    gateFilterTypeLine->setMaximumWidth(48*dpiRatio);
    gateFilterTypeLine->setFont(m_fonts->boldFont8);
    vlayout_->addWidget(gateFilterTypeLine);

    QHBoxLayout *sliderslayout = new QHBoxLayout;
    sliderslayout->setContentsMargins(0, 0, 0, 0);
    sliderslayout->setSpacing(0);
    gateFilterTypeSlider = new QSlider;
    gateFilterTypeSlider->setOrientation(Qt::Orientation::Vertical);
    gateFilterTypeSlider->setMinimumSize(32*dpiRatio, 32*dpiRatio);
    gateFilterTypeSlider->setStyleSheet(sliderStyle);
    gateFilterTypeSlider->setMaximum(8);
    gateFilterTypeSlider->setValue(0);
    gateFilterTypeSlider->setPageStep(0);
    sliderslayout->addWidget(gateFilterTypeSlider);
    vlayout_->addLayout(sliderslayout);

    label = new QLabel("Type");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setAlignment(Qt::AlignCenter);
    vlayout_->addWidget(label);

    hlayout->addLayout(vlayout_);
    /*filterfreq*/
    vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);

    gateFilterFreqLine = new QLineEdit("0");
    gateFilterFreqLine->setStyleSheet(lineStyle);
    gateFilterFreqLine->setAlignment(Qt::AlignCenter);
    gateFilterFreqLine->setMinimumWidth(48*dpiRatio);
    gateFilterFreqLine->setMaximumWidth(48*dpiRatio);
    gateFilterFreqLine->setFont(m_fonts->boldFont8);
    vlayout_->addWidget(gateFilterFreqLine);

    sliderslayout = new QHBoxLayout;
    sliderslayout->setContentsMargins(0, 0, 0, 0);
    sliderslayout->setSpacing(0);
    gateFilterFreqSlider = new QSlider;
    gateFilterFreqSlider->setOrientation(Qt::Orientation::Vertical);
    gateFilterFreqSlider->setMinimumSize(32*dpiRatio, 32*dpiRatio);
    gateFilterFreqSlider->setStyleSheet(sliderStyle);
    gateFilterFreqSlider->setMaximum(200);
    gateFilterFreqSlider->setValue(0);
    gateFilterFreqSlider->setPageStep(0);
    sliderslayout->addWidget(gateFilterFreqSlider);
    vlayout_->addLayout(sliderslayout);

    label = new QLabel("Freq");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setAlignment(Qt::AlignCenter);
    vlayout_->addWidget(label);

    hlayout->addLayout(vlayout_);
    //End
    vlayout->addLayout(hlayout);

    widget->setLayout(vlayout);

    return widget;
}

void DynamicsWidget::connectSignals(){
    /*COMP*/
    QPushButton::connect(compActivePushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitCompOnOffChanged);
    QPushButton::connect(compAutoPushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitCompAutoTimeChanged);
    QPushButton::connect(compFilterPushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitCompFilterChanged);
    QPushButton::connect(compFilterSoloPushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitCompFilterSoloChanged);
    QComboBox::connect(compFilterSourceComboBox, &QComboBox::currentIndexChanged,
                         this, &DynamicsWidget::emitCompKeySourceChanged);
    QSlider::connect(tresholdSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitCompTresholdChanged);
    QSlider::connect(ratioSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitCompRatioChanged);
    QSlider::connect(mixSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitCompMixChanged);
    QSlider::connect(gainSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitCompGainChanged);
    QSlider::connect(kneeSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitCompKneeChanged);
    QRadioButton::connect(compTypeRadio, &QRadioButton::clicked,
                          this, &DynamicsWidget::emitCompModeCompExpChanged);
    QRadioButton::connect(expTypeRadio, &QRadioButton::clicked,
                          this, &DynamicsWidget::emitCompModeCompExpChanged);
    QRadioButton::connect(compLinRadio, &QRadioButton::clicked,
                          this, &DynamicsWidget::emitCompEnvLinLogChanged);
    QRadioButton::connect(compLogRadio, &QRadioButton::clicked,
                          this, &DynamicsWidget::emitCompEnvLinLogChanged);
    QRadioButton::connect(compPeakRadio, &QRadioButton::clicked,
                          this, &DynamicsWidget::emitCompDetPeakRmsChanged);
    QRadioButton::connect(compRmsRadio, &QRadioButton::clicked,
                          this, &DynamicsWidget::emitCompDetPeakRmsChanged);
    QSlider::connect(compAttackSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitCompAttackChanged);
    QSlider::connect(compHoldSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitCompHoldChanged);
    QSlider::connect(compReleaseSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitCompReleaseChanged);
    QSlider::connect(compFilterTypeSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitCompFilterTypeChanged);
    QSlider::connect(compFilterFreqSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitCompFilterFrequencyChanged);
    /*GATE*/
    QPushButton::connect(gateActivePushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitGateOnOffChanged);
    QPushButton::connect(gateFilterPushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitGateFilterChanged);
    QPushButton::connect(gateFilterSoloPushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitGateFilterSoloChanged);
    QComboBox::connect(gateFilterSourceComboBox, &QComboBox::currentIndexChanged,
                         this, &DynamicsWidget::emitGateKeySourceChanged);
    QSlider::connect(gateTresholdSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitGateTresholdChanged);
    QSlider::connect(gateRangeSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitGateRangeChanged);
    QRadioButton::connect(gateTypeRadioExp2, &QRadioButton::clicked,
                          this, &DynamicsWidget::emitGateModeChanged);
    QRadioButton::connect(gateTypeRadioExp3, &QRadioButton::clicked,
                          this, &DynamicsWidget::emitGateModeChanged);
    QRadioButton::connect(gateTypeRadioExp4, &QRadioButton::clicked,
                          this, &DynamicsWidget::emitGateModeChanged);
    QRadioButton::connect(gateTypeRadioGate, &QRadioButton::clicked,
                          this, &DynamicsWidget::emitGateModeChanged);
    QRadioButton::connect(gateTypeRadioDucker, &QRadioButton::clicked,
                          this, &DynamicsWidget::emitGateModeChanged);
    QSlider::connect(gateAttackSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitGateAttackChanged);
    QSlider::connect(gateHoldSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitGateHoldChanged);
    QSlider::connect(gateReleaseSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitGateReleaseChanged);
    QSlider::connect(gateFilterTypeSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitGateFilterTypeChanged);
    QSlider::connect(gateFilterFreqSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitGateFilterFrequencyChanged);
}

void DynamicsWidget::disconnectSignals(){
    /*COMP*/
    QPushButton::disconnect(compActivePushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitCompOnOffChanged);
    QPushButton::disconnect(compAutoPushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitCompAutoTimeChanged);
    QPushButton::disconnect(compFilterPushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitCompFilterChanged);
    QPushButton::disconnect(compFilterSoloPushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitCompFilterSoloChanged);
    QComboBox::disconnect(compFilterSourceComboBox, &QComboBox::currentIndexChanged,
                         this, &DynamicsWidget::emitCompKeySourceChanged);
    QSlider::disconnect(tresholdSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitCompTresholdChanged);
    QSlider::disconnect(ratioSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitCompRatioChanged);
    QSlider::disconnect(mixSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitCompMixChanged);
    QSlider::disconnect(gainSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitCompGainChanged);
    QSlider::disconnect(kneeSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitCompKneeChanged);
    QRadioButton::disconnect(compTypeRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitCompModeCompExpChanged);
    QRadioButton::disconnect(expTypeRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitCompModeCompExpChanged);
    QRadioButton::disconnect(compLinRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitCompEnvLinLogChanged);
    QRadioButton::disconnect(compLogRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitCompEnvLinLogChanged);
    QRadioButton::disconnect(compPeakRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitCompDetPeakRmsChanged);
    QRadioButton::disconnect(compRmsRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitCompDetPeakRmsChanged);
    QSlider::disconnect(compAttackSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitCompAttackChanged);
    QSlider::disconnect(compHoldSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitCompHoldChanged);
    QSlider::disconnect(compReleaseSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitCompReleaseChanged);
    QSlider::disconnect(compFilterTypeSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitCompFilterTypeChanged);
    QSlider::disconnect(compFilterFreqSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitCompFilterFrequencyChanged);
    /*GATE*/
    QPushButton::disconnect(gateActivePushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitGateOnOffChanged);
    QPushButton::disconnect(gateFilterPushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitGateFilterChanged);
    QPushButton::disconnect(gateFilterSoloPushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitGateFilterSoloChanged);
    QComboBox::disconnect(gateFilterSourceComboBox, &QComboBox::currentIndexChanged,
                         this, &DynamicsWidget::emitGateKeySourceChanged);
    QSlider::disconnect(gateTresholdSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitGateTresholdChanged);
    QSlider::disconnect(gateRangeSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitGateRangeChanged);
    QRadioButton::disconnect(gateTypeRadioExp2, &QRadioButton::clicked,
                          this, &DynamicsWidget::emitGateModeChanged);
    QRadioButton::disconnect(gateTypeRadioExp3, &QRadioButton::clicked,
                          this, &DynamicsWidget::emitGateModeChanged);
    QRadioButton::disconnect(gateTypeRadioExp4, &QRadioButton::clicked,
                          this, &DynamicsWidget::emitGateModeChanged);
    QRadioButton::disconnect(gateTypeRadioGate, &QRadioButton::clicked,
                          this, &DynamicsWidget::emitGateModeChanged);
    QRadioButton::disconnect(gateTypeRadioDucker, &QRadioButton::clicked,
                          this, &DynamicsWidget::emitGateModeChanged);
    QSlider::disconnect(gateAttackSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitGateAttackChanged);
    QSlider::disconnect(gateHoldSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitGateHoldChanged);
    QSlider::disconnect(gateReleaseSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitGateReleaseChanged);
    QSlider::disconnect(gateFilterTypeSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitGateFilterTypeChanged);
    QSlider::disconnect(gateFilterFreqSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitGateFilterFrequencyChanged);
}

void DynamicsWidget::compOnOffRecieved(int value){
    QPushButton::disconnect(compActivePushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitCompOnOffChanged);
    compActivePushButton->setChecked(value);
    QPushButton::connect(compActivePushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitCompOnOffChanged);
}

void DynamicsWidget::compTresholdRecieved(float value){
    QSlider::disconnect(tresholdSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitCompTresholdChanged);
    tresholdSlider->setValue(value * 120);
    float db = float(tresholdSlider->value()) / 2 - 60.;
    compGain->setTreshold(db);
    tresholdLine->setText(QString::number(db) + " dB");
    QSlider::connect(tresholdSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitCompTresholdChanged);
}

void DynamicsWidget::compRatioRecieved(int value){
    QSlider::disconnect(ratioSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitCompRatioChanged);
    ratioSlider->setValue(value);
    compGain->setRatio(int2ratio(value));
    ratioLine->setText(QString::number(int2ratio(value)));
    QSlider::connect(ratioSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitCompRatioChanged);
}

void DynamicsWidget::compMixRecieved(float value){
    QSlider::disconnect(mixSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitCompMixChanged);
    mixSlider->setValue(value * 20);
    mixLine->setText(QString::number(value*100) + " %");
    QSlider::connect(mixSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitCompMixChanged);
}

void DynamicsWidget::compGainRecieved(float value){
    QSlider::disconnect(gainSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitCompGainChanged);
    gainSlider->setValue(value * 48);
    gainLine->setText(QString::number(value * 24) + " dB");
    QSlider::connect(gainSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitCompGainChanged);
}

void DynamicsWidget::compKneeRecieved(float value){
    QSlider::disconnect(kneeSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitCompKneeChanged);
    kneeSlider->setValue(value * 5);
    kneeLine->setText(QString::number(kneeSlider->value()));
    compGain->setKnee(kneeSlider->value());
    QSlider::connect(kneeSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitCompKneeChanged);
}

void DynamicsWidget::compModeCompExpRecieved(int value){
    QRadioButton::disconnect(compTypeRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitCompModeCompExpChanged);
    QRadioButton::disconnect(expTypeRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitCompModeCompExpChanged);
    if(value == 1)
        expTypeRadio->setChecked(true);
    else
        compTypeRadio->setChecked(true);
    compGain->setType(value);
    QRadioButton::connect(compTypeRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitCompModeCompExpChanged);
    QRadioButton::connect(expTypeRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitCompModeCompExpChanged);
}

void DynamicsWidget::compEnvLinLogRecieved(int value){
    QRadioButton::disconnect(compLinRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitCompEnvLinLogChanged);
    QRadioButton::disconnect(compLogRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitCompEnvLinLogChanged);
    if(value == 1)
        compLogRadio->setChecked(true);
    else
        compLinRadio->setChecked(true);
    QRadioButton::connect(compLinRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitCompEnvLinLogChanged);
    QRadioButton::connect(compLogRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitCompEnvLinLogChanged);
}

void DynamicsWidget::compDetPeakRmsRecieved(int value){
    QRadioButton::disconnect(compPeakRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitCompDetPeakRmsChanged);
    QRadioButton::disconnect(compRmsRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitCompDetPeakRmsChanged);
    if(value == 1)
        compRmsRadio->setChecked(true);
    else
        compPeakRadio->setChecked(true);
    QRadioButton::connect(compPeakRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitCompDetPeakRmsChanged);
    QRadioButton::connect(compRmsRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitCompDetPeakRmsChanged);
}

void DynamicsWidget::compAttackRecieved(float value){
    QSlider::disconnect(compAttackSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitCompAttackChanged);
    compAttackSlider->setValue(value * 120);
    compAttackLine->setText(QString::number(compAttackSlider->value()) + " ms");
    QSlider::connect(compAttackSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitCompAttackChanged);
}

void DynamicsWidget::compHoldRecieved(float value){
    QSlider::disconnect(compHoldSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitCompHoldChanged);
    compHoldSlider->setValue(value * 100);
    compHoldLine->setText(QString::number(hold2ms(compHoldSlider->value())) + " ms");
    QSlider::connect(compHoldSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitCompHoldChanged);
}

void DynamicsWidget::compReleaseRecieved(float value){
    QSlider::disconnect(compReleaseSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitCompReleaseChanged);
    compReleaseSlider->setValue(value * 100);
    compReleaseLine->setText(QString::number(release2ms(compReleaseSlider->value())) + " ms");
    QSlider::connect(compReleaseSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitCompReleaseChanged);
}

void DynamicsWidget::compAutoTimeRecieved(int value){
    QPushButton::disconnect(compAutoPushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitCompAutoTimeChanged);
    compAutoPushButton->setChecked(value);

    compAttackLine->setDisabled(value);
    compAttackSlider->setDisabled(value);
    compHoldLine->setDisabled(value);
    compHoldSlider->setDisabled(value);
    compReleaseLine->setDisabled(value);
    compReleaseSlider->setDisabled(value);

    QPushButton::connect(compAutoPushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitCompAutoTimeChanged);
}

void DynamicsWidget::compKeySourceRecieved(int value){
    QComboBox::disconnect(compFilterSourceComboBox, &QComboBox::currentIndexChanged,
                         this, &DynamicsWidget::emitCompKeySourceChanged);
    for(int i=0;i < 65;i++)
        if(compFilterSourceComboBox->itemData(i).toInt() == value)
            compFilterSourceComboBox->setCurrentIndex(i);
    QComboBox::connect(compFilterSourceComboBox, &QComboBox::currentIndexChanged,
                         this, &DynamicsWidget::emitCompKeySourceChanged);
}

void DynamicsWidget::compFilterRecieved(int value){
    QPushButton::disconnect(compFilterPushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitCompFilterChanged);
    compFilterPushButton->setChecked(value);
    QPushButton::connect(compFilterPushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitCompFilterChanged);
}

void DynamicsWidget::compFilterSoloRecieved(int value){
    QPushButton::disconnect(compFilterSoloPushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitCompFilterSoloChanged);
    compFilterSoloPushButton->setChecked(value);
    QPushButton::connect(compFilterSoloPushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitCompFilterSoloChanged);
}

void DynamicsWidget::compFilterTypeRecieved(int value){
    QSlider::disconnect(compFilterTypeSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitCompFilterTypeChanged);
    compFilterTypeSlider->setValue(value);
    compFilterTypeLine->setText(
                QString::fromStdString(
                    index2filtertype(value)
                    )
                );
    QSlider::connect(compFilterTypeSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitCompFilterTypeChanged);
}

void DynamicsWidget::compFilterFrequencyRecieved(float value){
    QSlider::disconnect(compFilterFreqSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitCompFilterFrequencyChanged);
    compFilterFreqSlider->setValue(value * 200);
    compFilterFreqLine->setText(QString::fromStdString(c201logScaleToFrequencyString(value)));
    QSlider::connect(compFilterFreqSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitCompFilterFrequencyChanged);
}

void DynamicsWidget::gateOnOffRecieved(int value){
    QPushButton::disconnect(gateActivePushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitGateOnOffChanged);
    gateActivePushButton->setChecked(value);
    QPushButton::connect(gateActivePushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitGateOnOffChanged);
}

void DynamicsWidget::gateTresholdRecieved(float value){
    QSlider::disconnect(gateTresholdSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitGateTresholdChanged);
    gateTresholdSlider->setValue(value * 160);
    float db = float(gateTresholdSlider->value()) / 2 - 80.;
    gateGain->setTreshold(db);
    gateTresholdLine->setText(QString::number(db) + " dB");
    QSlider::connect(gateTresholdSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitGateTresholdChanged);
}

void DynamicsWidget::gateRangeRecieved(float value){
    QSlider::disconnect(gateRangeSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitGateRangeChanged);
    gateRangeSlider->setValue(int(value * 57));
    gateRangeLine->setText(QString::number(int(value * 57) + 3) + " dB");
    QSlider::connect(gateRangeSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitGateRangeChanged);
}

void DynamicsWidget::gateModeRecieved(int value){
    QRadioButton::disconnect(gateTypeRadioExp2, &QRadioButton::clicked,
                          this, &DynamicsWidget::emitGateModeChanged);
    QRadioButton::disconnect(gateTypeRadioExp3, &QRadioButton::clicked,
                          this, &DynamicsWidget::emitGateModeChanged);
    QRadioButton::disconnect(gateTypeRadioExp4, &QRadioButton::clicked,
                          this, &DynamicsWidget::emitGateModeChanged);
    QRadioButton::disconnect(gateTypeRadioGate, &QRadioButton::clicked,
                          this, &DynamicsWidget::emitGateModeChanged);
    QRadioButton::disconnect(gateTypeRadioDucker, &QRadioButton::clicked,
                          this, &DynamicsWidget::emitGateModeChanged);
    switch (value) {
    case 0: gateTypeRadioExp2->setChecked(true); break;
    case 1: gateTypeRadioExp3->setChecked(true); break;
    case 2: gateTypeRadioExp4->setChecked(true); break;
    case 3: gateTypeRadioGate->setChecked(true); break;
    case 4: gateTypeRadioDucker->setChecked(true); break;
    default: break;
    }
    QRadioButton::connect(gateTypeRadioExp2, &QRadioButton::clicked,
                          this, &DynamicsWidget::emitGateModeChanged);
    QRadioButton::connect(gateTypeRadioExp3, &QRadioButton::clicked,
                          this, &DynamicsWidget::emitGateModeChanged);
    QRadioButton::connect(gateTypeRadioExp4, &QRadioButton::clicked,
                          this, &DynamicsWidget::emitGateModeChanged);
    QRadioButton::connect(gateTypeRadioGate, &QRadioButton::clicked,
                          this, &DynamicsWidget::emitGateModeChanged);
    QRadioButton::connect(gateTypeRadioDucker, &QRadioButton::clicked,
                          this, &DynamicsWidget::emitGateModeChanged);
}

void DynamicsWidget::gateAttackRecieved(float value){
    QSlider::disconnect(gateAttackSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitGateAttackChanged);
    gateAttackSlider->setValue(value * 120);
    gateAttackLine->setText(QString::number(gateAttackSlider->value()) + " ms");
    QSlider::connect(gateAttackSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitGateAttackChanged);
}

void DynamicsWidget::gateHoldRecieved(float value){
    QSlider::disconnect(gateHoldSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitGateHoldChanged);
    gateHoldSlider->setValue(value * 100);
    gateHoldLine->setText(QString::number(hold2ms(gateHoldSlider->value())) + " ms");
    QSlider::connect(gateHoldSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitGateHoldChanged);
}

void DynamicsWidget::gateReleaseRecieved(float value){
    QSlider::disconnect(gateReleaseSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitGateReleaseChanged);
    gateReleaseSlider->setValue(value * 100);
    gateReleaseLine->setText(QString::number(release2ms(gateReleaseSlider->value())) + " ms");
    QSlider::connect(gateReleaseSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitGateReleaseChanged);
}

void DynamicsWidget::gateKeySourceRecieved(int value){
    QComboBox::disconnect(gateFilterSourceComboBox, &QComboBox::currentIndexChanged,
                         this, &DynamicsWidget::emitGateKeySourceChanged);
    for(int i=0;i < 65;i++)
        if(gateFilterSourceComboBox->itemData(i).toInt() == value)
            gateFilterSourceComboBox->setCurrentIndex(i);
    QComboBox::connect(gateFilterSourceComboBox, &QComboBox::currentIndexChanged,
                         this, &DynamicsWidget::emitGateKeySourceChanged);
}

void DynamicsWidget::gateFilterRecieved(int value){
    QPushButton::disconnect(gateFilterPushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitGateFilterChanged);
    gateFilterPushButton->setChecked(value);
    QPushButton::connect(gateFilterPushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitGateFilterChanged);
}

void DynamicsWidget::gateFilterSoloRecieved(int value){
    QPushButton::disconnect(gateFilterSoloPushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitGateFilterSoloChanged);
    gateFilterSoloPushButton->setChecked(value);
    QPushButton::connect(gateFilterSoloPushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitGateFilterSoloChanged);
}

void DynamicsWidget::gateFilterTypeRecieved(int value){
    QSlider::disconnect(gateFilterTypeSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitGateFilterTypeChanged);
    gateFilterTypeSlider->setValue(value);
    gateFilterTypeLine->setText(
                QString::fromStdString(
                    index2filtertype(value)
                    )
                );
    QSlider::connect(gateFilterTypeSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitGateFilterTypeChanged);
}

void DynamicsWidget::gateFilterFrequencyRecieved(float value){
    QSlider::disconnect(gateFilterFreqSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitGateFilterFrequencyChanged);
    gateFilterFreqSlider->setValue(value * 200);
    gateFilterFreqLine->setText(QString::fromStdString(c201logScaleToFrequencyString(value)));
    QSlider::connect(gateFilterFreqSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitGateFilterFrequencyChanged);
}

void DynamicsWidget::emitCompOnOffChanged(){
    emit compOnOffChanged(compActivePushButton->isChecked());
}

void DynamicsWidget::emitCompTresholdChanged(){
    float value = float(tresholdSlider->value()) / 120;
    float db = float(tresholdSlider->value()) / 2 - 60.;
    compGain->setTreshold(db);
    tresholdLine->setText(QString::number(db) + " dB");
    emit compTresholdChanged(value);
}

void DynamicsWidget::emitCompRatioChanged(){
    float ratio = int2ratio(ratioSlider->value());
    compGain->setRatio(ratio);
    ratioLine->setText(QString::number(ratio));
    emit compRatioChanged(ratioSlider->value());
}

void DynamicsWidget::emitCompMixChanged(){
    float value = float(mixSlider->value()) / 20;
    mixLine->setText(QString::number(value*100) + " %");
    emit compMixChanged(value);
}

void DynamicsWidget::emitCompGainChanged(){
    float value = float(gainSlider->value()) / 48;
    gainLine->setText(QString::number(value*24) + " dB");
    emit compGainChanged(value);
}

void DynamicsWidget::emitCompKneeChanged(){
    float value = float(kneeSlider->value()) / 5;
    kneeLine->setText(QString::number(kneeSlider->value()));
    compGain->setKnee(kneeSlider->value());
    emit compKneeChanged(value);
}

void DynamicsWidget::emitCompModeCompExpChanged(){
    int value = 0;
    if (expTypeRadio->isChecked()) value = 1;
    compGain->setType(value);
    emit compModeCompExpChanged(value);
}

void DynamicsWidget::emitCompEnvLinLogChanged(){
    int value = 0;
    if (compLogRadio->isChecked()) value = 1;
    emit compEnvLinLogChanged(value);
}

void DynamicsWidget::emitCompDetPeakRmsChanged(){
    int value = 0;
    if (compRmsRadio->isChecked()) value = 1;
    emit compDetPeakRmsChanged(value);
}

void DynamicsWidget::emitCompAttackChanged(){
    float value = float(compAttackSlider->value()) / 120;
    emit compAttackChanged(value);
    compAttackLine->setText(QString::number(compAttackSlider->value()) + " ms");
}

void DynamicsWidget::emitCompHoldChanged(){
    float value = float(compHoldSlider->value()) / 100;
    emit compHoldChanged(value);
    compHoldLine->setText(QString::number(hold2ms(compHoldSlider->value())) + " ms");
}

void DynamicsWidget::emitCompReleaseChanged(){
    float value = float(compReleaseSlider->value()) / 100;
    emit compReleaseChanged(value);
    compReleaseLine->setText(QString::number(release2ms(compReleaseSlider->value())) + " ms");
}

void DynamicsWidget::emitCompAutoTimeChanged(){
    bool value = compAutoPushButton->isChecked();
    compAttackLine->setDisabled(value);
    compAttackSlider->setDisabled(value);
    compHoldLine->setDisabled(value);
    compHoldSlider->setDisabled(value);
    compReleaseLine->setDisabled(value);
    compReleaseSlider->setDisabled(value);
    emit compAutoTimeChanged(int(value));
}

void DynamicsWidget::emitCompKeySourceChanged(){
    emit compKeySourceChanged(compFilterSourceComboBox->currentData().toInt());
}

void DynamicsWidget::emitCompFilterChanged(){
    emit compFilterChanged(compFilterPushButton->isChecked());
}

void DynamicsWidget::emitCompFilterSoloChanged(){
    emit compFilterSoloChanged(compFilterSoloPushButton->isChecked());
}

void DynamicsWidget::emitCompFilterTypeChanged(){
    int value = compFilterTypeSlider->value();
    compFilterTypeLine->setText(
                QString::fromStdString(
                    index2filtertype(value)
                    )
                );
    emit compFilterTypeChanged(value);
}

void DynamicsWidget::emitCompFilterFrequencyChanged(){
    float value = float(compFilterFreqSlider->value()) / 200.f;
    compFilterFreqLine->setText(QString::fromStdString(
                                    c201logScaleToFrequencyString(value)
                                    ));
    emit compFilterFrequencyChanged(value);
}

void DynamicsWidget::emitGateOnOffChanged(){
    emit gateOnOffChanged(gateActivePushButton->isChecked());
}

void DynamicsWidget::emitGateTresholdChanged(){
    float value = float(gateTresholdSlider->value()) / 160;
    float db = float(gateTresholdSlider->value()) / 2 - 80.;
    gateGain->setTreshold(db);
    gateTresholdLine->setText(QString::number(db) + " dB");
    emit gateTresholdChanged(value);
}

void DynamicsWidget::emitGateRangeChanged(){
    float value = float(gateRangeSlider->value()) / 57;
    int db = gateRangeSlider->value() + 3;
    gateGain->setRange(db);
    gateRangeLine->setText(QString::number(db) + " dB");
    emit gateRangeChanged(value);
}

void DynamicsWidget::emitGateModeChanged(){
    int value = -1;
    if(gateTypeRadioExp2->isChecked()) value = 0;
    else if(gateTypeRadioExp3->isChecked()) value = 1;
    else if(gateTypeRadioExp4->isChecked()) value = 2;
    else if(gateTypeRadioGate->isChecked()) value = 3;
    else if(gateTypeRadioDucker->isChecked()) value = 4;
    emit gateModeChanged(value);
}

void DynamicsWidget::emitGateAttackChanged(){
    float value = float(gateAttackSlider->value()) / 120;
    emit gateAttackChanged(value);
    gateAttackLine->setText(QString::number(gateAttackSlider->value()) + " ms");
}

void DynamicsWidget::emitGateHoldChanged(){
    float value = float(gateHoldSlider->value()) / 100;
    emit gateHoldChanged(value);
    gateHoldLine->setText(QString::number(hold2ms(gateHoldSlider->value())) + " ms");
}

void DynamicsWidget::emitGateReleaseChanged(){
    float value = float(gateReleaseSlider->value()) / 100;
    emit gateReleaseChanged(value);
    gateReleaseLine->setText(QString::number(release2ms(gateReleaseSlider->value())) + " ms");
}

void DynamicsWidget::emitGateKeySourceChanged(){
    emit gateKeySourceChanged(gateFilterSourceComboBox->currentData().toInt());
}

void DynamicsWidget::emitGateFilterChanged(){
    emit gateFilterChanged(gateFilterPushButton->isChecked());
}

void DynamicsWidget::emitGateFilterSoloChanged(){
    emit gateFilterSoloChanged(gateFilterSoloPushButton->isChecked());
}

void DynamicsWidget::emitGateFilterTypeChanged(){
    int value = gateFilterTypeSlider->value();
    gateFilterTypeLine->setText(
                QString::fromStdString(
                    index2filtertype(value)
                    )
                );
    emit gateFilterTypeChanged(value);
}

void DynamicsWidget::emitGateFilterFrequencyChanged(){
    float value = float(gateFilterFreqSlider->value()) / 200.f;
    gateFilterFreqLine->setText(QString::fromStdString(
                                    c201logScaleToFrequencyString(value)
                                    ));
    emit gateFilterFrequencyChanged(value);
}

compGainPaint::compGainPaint(float dpiRatio_) : QWidget() {
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(0, 0, 0, 0);
    vlayout->setSpacing(0);
    QSpacerItem *verticalSpacer = new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
    vlayout->addItem(verticalSpacer);
    this->setLayout(vlayout);
    this->setStyleSheet("background-color: rgb(96, 96, 96);");

    dpiRatio = dpiRatio_;

    backColor.setRgb(0x10, 0x10, 0x10);
    squaresColor.setRgb(0x10, 0x80, 0x30);
    lineColor.setRgb(0x10, 0xe0, 0x50);
    linePen.setColor(lineColor);
    linePen.setWidthF(qreal(2));

    squaresCache = new QPixmap;
    lineCache = new QPixmap;
}

void compGainPaint::paintEvent(QPaintEvent *event)
{
    const QRect rect = event->region().boundingRect();
    int side = rect.height();
    int center = (rect.width() / 2) - (rect.height() / 2);
    if (rect.width() < rect.height()) {
        side = rect.width();
        center = 0;
    }

    QSize paintCacheSize = QSize(side, side);
    if (squaresCache == nullptr || squaresCache->size() != paintCacheSize) {
        delete squaresCache;
        squaresCache = new QPixmap(paintCacheSize);

        QColor clearColor(0, 0, 0, 0);
        squaresCache->fill(clearColor);

        QPainter painter(squaresCache);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.translate(0, 0);
        painter.scale(1, 1);
        paintSquares(painter, paintCacheSize.height());
        painter.end();
    }

    paintCacheSize = QSize(side - 4, side - 4);
    if (lineCache == nullptr || lineCache->size() != paintCacheSize || repaint) {
        delete lineCache;
        lineCache = new QPixmap(paintCacheSize);

        QColor clearColor(0, 0, 0, 0);
        lineCache->fill(clearColor);

        QPainter painter(lineCache);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.translate(0, 0);
        painter.scale(1, 1);
        paintLine(painter, paintCacheSize.height());
        painter.end();
        repaint = false;
    }

    QPainter painter(this);

    painter.translate(0, side);
    painter.scale(1, -1);
    painter.drawPixmap(center, 0, *squaresCache);
    painter.drawPixmap(center + 2, 2, *lineCache);
}

void compGainPaint::paintSquares(QPainter &painter, int height){
    painter.setPen(backColor);
    painter.fillRect(0, 0, height, height, backColor);

    painter.setPen(lineColor);
    painter.drawRect(0, 0, height, height);
    painter.drawRect(1, 1, height - 2, height - 2);

    painter.setPen(squaresColor);
    qreal scale = height / qreal(10) ;
    for (int i = 1;i < 10; i++){
        painter.drawLine(i * scale, 2, i * scale, height - 2);
        painter.drawLine(2, i * scale, height - 2, i * scale);
    }
    painter.drawLine(2, 2, height - 2, height - 2);
}

void compGainPaint::paintLine(QPainter &painter, int height){
    qreal scale = height / qreal(60) ;
    qreal tresholdScale = scale * (treshold + 60);

    QPointF zero = QPointF(0, 0);
    QPointF treshold = QPointF(tresholdScale, tresholdScale);
    QPointF end = QPointF(qreal(height), (((qreal(height) - tresholdScale) / ratio) + tresholdScale));

    painter.setPen(linePen);
    QPainterPath painterpath(zero);

    if (knee > 0){
        qreal kneeScale = scale * qreal(knee*2);
        qreal sideb = qreal(height) - tresholdScale;
        qreal sidea = ((qreal(height) - tresholdScale) / ratio);
        qreal tgA = sidea/sideb;
        sideb = kneeScale;
        sidea = kneeScale * tgA;
        if (sidea < 0) sidea = -sidea;

        QPointF kneeStart = QPointF(tresholdScale - kneeScale, tresholdScale - kneeScale);
        QPointF kneeEnd = QPointF(tresholdScale + kneeScale, tresholdScale + sidea);

        painterpath.lineTo(kneeStart);
        painterpath.quadTo(treshold,kneeEnd);
    } else {
        painterpath.lineTo(treshold);
    }

    painterpath.lineTo(end);

    painter.drawPath(painterpath);
}

gateGainPaint::gateGainPaint(float dpiRatio_) : QWidget() {
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(0, 0, 0, 0);
    vlayout->setSpacing(0);
    QSpacerItem *verticalSpacer = new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
    vlayout->addItem(verticalSpacer);
    this->setLayout(vlayout);
    this->setStyleSheet("background-color: rgb(96, 96, 96);");

    dpiRatio = dpiRatio_;

    backColor.setRgb(0x10, 0x10, 0x10);
    squaresColor.setRgb(0x10, 0x80, 0x30);
    lineColor.setRgb(0x10, 0xe0, 0x50);
    linePen.setColor(lineColor);
    linePen.setWidthF(qreal(2));

    squaresCache = new QPixmap;
    lineCache = new QPixmap;
}

void gateGainPaint::paintEvent(QPaintEvent *event)
{
    const QRect rect = event->region().boundingRect();
    int side = rect.height();
    int center = (rect.width() / 2) - (rect.height() / 2);
    if (rect.width() < rect.height()) {
        side = rect.width();
        center = 0;
    }

    QSize paintCacheSize = QSize(side, side);
    if (squaresCache == nullptr || squaresCache->size() != paintCacheSize) {
        delete squaresCache;
        squaresCache = new QPixmap(paintCacheSize);

        QColor clearColor(0, 0, 0, 0);
        squaresCache->fill(clearColor);

        QPainter painter(squaresCache);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.translate(0, 0);
        painter.scale(1, 1);
        paintSquares(painter, paintCacheSize.height());
        painter.end();
    }

    paintCacheSize = QSize(side - 4, side - 4);
    if (lineCache == nullptr || lineCache->size() != paintCacheSize || repaint) {
        delete lineCache;
        lineCache = new QPixmap(paintCacheSize);

        QColor clearColor(0, 0, 0, 0);
        lineCache->fill(clearColor);

        QPainter painter(lineCache);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.translate(0, 0);
        painter.scale(1, 1);
        paintLine(painter, paintCacheSize.height());
        painter.end();
        repaint = false;
    }

    QPainter painter(this);

    painter.translate(0, side);
    painter.scale(1, -1);
    painter.drawPixmap(center, 0, *squaresCache);
    painter.drawPixmap(center + 2, 2, *lineCache);
}

void gateGainPaint::paintSquares(QPainter &painter, int height){
    painter.setPen(backColor);
    painter.fillRect(0, 0, height, height, backColor);

    painter.setPen(lineColor);
    painter.drawRect(0, 0, height, height);
    painter.drawRect(1, 1, height - 2, height - 2);

    painter.setPen(squaresColor);
    qreal scale = height / qreal(10) ;
    for (int i = 1;i < 10; i++){
        painter.drawLine(i * scale, 2, i * scale, height - 2);
        painter.drawLine(2, i * scale, height - 2, i * scale);
    }
    painter.drawLine(2, 2, height - 2, height - 2);
}

void gateGainPaint::paintLine(QPainter &painter, int height){
    qreal scale = height / qreal(60) ;
    qreal tresholdScale = scale * (treshold + 60);

    QPointF zero = QPointF(0, 0);
    QPointF treshold = QPointF(tresholdScale, tresholdScale);
    QPointF end = QPointF(qreal(height), (((qreal(height) - tresholdScale)) + tresholdScale));

    painter.setPen(linePen);
    QPainterPath painterpath(zero);

    painterpath.lineTo(treshold);

    painterpath.lineTo(end);

    painter.drawPath(painterpath);
}
