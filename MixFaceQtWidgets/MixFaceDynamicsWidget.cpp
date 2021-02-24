#include "MixFaceDynamicsWidget.h"
#include "MixFaceStaticMath.h"
#include <math.h>

DynamicsWidget::DynamicsWidget(int idx, float dpiRatio, MixFaceFonts *m_fonts_)
    : m_fonts(m_fonts_)
{
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(0, 0, 0, 0);
    vlayout->setSpacing(0);

    QFrame *compFrame = compWidget(dpiRatio);
    vlayout->addWidget(compFrame);

    QFrame *gateFrame = gateWidget(idx);
    vlayout->addWidget(gateFrame);

    this->setLayout(vlayout);
    this->setStyleSheet("QWidget {background-color: rgb(96, 96, 96);}");

    connectSignals();

    tablesRenew = new tablesTimer;
    tablesRenew->addGainPaint(compGain);
    tablesRenew->addEnvPaint(compEnv);
    tablesRenew->start(30);
}

DynamicsWidget::~DynamicsWidget(){
    tablesRenew->stop();
    QSlider::disconnect(tresholdSlider, &QSlider::valueChanged, this, &DynamicsWidget::emitTresholdChanged);
    QSlider::disconnect(ratioSlider, &QSlider::valueChanged, this, &DynamicsWidget::emitRatioChanged);
    QSlider::disconnect(mixSlider, &QSlider::valueChanged, this, &DynamicsWidget::emitMixChanged);
    QSlider::disconnect(gainSlider, &QSlider::valueChanged, this, &DynamicsWidget::emitGainChanged);
    QSlider::disconnect(kneeSlider, &QSlider::valueChanged, this, &DynamicsWidget::emitKneeChanged);
    QRadioButton::disconnect(compTypeRadio, &QRadioButton::clicked, this, &DynamicsWidget::emitTypeChanged);
    QRadioButton::disconnect(expTypeRadio, &QRadioButton::clicked, this, &DynamicsWidget::emitTypeChanged);
    QRadioButton::disconnect(compLinRadio, &QRadioButton::clicked, this, &DynamicsWidget::emitLinLogChanged);
    QRadioButton::disconnect(compLogRadio, &QRadioButton::clicked, this, &DynamicsWidget::emitLinLogChanged);
    QRadioButton::disconnect(compPeakRadio, &QRadioButton::clicked, this, &DynamicsWidget::emitPeakRmsChanged);
    QRadioButton::disconnect(compRmsRadio, &QRadioButton::clicked, this, &DynamicsWidget::emitPeakRmsChanged);
    QSlider::disconnect(compAttackSlider, &QSlider::valueChanged, this, &DynamicsWidget::emitAttackChanged);
    QSlider::disconnect(compHoldSlider, &QSlider::valueChanged, this, &DynamicsWidget::emitHoldChanged);
    QSlider::disconnect(compReleaseSlider, &QSlider::valueChanged, this, &DynamicsWidget::emitReleaseChanged);
    delete tablesRenew;
}

QFrame *DynamicsWidget::compWidget(float dpiRatio){

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->setSpacing(0);

    compGain = new compGainPaint(dpiRatio);
    hlayout->addWidget(compGain);

    QVBoxLayout *vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(10, 10, 10, 10);
    vlayout_->setSpacing(4);
    hlayout->addLayout(vlayout_);

    QHBoxLayout *hlayout_ = new QHBoxLayout;

    QLabel *label = new QLabel("Treshold");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setMaximumWidth(64*dpiRatio);
    hlayout_->addWidget(label);

    tresholdSlider = new QSlider;
    tresholdSlider->setOrientation(Qt::Orientation::Horizontal);
    tresholdSlider->setMaximumWidth(256*dpiRatio);
    tresholdSlider->setMaximum(120);
    tresholdSlider->setValue(87);
    tresholdSlider->setPageStep(0);
    tresholdSlider->setStyleSheet("QSlider::groove:horizontal {"
                                      "border: 1px solid #999999;"
                                      "height: "+QString::number(24*dpiRatio)+"px;"
                                      "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);"
                                      "margin: 2px 0;"
                                  "}"

                                  "QSlider::handle:horizontal {"
                                      "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);"
                                      "border: 1px solid #5c5c5c;"
                                      "width: 18px;"
                                      "margin: -2px 0; /* handle is placed by default on the contents rect of the groove. Expand outside the groove */"
                                      "border-radius: 3px;"
                                  "}");
    hlayout_->addWidget(tresholdSlider);

    tresholdLine = new QLineEdit("-16.5 db");
    tresholdLine->setStyleSheet("QLineEdit {"
                          "color: rgb(255,255,255);"
                          "background-color: rgb(96, 96, 96);"
                          "border: 2px solid rgb(32,32,32);"
                          "border-radius: 0px;"
                          "}");
    tresholdLine->setAlignment(Qt::AlignCenter);
    tresholdLine->setMaximumWidth(64*dpiRatio);
    tresholdLine->setFont(m_fonts->boldFont12);
    hlayout_->addWidget(tresholdLine);

    vlayout_->addLayout(hlayout_);

    hlayout_ = new QHBoxLayout;
    hlayout_->setContentsMargins(0, 0, 0, 0);
    hlayout_->setSpacing(0);

    label = new QLabel("Ratio");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setMaximumWidth(64*dpiRatio);
    hlayout_->addWidget(label);

    ratioSlider = new QSlider;
    ratioSlider->setOrientation(Qt::Orientation::Horizontal);
    ratioSlider->setMaximumWidth(256*dpiRatio);
    ratioSlider->setMaximum(11);
    ratioSlider->setValue(4);
    ratioSlider->setPageStep(0);
    ratioSlider->setStyleSheet("QSlider::groove:horizontal {"
                                      "border: 1px solid #999999;"
                                      "height: "+QString::number(24*dpiRatio)+"px;"
                                      "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);"
                                      "margin: 2px 0;"
                                  "}"

                                  "QSlider::handle:horizontal {"
                                      "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);"
                                      "border: 1px solid #5c5c5c;"
                                      "width: 18px;"
                                      "margin: -2px 0; /* handle is placed by default on the contents rect of the groove. Expand outside the groove */"
                                      "border-radius: 3px;"
                                  "}");
    hlayout_->addWidget(ratioSlider);

    ratioLine = new QLineEdit("2.5");
    ratioLine->setStyleSheet("QLineEdit {"
                          "color: rgb(255,255,255);"
                          "background-color: rgb(96, 96, 96);"
                          "border: 2px solid rgb(32,32,32);"
                          "border-radius: 0px;"
                          "}");
    ratioLine->setAlignment(Qt::AlignCenter);
    ratioLine->setMaximumWidth(64*dpiRatio);
    ratioLine->setFont(m_fonts->boldFont12);
    hlayout_->addWidget(ratioLine);

    vlayout_->addLayout(hlayout_);

    hlayout_ = new QHBoxLayout;
    hlayout_->setContentsMargins(0, 0, 0, 0);
    hlayout_->setSpacing(0);

    label = new QLabel("Mix");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setMaximumWidth(64*dpiRatio);
    hlayout_->addWidget(label);

    mixSlider = new QSlider;
    mixSlider->setOrientation(Qt::Orientation::Horizontal);
    mixSlider->setMaximumWidth(256*dpiRatio);
    mixSlider->setMaximum(20);
    mixSlider->setValue(20);
    mixSlider->setPageStep(0);
    mixSlider->setStyleSheet("QSlider::groove:horizontal {"
                                      "border: 1px solid #999999;"
                                      "height: "+QString::number(24*dpiRatio)+"px;"
                                      "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);"
                                      "margin: 2px 0;"
                                  "}"

                                  "QSlider::handle:horizontal {"
                                      "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);"
                                      "border: 1px solid #5c5c5c;"
                                      "width: 18px;"
                                      "margin: -2px 0; /* handle is placed by default on the contents rect of the groove. Expand outside the groove */"
                                      "border-radius: 3px;"
                                  "}");
    hlayout_->addWidget(mixSlider);

    mixLine = new QLineEdit("100 %");
    mixLine->setStyleSheet("QLineEdit {"
                          "color: rgb(255,255,255);"
                          "background-color: rgb(96, 96, 96);"
                          "border: 2px solid rgb(32,32,32);"
                          "border-radius: 0px;"
                          "}");
    mixLine->setAlignment(Qt::AlignCenter);
    mixLine->setMaximumWidth(64*dpiRatio);
    mixLine->setFont(m_fonts->boldFont12);
    hlayout_->addWidget(mixLine);

    vlayout_->addLayout(hlayout_);

    hlayout_ = new QHBoxLayout;
    hlayout_->setContentsMargins(0, 0, 0, 0);
    hlayout_->setSpacing(0);

    label = new QLabel("Gain");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setMaximumWidth(64*dpiRatio);
    hlayout_->addWidget(label);

    gainSlider = new QSlider;
    gainSlider->setOrientation(Qt::Orientation::Horizontal);
    gainSlider->setMaximumWidth(256*dpiRatio);
    gainSlider->setMaximum(48);
    gainSlider->setValue(0);
    gainSlider->setPageStep(0);
    gainSlider->setStyleSheet("QSlider::groove:horizontal {"
                                      "border: 1px solid #999999;"
                                      "height: "+QString::number(24*dpiRatio)+"px;"
                                      "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);"
                                      "margin: 2px 0;"
                                  "}"

                                  "QSlider::handle:horizontal {"
                                      "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);"
                                      "border: 1px solid #5c5c5c;"
                                      "width: 18px;"
                                      "margin: -2px 0; /* handle is placed by default on the contents rect of the groove. Expand outside the groove */"
                                      "border-radius: 3px;"
                                  "}");
    hlayout_->addWidget(gainSlider);

    gainLine = new QLineEdit("0 db");
    gainLine->setStyleSheet("QLineEdit {"
                          "color: rgb(255,255,255);"
                          "background-color: rgb(96, 96, 96);"
                          "border: 2px solid rgb(32,32,32);"
                          "border-radius: 0px;"
                          "}");
    gainLine->setAlignment(Qt::AlignCenter);
    gainLine->setMaximumWidth(64*dpiRatio);
    gainLine->setFont(m_fonts->boldFont12);
    hlayout_->addWidget(gainLine);

    vlayout_->addLayout(hlayout_);

    hlayout_ = new QHBoxLayout;
    hlayout_->setContentsMargins(0, 0, 0, 0);
    hlayout_->setSpacing(0);

    label = new QLabel("Knee");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setMaximumWidth(64*dpiRatio);
    hlayout_->addWidget(label);

    kneeSlider = new QSlider;
    kneeSlider->setOrientation(Qt::Orientation::Horizontal);
    kneeSlider->setMaximumWidth(256*dpiRatio);
    kneeSlider->setMaximum(5);
    kneeSlider->setValue(0);
    kneeSlider->setPageStep(0);
    kneeSlider->setStyleSheet("QSlider::groove:horizontal {"
                                      "border: 1px solid #999999;"
                                      "height: "+QString::number(24*dpiRatio)+"px;"
                                      "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);"
                                      "margin: 2px 0;"
                                  "}"

                                  "QSlider::handle:horizontal {"
                                      "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);"
                                      "border: 1px solid #5c5c5c;"
                                      "width: 18px;"
                                      "margin: -2px 0; /* handle is placed by default on the contents rect of the groove. Expand outside the groove */"
                                      "border-radius: 3px;"
                                  "}");
    hlayout_->addWidget(kneeSlider);

    kneeLine = new QLineEdit("0");
    kneeLine->setStyleSheet("QLineEdit {"
                          "color: rgb(255,255,255);"
                          "background-color: rgb(96, 96, 96);"
                          "border: 2px solid rgb(32,32,32);"
                          "border-radius: 0px;"
                          "}");
    kneeLine->setAlignment(Qt::AlignCenter);
    kneeLine->setMaximumWidth(64*dpiRatio);
    kneeLine->setFont(m_fonts->boldFont12);
    hlayout_->addWidget(kneeLine);

    vlayout_->addLayout(hlayout_);

    hlayout_ = new QHBoxLayout;
    hlayout_->setContentsMargins(0, 0, 0, 0);
    hlayout_->setSpacing(0);

    compType = new QButtonGroup;
    compTypeRadio = new QRadioButton("Comp");
    compTypeRadio->setStyleSheet("QRadioButton {color: rgb(255,255,255);}");
    compTypeRadio->setFont(m_fonts->boldFont8);
    compType->addButton(compTypeRadio);
    compTypeRadio->setChecked(true);
    hlayout_->addWidget(compTypeRadio);

    expTypeRadio = new QRadioButton("Exp");
    expTypeRadio->setStyleSheet("QRadioButton {color: rgb(255,255,255);}");
    expTypeRadio->setFont(m_fonts->boldFont8);
    compType->addButton(expTypeRadio);
    hlayout_->addWidget(expTypeRadio);

    vlayout_->addLayout(hlayout_);

    //Comp Env Widgets

    compEnv = new compEnvPaint(dpiRatio);
    //hlayout->addWidget(compEnv);

    hlayout_ = new QHBoxLayout;
    hlayout_->setContentsMargins(0, 0, 0, 0);
    hlayout_->setSpacing(0);

    compLinLogType = new QButtonGroup;

    compLinRadio = new QRadioButton("Lin");
    compLinRadio->setStyleSheet("QRadioButton {color: rgb(255,255,255);}");
    compLinRadio->setFont(m_fonts->boldFont8);
    compLinLogType->addButton(compLinRadio);
    hlayout_->addWidget(compLinRadio);

    compLogRadio = new QRadioButton("Log");
    compLogRadio->setStyleSheet("QRadioButton {color: rgb(255,255,255);}");
    compLogRadio->setFont(m_fonts->boldFont8);
    compLinLogType->addButton(compLogRadio);
    compLogRadio->setChecked(true);
    hlayout_->addWidget(compLogRadio);

    vlayout_->addLayout(hlayout_);

    hlayout_ = new QHBoxLayout;
    hlayout_->setContentsMargins(0, 0, 0, 0);
    hlayout_->setSpacing(0);

    compPeakRmsType = new QButtonGroup;

    compPeakRadio = new QRadioButton("Peak");
    compPeakRadio->setStyleSheet("QRadioButton {color: rgb(255,255,255);}");
    compPeakRadio->setFont(m_fonts->boldFont8);
    compPeakRmsType->addButton(compPeakRadio);
    compPeakRadio->setChecked(true);
    hlayout_->addWidget(compPeakRadio);

    compRmsRadio = new QRadioButton("RMS");
    compRmsRadio->setStyleSheet("QRadioButton {color: rgb(255,255,255);}");
    compRmsRadio->setFont(m_fonts->boldFont8);
    compPeakRmsType->addButton(compRmsRadio);
    hlayout_->addWidget(compRmsRadio);

    vlayout_->addLayout(hlayout_);

    hlayout_ = new QHBoxLayout;
    hlayout_->setContentsMargins(0, 0, 0, 0);
    hlayout_->setSpacing(0);

    label = new QLabel("Attack");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setMaximumWidth(64*dpiRatio);
    hlayout_->addWidget(label);

    compAttackSlider = new QSlider;
    compAttackSlider->setOrientation(Qt::Orientation::Horizontal);
    compAttackSlider->setMaximumWidth(256*dpiRatio);
    compAttackSlider->setMaximum(120);
    compAttackSlider->setValue(0);
    compAttackSlider->setPageStep(0);
    compAttackSlider->setStyleSheet("QSlider::groove:horizontal {"
                                      "border: 1px solid #999999;"
                                      "height: "+QString::number(24*dpiRatio)+"px;"
                                      "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);"
                                      "margin: 2px 0;"
                                  "}"

                                  "QSlider::handle:horizontal {"
                                      "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);"
                                      "border: 1px solid #5c5c5c;"
                                      "width: 18px;"
                                      "margin: -2px 0; /* handle is placed by default on the contents rect of the groove. Expand outside the groove */"
                                      "border-radius: 3px;"
                                  "}");
    hlayout_->addWidget(compAttackSlider);

    compAttackLine = new QLineEdit("0");
    compAttackLine->setStyleSheet("QLineEdit {"
                          "color: rgb(255,255,255);"
                          "background-color: rgb(96, 96, 96);"
                          "border: 2px solid rgb(32,32,32);"
                          "border-radius: 0px;"
                          "}");
    compAttackLine->setAlignment(Qt::AlignCenter);
    compAttackLine->setMaximumWidth(64*dpiRatio);
    compAttackLine->setFont(m_fonts->boldFont12);
    hlayout_->addWidget(compAttackLine);

    vlayout_->addLayout(hlayout_);

    hlayout_ = new QHBoxLayout;
    hlayout_->setContentsMargins(0, 0, 0, 0);
    hlayout_->setSpacing(0);

    label = new QLabel("Hold");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setMaximumWidth(64*dpiRatio);
    hlayout_->addWidget(label);

    compHoldSlider = new QSlider;
    compHoldSlider->setOrientation(Qt::Orientation::Horizontal);
    compHoldSlider->setMaximumWidth(256*dpiRatio);
    compHoldSlider->setMaximum(100);
    compHoldSlider->setValue(0);
    compHoldSlider->setPageStep(0);
    compHoldSlider->setStyleSheet("QSlider::groove:horizontal {"
                                      "border: 1px solid #999999;"
                                      "height: "+QString::number(24*dpiRatio)+"px;"
                                      "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);"
                                      "margin: 2px 0;"
                                  "}"

                                  "QSlider::handle:horizontal {"
                                      "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);"
                                      "border: 1px solid #5c5c5c;"
                                      "width: 18px;"
                                      "margin: -2px 0; /* handle is placed by default on the contents rect of the groove. Expand outside the groove */"
                                      "border-radius: 3px;"
                                  "}");
    hlayout_->addWidget(compHoldSlider);

    compHoldLine = new QLineEdit("0");
    compHoldLine->setStyleSheet("QLineEdit {"
                          "color: rgb(255,255,255);"
                          "background-color: rgb(96, 96, 96);"
                          "border: 2px solid rgb(32,32,32);"
                          "border-radius: 0px;"
                          "}");
    compHoldLine->setAlignment(Qt::AlignCenter);
    compHoldLine->setMaximumWidth(64*dpiRatio);
    compHoldLine->setFont(m_fonts->boldFont12);
    hlayout_->addWidget(compHoldLine);

    vlayout_->addLayout(hlayout_);

    hlayout_ = new QHBoxLayout;
    hlayout_->setContentsMargins(0, 0, 0, 0);
    hlayout_->setSpacing(0);

    label = new QLabel("Release");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setMaximumWidth(64*dpiRatio);
    hlayout_->addWidget(label);

    compReleaseSlider = new QSlider;
    compReleaseSlider->setOrientation(Qt::Orientation::Horizontal);
    compReleaseSlider->setMaximumWidth(256*dpiRatio);
    compReleaseSlider->setMaximum(100);
    compReleaseSlider->setValue(0);
    compReleaseSlider->setPageStep(0);
    compReleaseSlider->setStyleSheet("QSlider::groove:horizontal {"
                                      "border: 1px solid #999999;"
                                      "height: "+QString::number(24*dpiRatio)+"px;"
                                      "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);"
                                      "margin: 2px 0;"
                                  "}"

                                  "QSlider::handle:horizontal {"
                                      "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);"
                                      "border: 1px solid #5c5c5c;"
                                      "width: 18px;"
                                      "margin: -2px 0; /* handle is placed by default on the contents rect of the groove. Expand outside the groove */"
                                      "border-radius: 3px;"
                                  "}");
    hlayout_->addWidget(compReleaseSlider);

    compReleaseLine = new QLineEdit("0");
    compReleaseLine->setStyleSheet("QLineEdit {"
                          "color: rgb(255,255,255);"
                          "background-color: rgb(96, 96, 96);"
                          "border: 2px solid rgb(32,32,32);"
                          "border-radius: 0px;"
                          "}");
    compReleaseLine->setAlignment(Qt::AlignCenter);
    compReleaseLine->setMaximumWidth(64*dpiRatio);
    compReleaseLine->setFont(m_fonts->boldFont12);
    hlayout_->addWidget(compReleaseLine);

    vlayout_->addLayout(hlayout_);

    QFrame *frame = new QFrame;
    frame->setLayout(hlayout);
    frame->setFrameShape(QFrame::Box);
    frame->setFrameShadow(QFrame::Plain);
    return frame;
}

QFrame *DynamicsWidget::gateWidget(int idx){
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->setSpacing(0);

    QLabel *label = new QLabel("DYN #" + QString::number(idx));
    label->setFont(m_fonts->boldFont12);
    hlayout->addWidget(label);

    QFrame *frame = new QFrame;
    frame->setLayout(hlayout);
    frame->setFrameShape(QFrame::Box);
    frame->setFrameShadow(QFrame::Plain);
    return frame;
}

void DynamicsWidget::connectSignals(){
    QSlider::connect(tresholdSlider, &QSlider::valueChanged, this, &DynamicsWidget::emitTresholdChanged);
    QSlider::connect(ratioSlider, &QSlider::valueChanged, this, &DynamicsWidget::emitRatioChanged);
    QSlider::connect(mixSlider, &QSlider::valueChanged, this, &DynamicsWidget::emitMixChanged);
    QSlider::connect(gainSlider, &QSlider::valueChanged, this, &DynamicsWidget::emitGainChanged);
    QSlider::connect(kneeSlider, &QSlider::valueChanged, this, &DynamicsWidget::emitKneeChanged);
    QRadioButton::connect(compTypeRadio, &QRadioButton::clicked, this, &DynamicsWidget::emitTypeChanged);
    QRadioButton::connect(expTypeRadio, &QRadioButton::clicked, this, &DynamicsWidget::emitTypeChanged);
    QRadioButton::connect(compLinRadio, &QRadioButton::clicked, this, &DynamicsWidget::emitLinLogChanged);
    QRadioButton::connect(compLogRadio, &QRadioButton::clicked, this, &DynamicsWidget::emitLinLogChanged);
    QRadioButton::connect(compPeakRadio, &QRadioButton::clicked, this, &DynamicsWidget::emitPeakRmsChanged);
    QRadioButton::connect(compRmsRadio, &QRadioButton::clicked, this, &DynamicsWidget::emitPeakRmsChanged);
    QSlider::connect(compAttackSlider, &QSlider::valueChanged, this, &DynamicsWidget::emitAttackChanged);
    QSlider::connect(compHoldSlider, &QSlider::valueChanged, this, &DynamicsWidget::emitHoldChanged);
    QSlider::connect(compReleaseSlider, &QSlider::valueChanged, this, &DynamicsWidget::emitReleaseChanged);
}

void DynamicsWidget::emitTresholdChanged(){
    float value = float(tresholdSlider->value()) / 120;
    float db = float(tresholdSlider->value()) / 2 - 60.;
    compGain->setTreshold(db);
    tresholdLine->setText(QString::number(db) + " db");
    emit tresholdChanged(value);
}

void DynamicsWidget::emitRatioChanged(){
    int value = ratioSlider->value();
    float ratio = int2ratio(value);
    compGain->setRatio(ratio);
    ratioLine->setText(QString::number(ratio));
    emit ratioChanged(value);
}

void DynamicsWidget::emitMixChanged(){
    float value = float(mixSlider->value()) / 20;
    mixLine->setText(QString::number(value*100) + " %");
    emit mixChanged(value);
}

void DynamicsWidget::emitGainChanged(){
    float value = float(gainSlider->value()) / 48;
    gainLine->setText(QString::number(value*24) + " db");
    emit gainChanged(value);
}

void DynamicsWidget::emitKneeChanged(){
    float value = float(kneeSlider->value()) / 5;
    kneeLine->setText(QString::number(kneeSlider->value()));
    compGain->setKnee(kneeSlider->value());
    emit kneeChanged(value);
}

void DynamicsWidget::emitTypeChanged(){
    int value = 0;
    if (expTypeRadio->isChecked()) value = 1;
    compGain->setType(value);
    emit typeChanged(value);
}

void DynamicsWidget::emitLinLogChanged(){
    int value = 0;
    if (compLogRadio->isChecked()) value = 1;
    emit linLogChanged(value);
}

void DynamicsWidget::emitPeakRmsChanged(){
    int value = 0;
    if (compRmsRadio->isChecked()) value = 1;
    emit peakRmsChanged(value);
}

void DynamicsWidget::emitAttackChanged(){
    float value = float(compAttackSlider->value()) / 120;
    emit attackChanged(value);
    compAttackLine->setText(QString::number(compAttackSlider->value()) + " ms");
}

void DynamicsWidget::emitHoldChanged(){
    float value = float(compHoldSlider->value()) / 100;
    emit holdChanged(value);
    compHoldLine->setText(QString::number(hold2ms(compHoldSlider->value())) + " ms");
}

void DynamicsWidget::emitReleaseChanged(){
    float value = float(compReleaseSlider->value()) / 100;
    emit releaseChanged(value);
    compReleaseLine->setText(QString::number(release2ms(compReleaseSlider->value())) + " ms");
}

void DynamicsWidget::emitAutoTimeChanged(){}

void DynamicsWidget::emitKeySourceChanged(){}

void DynamicsWidget::emitFilterChanged(){}

void DynamicsWidget::emitFilterTypeChanged(){}

void DynamicsWidget::emitFilterFrequencyChanged(){}

compGainPaint::compGainPaint(float dpiRatio_) : QWidget() {
    dpiRatio = dpiRatio_;

    backColor.setRgb(0x10, 0x10, 0x10);
    squaresColor.setRgb(0x10, 0x80, 0x30);
    lineColor.setRgb(0x10, 0xe0, 0x50);
    linePen.setColor(lineColor);
    linePen.setWidthF(qreal(2.5));

    squaresCache = new QPixmap;
    lineCache = new QPixmap;
}

void compGainPaint::paintEvent(QPaintEvent *event)
{
    const QRect rect = event->region().boundingRect();
    int side = rect.height();
    if (rect.width() < rect.height())
        side = rect.width();
    this->setMaximumSize(side,side);

    QSize paintCacheSize = QSize(side - 32, side - 32);
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

    paintCacheSize = QSize(side - 40, side - 40);
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
    painter.drawPixmap(16, 16, *squaresCache);
    painter.drawPixmap(20, 20, *lineCache);
}

void compGainPaint::paintSquares(QPainter &painter, int height){
    painter.setPen(backColor);
    painter.fillRect(4,4,height-8,height-8,backColor);

    painter.setPen(lineColor);
    for(int i=0;i<4;i++){
        painter.drawRect(i,i,(height-(i*2))-1,(height-(i*2))-1);
    }

    painter.setPen(squaresColor);
    qreal scale = height / qreal(10) ;
    for (int i = 1;i < 10; i++){
        painter.drawLine(i * scale, 4, i * scale, height-5);
        painter.drawLine(4, i*scale, height-5, i*scale);
    }
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

compEnvPaint::compEnvPaint(float dpiRatio_) : QWidget() {
    dpiRatio = dpiRatio_;

    backColor.setRgb(0x10, 0x10, 0x10);
    squaresColor.setRgb(0x10, 0x80, 0x30);
    lineColor.setRgb(0x10, 0xe0, 0x50);
    linePen.setColor(lineColor);
    linePen.setWidthF(qreal(2.5));

    squaresCache = new QPixmap;
    lineCache = new QPixmap;
}

void compEnvPaint::paintEvent(QPaintEvent *event)
{
    const QRect rect = event->region().boundingRect();
    int side = rect.height();
    if (rect.width() < rect.height())
        side = rect.width();

    QSize paintCacheSize = QSize(side - 32, side - 32);
    if (squaresCache == nullptr || squaresCache->size() != paintCacheSize) {
        delete squaresCache;
        squaresCache = new QPixmap(paintCacheSize);

        QColor clearColor(0, 0, 0, 0);
        squaresCache->fill(clearColor);

        QPainter painter(squaresCache);
        //painter.setRenderHint(QPainter::Antialiasing, true);
        painter.translate(0, 0);
        painter.scale(1, 1);
        paintSquares(painter, paintCacheSize.height());
        painter.end();
    }

    paintCacheSize = QSize(side - 40, side - 40);
    if (lineCache == nullptr || lineCache->size() != paintCacheSize || repaint) {
        delete lineCache;
        lineCache = new QPixmap(paintCacheSize);

        QColor clearColor(0, 0, 0, 0);
        lineCache->fill(clearColor);

        QPainter painter(lineCache);
        //painter.setRenderHint(QPainter::Antialiasing, true);
        painter.translate(0, 0);
        painter.scale(1, 1);
        paintLine(painter, paintCacheSize.height());
        painter.end();
        repaint = false;
    }

    QPainter painter(this);

    painter.translate(0, side);
    painter.scale(1, -1);

    painter.drawPixmap(16, 16, *squaresCache);
    painter.drawPixmap(20, 20, *lineCache);
}

void compEnvPaint::paintSquares(QPainter &painter, int height){
    painter.setPen(backColor);
    painter.fillRect(4,4,height-8,height-8,backColor);

    painter.setPen(lineColor);
    for(int i=0;i<4;i++){
        painter.drawRect(i,i,(height-(i*2))-1,(height-(i*2))-1);
    }

    painter.setPen(squaresColor);
    qreal scale = height / qreal(10) ;
    for (int i = 1;i < 10; i++){
        painter.drawLine(i * scale, 4, i * scale, height-5);
        painter.drawLine(4, i*scale, height-5, i*scale);
    }
}

void compEnvPaint::paintLine(QPainter &painter, int height){
    Q_UNUSED(painter)
    Q_UNUSED(height)
}
