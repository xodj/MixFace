#include "MixFaceDynamicsWidget.h"
#include "MixFaceStaticMath.h"
#include <math.h>

#include <QDebug>

DynamicsWidget::DynamicsWidget(float dpiRatio, MixFaceFonts *m_fonts_)
    : m_fonts(m_fonts_), dpiRatio(dpiRatio)
{
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
    tablesRenew->addEnvPaint(compEnv);
    tablesRenew->addFilterPaint(compFilter);
    tablesRenew->start(30);
}

DynamicsWidget::~DynamicsWidget(){
    tablesRenew->stop();
    QPushButton::disconnect(compActivePushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitOnOffChanged);
    QPushButton::disconnect(compAutoPushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitAutoTimeChanged);
    QPushButton::disconnect(compFilterPushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitFilterChanged);
    /*QPushButton::disconnect(compFilterSoloPushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitCompFilterSoloChanged);*/
    QComboBox::disconnect(compFilterSourceComboBox, &QComboBox::currentIndexChanged,
                         this, &DynamicsWidget::emitKeySourceChanged);
    QSlider::disconnect(tresholdSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitTresholdChanged);
    QSlider::disconnect(ratioSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitRatioChanged);
    QSlider::disconnect(mixSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitMixChanged);
    QSlider::disconnect(gainSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitGainChanged);
    QSlider::disconnect(kneeSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitKneeChanged);
    QRadioButton::disconnect(compTypeRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitModeCompExpChanged);
    QRadioButton::disconnect(expTypeRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitModeCompExpChanged);
    QRadioButton::disconnect(compLinRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitEnvLinLogChanged);
    QRadioButton::disconnect(compLogRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitEnvLinLogChanged);
    QRadioButton::disconnect(compPeakRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitDetPeakRmsChanged);
    QRadioButton::disconnect(compRmsRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitDetPeakRmsChanged);
    QSlider::disconnect(compAttackSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitAttackChanged);
    QSlider::disconnect(compHoldSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitHoldChanged);
    QSlider::disconnect(compReleaseSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitReleaseChanged);
    delete tablesRenew;
}

QFrame *DynamicsWidget::compWidget(){
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setContentsMargins(16, 0, 0, 0);
    hlayout->setSpacing(0);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(4, 16, 4, 4);
    vlayout->setSpacing(4);

    compActivePushButton = new QPushButton("Active");
    compActivePushButton->setCheckable(true);
    compActivePushButton->setMinimumSize(72*dpiRatio, 24*dpiRatio);
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
    compActivePushButton->setFont(m_fonts->boldFont12);
    vlayout->addWidget(compActivePushButton);

    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    vlayout->addItem(verticalSpacer);

    hlayout->addLayout(vlayout);

    vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(4, 4, 4, 4);
    vlayout->setSpacing(4);

    QHBoxLayout *hlayout_ = new QHBoxLayout;
    hlayout_->setContentsMargins(4, 4, 4, 4);
    hlayout_->setSpacing(4);
    //h1
    /*Gain*/
    QVBoxLayout *vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);

    QLabel *label = new QLabel("Gain");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setAlignment(Qt::AlignCenter);
    vlayout_->addWidget(label);

    compGain = new compGainPaint(dpiRatio);
    vlayout_->addWidget(compGain);

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

    vlayout_->addLayout(radiohlayout);

    hlayout_->addLayout(vlayout_);
    /*knee*/
    vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);

    kneeLine = new QLineEdit("0");
    kneeLine->setStyleSheet("QLineEdit {"
                          "color: rgb(255,255,255);"
                          "background-color: rgb(96, 96, 96);"
                          "border: 2px solid rgb(32,32,32);"
                          "border-radius: 0px;"
                          "}");
    kneeLine->setAlignment(Qt::AlignCenter);
    kneeLine->setMaximumWidth(64*dpiRatio);
    kneeLine->setFont(m_fonts->boldFont8);
    vlayout_->addWidget(kneeLine);

    QHBoxLayout *sliderslayout = new QHBoxLayout;
    sliderslayout->setContentsMargins(0, 0, 0, 0);
    sliderslayout->setSpacing(0);
    kneeSlider = new QSlider;
    kneeSlider->setOrientation(Qt::Orientation::Vertical);
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

    hlayout_->addLayout(vlayout_);
    /*linlogpeakrms*/
    vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);


    verticalSpacer = new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
    vlayout_->addItem(verticalSpacer);

    compLinLogType = new QButtonGroup;

    compLinRadio = new QRadioButton("Lin");
    compLinRadio->setStyleSheet("QRadioButton {color: rgb(255,255,255);}");
    compLinRadio->setFont(m_fonts->boldFont8);
    compLinLogType->addButton(compLinRadio);
    vlayout_->addWidget(compLinRadio);

    compLogRadio = new QRadioButton("Log");
    compLogRadio->setStyleSheet("QRadioButton {color: rgb(255,255,255);}");
    compLogRadio->setFont(m_fonts->boldFont8);
    compLinLogType->addButton(compLogRadio);
    compLogRadio->setChecked(true);
    vlayout_->addWidget(compLogRadio);

    verticalSpacer = new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
    vlayout_->addItem(verticalSpacer);

    compPeakRmsType = new QButtonGroup;

    compPeakRadio = new QRadioButton("Peak");
    compPeakRadio->setStyleSheet("QRadioButton {color: rgb(255,255,255);}");
    compPeakRadio->setFont(m_fonts->boldFont8);
    compPeakRmsType->addButton(compPeakRadio);
    compPeakRadio->setChecked(true);
    vlayout_->addWidget(compPeakRadio);

    compRmsRadio = new QRadioButton("RMS");
    compRmsRadio->setStyleSheet("QRadioButton {color: rgb(255,255,255);}");
    compRmsRadio->setFont(m_fonts->boldFont8);
    compPeakRmsType->addButton(compRmsRadio);
    vlayout_->addWidget(compRmsRadio);

    verticalSpacer = new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
    vlayout_->addItem(verticalSpacer);


    hlayout_->addLayout(vlayout_);
    /*env*/
    QSpacerItem *horizontalSpacer = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hlayout_->addItem(horizontalSpacer);

    vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);


    label = new QLabel("Envelope");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setAlignment(Qt::AlignCenter);
    vlayout_->addWidget(label);

    //Comp Env Widgets
    compEnv = new compEnvPaint(dpiRatio);
    vlayout_->addWidget(compEnv);

    //Add auto time button
    QHBoxLayout *autohlayout = new QHBoxLayout;
    autohlayout->setContentsMargins(0, 0, 0, 0);
    autohlayout->setSpacing(0);

    horizontalSpacer = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);
    autohlayout->addItem(horizontalSpacer);

    compAutoPushButton = new QPushButton("Auto Time");
    compAutoPushButton->setCheckable(true);
    compAutoPushButton->setMinimumSize(96*dpiRatio, 24*dpiRatio);
    compAutoPushButton->setStyleSheet("QPushButton {"
                          "color: rgb(0,0,0);"
                          "background-color: rgb(196, 196, 196);"
                          "border: 1px solid rgb(32,32,32);"
                          "border-radius: 0px;}"
                          "QPushButton:pressed {"
                          "background-color: rgb(128, 128, 128);}"
                          "QPushButton:checked {"
                          "background-color: rgb(128, 255, 128);}"
                          "QPushButton:checked:pressed {"
                          "background-color: rgb(128, 196, 128);}");
    compAutoPushButton->setFont(m_fonts->boldFont8);
    autohlayout->addWidget(compAutoPushButton);

    horizontalSpacer = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);
    autohlayout->addItem(horizontalSpacer);

    vlayout_->addLayout(autohlayout);

    hlayout_->addLayout(vlayout_);
    /*filter*/
    horizontalSpacer = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hlayout_->addItem(horizontalSpacer);

    vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);

    //Comp Filter Widgets
    label = new QLabel("Filter");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setAlignment(Qt::AlignCenter);
    vlayout_->addWidget(label);

    compFilter = new compFilterPaint(dpiRatio);
    vlayout_->addWidget(compFilter);

    //add key source
    autohlayout = new QHBoxLayout;
    autohlayout->setContentsMargins(0, 0, 0, 0);
    autohlayout->setSpacing(0);

    horizontalSpacer = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);
    autohlayout->addItem(horizontalSpacer);

    compFilterSourceComboBox = new QComboBox;
    compFilterSourceComboBox->setMinimumSize(72*dpiRatio, 24*dpiRatio);
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
    autohlayout->addWidget(compFilterSourceComboBox);

    horizontalSpacer = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);
    autohlayout->addItem(horizontalSpacer);

    vlayout_->addLayout(autohlayout);

    hlayout_->addLayout(vlayout_);

    vlayout->addLayout(hlayout_);

    hlayout_ = new QHBoxLayout;
    hlayout_->setContentsMargins(0, 0, 0, 0);
    hlayout_->setSpacing(0);
    /*treshold*/
    vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);


    tresholdLine = new QLineEdit("-16.5 db");
    tresholdLine->setStyleSheet("QLineEdit {"
                          "color: rgb(255,255,255);"
                          "background-color: rgb(96, 96, 96);"
                          "border: 2px solid rgb(32,32,32);"
                          "border-radius: 0px;"
                          "}");
    tresholdLine->setAlignment(Qt::AlignCenter);
    tresholdLine->setMaximumWidth(64*dpiRatio);
    tresholdLine->setFont(m_fonts->boldFont8);
    vlayout_->addWidget(tresholdLine);

    sliderslayout = new QHBoxLayout;
    sliderslayout->setContentsMargins(0, 0, 0, 0);
    sliderslayout->setSpacing(0);
    tresholdSlider = new QSlider;
    tresholdSlider->setOrientation(Qt::Orientation::Vertical);
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


    hlayout_->addLayout(vlayout_);
    /*ratio*/
    vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);


    ratioLine = new QLineEdit("2.5");
    ratioLine->setStyleSheet("QLineEdit {"
                          "color: rgb(255,255,255);"
                          "background-color: rgb(96, 96, 96);"
                          "border: 2px solid rgb(32,32,32);"
                          "border-radius: 0px;"
                          "}");
    ratioLine->setAlignment(Qt::AlignCenter);
    ratioLine->setMaximumWidth(64*dpiRatio);
    ratioLine->setFont(m_fonts->boldFont8);
    vlayout_->addWidget(ratioLine);

    sliderslayout = new QHBoxLayout;
    sliderslayout->setContentsMargins(0, 0, 0, 0);
    sliderslayout->setSpacing(0);
    ratioSlider = new QSlider;
    ratioSlider->setOrientation(Qt::Orientation::Vertical);
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


    hlayout_->addLayout(vlayout_);
    /*mix*/
    vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);


    mixLine = new QLineEdit("100 %");
    mixLine->setStyleSheet("QLineEdit {"
                          "color: rgb(255,255,255);"
                          "background-color: rgb(96, 96, 96);"
                          "border: 2px solid rgb(32,32,32);"
                          "border-radius: 0px;"
                          "}");
    mixLine->setAlignment(Qt::AlignCenter);
    mixLine->setMaximumWidth(64*dpiRatio);
    mixLine->setFont(m_fonts->boldFont8);
    vlayout_->addWidget(mixLine);

    sliderslayout = new QHBoxLayout;
    sliderslayout->setContentsMargins(0, 0, 0, 0);
    sliderslayout->setSpacing(0);
    mixSlider = new QSlider;
    mixSlider->setOrientation(Qt::Orientation::Vertical);
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


    hlayout_->addLayout(vlayout_);
    /*gain*/
    vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);


    gainLine = new QLineEdit("0 db");
    gainLine->setStyleSheet("QLineEdit {"
                          "color: rgb(255,255,255);"
                          "background-color: rgb(96, 96, 96);"
                          "border: 2px solid rgb(32,32,32);"
                          "border-radius: 0px;"
                          "}");
    gainLine->setAlignment(Qt::AlignCenter);
    gainLine->setMaximumWidth(64*dpiRatio);
    gainLine->setFont(m_fonts->boldFont8);
    vlayout_->addWidget(gainLine);

    sliderslayout = new QHBoxLayout;
    sliderslayout->setContentsMargins(0, 0, 0, 0);
    sliderslayout->setSpacing(0);
    gainSlider = new QSlider;
    gainSlider->setOrientation(Qt::Orientation::Vertical);
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

    hlayout_->addLayout(vlayout_);

    horizontalSpacer = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hlayout_->addItem(horizontalSpacer);

    /*attack*/
    vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);


    compAttackLine = new QLineEdit("0");
    compAttackLine->setStyleSheet("QLineEdit {"
                          "color: rgb(255,255,255);"
                          "background-color: rgb(96, 96, 96);"
                          "border: 2px solid rgb(32,32,32);"
                          "border-radius: 0px;"
                          "}");
    compAttackLine->setAlignment(Qt::AlignCenter);
    compAttackLine->setMaximumWidth(64*dpiRatio);
    compAttackLine->setFont(m_fonts->boldFont8);
    vlayout_->addWidget(compAttackLine);

    sliderslayout = new QHBoxLayout;
    sliderslayout->setContentsMargins(0, 0, 0, 0);
    sliderslayout->setSpacing(0);
    compAttackSlider = new QSlider;
    compAttackSlider->setOrientation(Qt::Orientation::Vertical);
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


    hlayout_->addLayout(vlayout_);
    /*hold*/
    vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);


    compHoldLine = new QLineEdit("0");
    compHoldLine->setStyleSheet("QLineEdit {"
                          "color: rgb(255,255,255);"
                          "background-color: rgb(96, 96, 96);"
                          "border: 2px solid rgb(32,32,32);"
                          "border-radius: 0px;"
                          "}");
    compHoldLine->setAlignment(Qt::AlignCenter);
    compHoldLine->setMaximumWidth(64*dpiRatio);
    compHoldLine->setFont(m_fonts->boldFont8);
    vlayout_->addWidget(compHoldLine);

    sliderslayout = new QHBoxLayout;
    sliderslayout->setContentsMargins(0, 0, 0, 0);
    sliderslayout->setSpacing(0);
    compHoldSlider = new QSlider;
    compHoldSlider->setOrientation(Qt::Orientation::Vertical);
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


    hlayout_->addLayout(vlayout_);
    /*release*/
    vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);


    compReleaseLine = new QLineEdit("0");
    compReleaseLine->setStyleSheet("QLineEdit {"
                          "color: rgb(255,255,255);"
                          "background-color: rgb(96, 96, 96);"
                          "border: 2px solid rgb(32,32,32);"
                          "border-radius: 0px;"
                          "}");
    compReleaseLine->setAlignment(Qt::AlignCenter);
    compReleaseLine->setMaximumWidth(64*dpiRatio);
    compReleaseLine->setFont(m_fonts->boldFont8);
    vlayout_->addWidget(compReleaseLine);

    sliderslayout = new QHBoxLayout;
    sliderslayout->setContentsMargins(0, 0, 0, 0);
    sliderslayout->setSpacing(0);
    compReleaseSlider = new QSlider;
    compReleaseSlider->setOrientation(Qt::Orientation::Vertical);
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

    hlayout_->addLayout(vlayout_);

    horizontalSpacer = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hlayout_->addItem(horizontalSpacer);

    /*filter/solo*/
    vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);

    compFilterPushButton = new QPushButton("Filter");
    compFilterPushButton->setCheckable(true);
    compFilterPushButton->setMinimumSize(48*dpiRatio, 24*dpiRatio);
    compFilterPushButton->setMaximumSize(48*dpiRatio, 24*dpiRatio);
    compFilterPushButton->setStyleSheet("QPushButton {"
                          "color: rgb(0,0,0);"
                          "background-color: rgb(196, 196, 196);"
                          "border: 1px solid rgb(32,32,32);"
                          "border-radius: 0px;}"
                          "QPushButton:pressed {"
                          "background-color: rgb(128, 128, 128);}"
                          "QPushButton:checked {"
                          "background-color: rgb(128, 255, 128);}"
                          "QPushButton:checked:pressed {"
                          "background-color: rgb(128, 196, 128);}");
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

    verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    vlayout_->addItem(verticalSpacer);

    hlayout_->addLayout(vlayout_);
    /*filtertype*/
    vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);


    compFilterTypeLine = new QLineEdit("0");
    compFilterTypeLine->setStyleSheet("QLineEdit {"
                          "color: rgb(255,255,255);"
                          "background-color: rgb(96, 96, 96);"
                          "border: 2px solid rgb(32,32,32);"
                          "border-radius: 0px;"
                          "}");
    compFilterTypeLine->setAlignment(Qt::AlignCenter);
    compFilterTypeLine->setMaximumWidth(64*dpiRatio);
    compFilterTypeLine->setFont(m_fonts->boldFont8);
    vlayout_->addWidget(compFilterTypeLine);

    sliderslayout = new QHBoxLayout;
    sliderslayout->setContentsMargins(0, 0, 0, 0);
    sliderslayout->setSpacing(0);
    compFilterTypeSlider = new QSlider;
    compFilterTypeSlider->setOrientation(Qt::Orientation::Vertical);
    compFilterTypeSlider->setMaximum(100);
    compFilterTypeSlider->setValue(0);
    compFilterTypeSlider->setPageStep(0);
    sliderslayout->addWidget(compFilterTypeSlider);
    vlayout_->addLayout(sliderslayout);

    label = new QLabel("Type");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setAlignment(Qt::AlignCenter);
    vlayout_->addWidget(label);

    hlayout_->addLayout(vlayout_);
    /*filterfreq*/
    vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(4, 4, 4, 4);
    vlayout_->setSpacing(4);

    compFilterFreqLine = new QLineEdit("0");
    compFilterFreqLine->setStyleSheet("QLineEdit {"
                          "color: rgb(255,255,255);"
                          "background-color: rgb(96, 96, 96);"
                          "border: 2px solid rgb(32,32,32);"
                          "border-radius: 0px;"
                          "}");
    compFilterFreqLine->setAlignment(Qt::AlignCenter);
    compFilterFreqLine->setMaximumWidth(64*dpiRatio);
    compFilterFreqLine->setFont(m_fonts->boldFont8);
    vlayout_->addWidget(compFilterFreqLine);

    sliderslayout = new QHBoxLayout;
    sliderslayout->setContentsMargins(0, 0, 0, 0);
    sliderslayout->setSpacing(0);
    compFilterFreqSlider = new QSlider;
    compFilterFreqSlider->setOrientation(Qt::Orientation::Vertical);
    compFilterFreqSlider->setMaximum(100);
    compFilterFreqSlider->setValue(0);
    compFilterFreqSlider->setPageStep(0);
    sliderslayout->addWidget(compFilterFreqSlider);
    vlayout_->addLayout(sliderslayout);

    label = new QLabel("Freq");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setAlignment(Qt::AlignCenter);
    vlayout_->addWidget(label);

    hlayout_->addLayout(vlayout_);

    vlayout->addLayout(hlayout_);

    hlayout->addLayout(vlayout);

    QFrame *frame = new QFrame;
    frame->setLayout(hlayout);
    frame->setFrameShape(QFrame::Box);
    frame->setFrameShadow(QFrame::Plain);
    frame->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,
                                     QSizePolicy::MinimumExpanding));
    return frame;
}

QFrame *DynamicsWidget::gateWidget(){
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
    frame->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding));
    return frame;
}

void DynamicsWidget::connectSignals(){
    QPushButton::connect(compActivePushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitOnOffChanged);
    QPushButton::connect(compAutoPushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitAutoTimeChanged);
    QPushButton::connect(compFilterPushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitFilterChanged);
    /*QPushButton::connect(compFilterSoloPushButton, &QPushButton::clicked,
                         this, &DynamicsWidget::emitCompFilterSoloChanged);*/
    QComboBox::connect(compFilterSourceComboBox, &QComboBox::currentIndexChanged,
                         this, &DynamicsWidget::emitKeySourceChanged);
    QSlider::connect(tresholdSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitTresholdChanged);
    QSlider::connect(ratioSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitRatioChanged);
    QSlider::connect(mixSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitMixChanged);
    QSlider::connect(gainSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitGainChanged);
    QSlider::connect(kneeSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitKneeChanged);
    QRadioButton::connect(compTypeRadio, &QRadioButton::clicked,
                          this, &DynamicsWidget::emitModeCompExpChanged);
    QRadioButton::connect(expTypeRadio, &QRadioButton::clicked,
                          this, &DynamicsWidget::emitModeCompExpChanged);
    QRadioButton::connect(compLinRadio, &QRadioButton::clicked,
                          this, &DynamicsWidget::emitEnvLinLogChanged);
    QRadioButton::connect(compLogRadio, &QRadioButton::clicked,
                          this, &DynamicsWidget::emitEnvLinLogChanged);
    QRadioButton::connect(compPeakRadio, &QRadioButton::clicked,
                          this, &DynamicsWidget::emitDetPeakRmsChanged);
    QRadioButton::connect(compRmsRadio, &QRadioButton::clicked,
                          this, &DynamicsWidget::emitDetPeakRmsChanged);
    QSlider::connect(compAttackSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitAttackChanged);
    QSlider::connect(compHoldSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitHoldChanged);
    QSlider::connect(compReleaseSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitReleaseChanged);
}

void DynamicsWidget::onOffRecieved(int value){}

void DynamicsWidget::tresholdRecieved(float value){
    QSlider::disconnect(tresholdSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitTresholdChanged);
    tresholdSlider->setValue(value * 120);
    float db = float(tresholdSlider->value()) / 2 - 60.;
    compGain->setTreshold(db);
    tresholdLine->setText(QString::number(db) + " db");
    QSlider::connect(tresholdSlider, &QSlider::valueChanged,
                     this, &DynamicsWidget::emitTresholdChanged);
}

void DynamicsWidget::ratioRecieved(int value){
    QSlider::disconnect(ratioSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitRatioChanged);
    ratioSlider->setValue(value);
    compGain->setRatio(int2ratio(value));
    ratioLine->setText(QString::number(int2ratio(value)));
    QSlider::connect(ratioSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitRatioChanged);
}

void DynamicsWidget::mixRecieved(float value){
    QSlider::disconnect(mixSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitMixChanged);
    mixSlider->setValue(value * 20);
    mixLine->setText(QString::number(value*100) + " %");
    QSlider::connect(mixSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitMixChanged);
}

void DynamicsWidget::gainRecieved(float value){
    QSlider::disconnect(gainSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitGainChanged);
    gainSlider->setValue(value * 48);
    gainLine->setText(QString::number(value * 24) + " db");
    QSlider::connect(gainSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitGainChanged);
}

void DynamicsWidget::kneeRecieved(float value){
    QSlider::disconnect(kneeSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitKneeChanged);
    kneeSlider->setValue(value * 5);
    kneeLine->setText(QString::number(kneeSlider->value()));
    compGain->setKnee(kneeSlider->value());
    QSlider::connect(kneeSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitKneeChanged);
}

void DynamicsWidget::modeCompExpRecieved(int value){
    QRadioButton::disconnect(compTypeRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitModeCompExpChanged);
    QRadioButton::disconnect(expTypeRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitModeCompExpChanged);
    if(value == 1)
        expTypeRadio->setChecked(true);
    else
        compTypeRadio->setChecked(true);
    compGain->setType(value);
    QRadioButton::connect(compTypeRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitModeCompExpChanged);
    QRadioButton::connect(expTypeRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitModeCompExpChanged);
}

void DynamicsWidget::envLinLogRecieved(int value){
    QRadioButton::disconnect(compLinRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitEnvLinLogChanged);
    QRadioButton::disconnect(compLogRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitEnvLinLogChanged);
    if(value == 1)
        compLogRadio->setChecked(true);
    else
        compLinRadio->setChecked(true);
    QRadioButton::connect(compLinRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitEnvLinLogChanged);
    QRadioButton::connect(compLogRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitEnvLinLogChanged);
}

void DynamicsWidget::detPeakRmsRecieved(int value){
    QRadioButton::disconnect(compPeakRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitDetPeakRmsChanged);
    QRadioButton::disconnect(compRmsRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitDetPeakRmsChanged);
    if(value == 1)
        compRmsRadio->setChecked(true);
    else
        compPeakRadio->setChecked(true);
    QRadioButton::connect(compPeakRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitDetPeakRmsChanged);
    QRadioButton::connect(compRmsRadio, &QRadioButton::clicked,
                             this, &DynamicsWidget::emitDetPeakRmsChanged);
}

void DynamicsWidget::attackRecieved(float value){
    QSlider::disconnect(compAttackSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitAttackChanged);
    compAttackSlider->setValue(value * 120);
    compAttackLine->setText(QString::number(compAttackSlider->value()) + " ms");
    QSlider::connect(compAttackSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitAttackChanged);
}

void DynamicsWidget::holdRecieved(float value){
    QSlider::disconnect(compHoldSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitHoldChanged);
    compHoldSlider->setValue(value * 100);
    compHoldLine->setText(QString::number(hold2ms(compHoldSlider->value())) + " ms");
    QSlider::connect(compHoldSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitHoldChanged);
}

void DynamicsWidget::releaseRecieved(float value){
    QSlider::disconnect(compReleaseSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitReleaseChanged);
    compReleaseSlider->setValue(value * 100);
    compReleaseLine->setText(QString::number(release2ms(compReleaseSlider->value())) + " ms");
    QSlider::connect(compReleaseSlider, &QSlider::valueChanged,
                        this, &DynamicsWidget::emitReleaseChanged);
}

void DynamicsWidget::autoTimeRecieved(int value){}

void DynamicsWidget::keySourceRecieved(int value){
    QComboBox::disconnect(compFilterSourceComboBox, &QComboBox::currentIndexChanged,
                         this, &DynamicsWidget::emitKeySourceChanged);
    for(int i=0;i < 65;i++)
        if(compFilterSourceComboBox->itemData(i).toInt() == value)
            compFilterSourceComboBox->setCurrentIndex(i);
    QComboBox::connect(compFilterSourceComboBox, &QComboBox::currentIndexChanged,
                         this, &DynamicsWidget::emitKeySourceChanged);
}

void DynamicsWidget::filterRecieved(int value){}
void DynamicsWidget::filterTypeRecieved(int value){}
void DynamicsWidget::filterFrequencyRecieved(float value){}

void DynamicsWidget::emitOnOffChanged(){}

void DynamicsWidget::emitTresholdChanged(){
    float value = float(tresholdSlider->value()) / 120;
    float db = float(tresholdSlider->value()) / 2 - 60.;
    compGain->setTreshold(db);
    tresholdLine->setText(QString::number(db) + " db");
    emit tresholdChanged(value);
}

void DynamicsWidget::emitRatioChanged(){
    float ratio = int2ratio(ratioSlider->value());
    compGain->setRatio(ratio);
    ratioLine->setText(QString::number(ratio));
    emit ratioChanged(ratioSlider->value());
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

void DynamicsWidget::emitModeCompExpChanged(){
    int value = 0;
    if (expTypeRadio->isChecked()) value = 1;
    compGain->setType(value);
    emit modeCompExpChanged(value);
}

void DynamicsWidget::emitEnvLinLogChanged(){
    int value = 0;
    if (compLogRadio->isChecked()) value = 1;
    emit envLinLogChanged(value);
}

void DynamicsWidget::emitDetPeakRmsChanged(){
    int value = 0;
    if (compRmsRadio->isChecked()) value = 1;
    emit detPeakRmsChanged(value);
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

void DynamicsWidget::emitKeySourceChanged(){
    emit keySourceChanged(compFilterSourceComboBox->currentData().toInt());
}

void DynamicsWidget::emitFilterChanged(){}

void DynamicsWidget::emitFilterTypeChanged(){}

void DynamicsWidget::emitFilterFrequencyChanged(){}

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

compEnvPaint::compEnvPaint(float dpiRatio_) : QWidget() {
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

void compEnvPaint::paintEvent(QPaintEvent *event)
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

void compEnvPaint::paintSquares(QPainter &painter, int height){
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
}

void compEnvPaint::paintLine(QPainter &painter, int height){
    Q_UNUSED(painter)
    Q_UNUSED(height)
}

compFilterPaint::compFilterPaint(float dpiRatio_) : QWidget() {
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

void compFilterPaint::paintEvent(QPaintEvent *event)
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

void compFilterPaint::paintSquares(QPainter &painter, int height){
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
}

void compFilterPaint::paintLine(QPainter &painter, int height){
    Q_UNUSED(painter)
    Q_UNUSED(height)
}
