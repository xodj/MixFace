#include "MixFaceEqualizerWidget.h"
#include "MixFaceStatic.h"
#include "QDebug"

EqualizerWidget::EqualizerWidget(float dpiRatio,
                                 MixFaceFonts *m_fonts_)
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
    sliderStyle = ("QSlider {"
                                     "background-color: rgb(196, 196, 196);"
                                     ""
                                     "height: "+QString::number(28*dpiRatio)+"px;"
                                     "border: 2px solid rgb(32,32,32);"
                                     "border-radius: 5px;}"
                                     "QSlider::handle"
                                     "{"
                                     "background-color:"
                                     "qlineargradient(spread:reflect,"
                                     "x1:0.5, y1:0, x2:1, y2:0,"
                                     "stop:0 rgba(0, 0, 0, 255),"
                                     "stop:0.1 rgba(128, 128, 128, 255));"
                                     "height: "+QString::number(36*dpiRatio)+"px;"
                                     "border: 1px solid rgb(32,32,32);"
                                     "border-radius: 4px;"
                                     "width: "+QString::number(18*dpiRatio)+"px;"
                                     "}"
                                     "QSlider::handle:pressed"
                                     "{"
                                     "background-color:"
                                     "qlineargradient(spread:reflect,"
                                     "x1:0.5, y1:0, x2:1, y2:0,"
                                     "stop:0 rgba(0, 0, 0, 255),"
                                     "stop:0.1 rgba(128, 128, 128, 255));"
                                     "height: "+QString::number(36*dpiRatio)+"px;"
                                     "border: 1px solid rgb(223,223,223);"
                                     "border-radius: 4px;"
                                     "width: "+QString::number(18*dpiRatio)+"px;"
                                     "}"
                                     "QSlider::groove"
                                     "{"
                                     "border-radius: 0px;"
                                     "}");
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
    //Construct
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(0, 0, 0, 0);
    vlayout->setSpacing(0);

    vlayout->addWidget(topFrame());
    vlayout->addWidget(bottomFrame());

    this->setLayout(vlayout);
    this->setStyleSheet("background-color: rgb(96, 96, 96);");

    connectSignals();

    eqTimer = new eqTableTimer;
    eqTimer->addEqTablePaint(eqTable);
    eqTimer->start(30);
}

EqualizerWidget::~EqualizerWidget(){
    disconnectSignals();
    eqTimer->stop();
    delete eqTimer;
}

QFrame *EqualizerWidget::topFrame(){
    QFrame *frame = new QFrame;
    frame->setFrameShape(QFrame::Box);
    frame->setFrameShadow(QFrame::Plain);
    frame->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,
                                     QSizePolicy::MinimumExpanding));

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setContentsMargins(10, 10, 10, 10);
    hlayout->setSpacing(0);

    //Activate Button
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(8, 16, 8, 16);
    vlayout->setSpacing(4);

    eqActivePushButton = new QPushButton("Active");
    eqActivePushButton->setCheckable(true);
    eqActivePushButton->setMinimumSize(96*dpiRatio, 36*dpiRatio);
    eqActivePushButton->setMaximumSize(96*dpiRatio, 36*dpiRatio);
    eqActivePushButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    eqActivePushButton->setStyleSheet("QPushButton {"
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
    eqActivePushButton->setFont(m_fonts->boldFont);
    vlayout->addWidget(eqActivePushButton);

    eqResetPushButton = new QPushButton("Reset");
    eqResetPushButton->setMinimumSize(96*dpiRatio, 24*dpiRatio);
    eqResetPushButton->setMaximumSize(96*dpiRatio, 24*dpiRatio);
    eqResetPushButton->setStyleSheet(buttonStyle);
    eqResetPushButton->setFont(m_fonts->boldFont8);
    vlayout->addWidget(eqResetPushButton);

    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    vlayout->addItem(verticalSpacer);

    eqRtaPushButton = new QPushButton("RTA");
    eqRtaPushButton->setCheckable(true);
    eqRtaPushButton->setMinimumSize(96*dpiRatio, 24*dpiRatio);
    eqRtaPushButton->setMaximumSize(96*dpiRatio, 24*dpiRatio);
    eqRtaPushButton->setStyleSheet(buttonStyle);
    eqRtaPushButton->setFont(m_fonts->boldFont8);
    vlayout->addWidget(eqRtaPushButton);

    hlayout->addLayout(vlayout);

    //PEQ GRAPHICS
    QHBoxLayout *hlayout_ = new QHBoxLayout;
    hlayout_->setContentsMargins(10, 10, 10, 10);
    hlayout_->setSpacing(0);
    eqTable = new eqTablePaint(dpiRatio);
    eqTable->setSizePolicy(QSizePolicy::MinimumExpanding,
                            QSizePolicy::MinimumExpanding);
    hlayout_->addWidget(eqTable);
    hlayout->addLayout(hlayout_);
    //PEQ GRAPHICS

    frame->setLayout(hlayout);
    return frame;
}

QFrame *EqualizerWidget::bottomFrame(){
    QFrame *frame = new QFrame;
    frame->setFrameShape(QFrame::Box);
    frame->setFrameShadow(QFrame::Plain);
    frame->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,
                                     QSizePolicy::MinimumExpanding));

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->setSpacing(0);

    for(int i=0;i<6;i++)
        hlayout->addWidget(bandWidget(i));

    frame->setLayout(hlayout);
    return frame;
}

QWidget *EqualizerWidget::bandWidget(int bandid){
    bandWidgetList[bandid] = new QWidget;
    bandWidgetList[bandid]->setSizePolicy(QSizePolicy::Maximum,
                            QSizePolicy::Minimum);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(4, 4, 4, 4);
    vlayout->setSpacing(4);

    //label
    QLabel *label = new QLabel("Band " + QString::number(bandid + 1));
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setAlignment(Qt::AlignCenter);
    vlayout->addWidget(label);

    //mode+reset
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setContentsMargins(4, 4, 4, 4);
    hlayout->setSpacing(4);

    eqBandTypeComboBox[bandid] = new QComboBox;
    eqBandTypeComboBox[bandid]->setProperty("bandid", bandid);
    eqBandTypeComboBox[bandid]->addItem("LCut");
    eqBandTypeComboBox[bandid]->addItem("LShv");
    eqBandTypeComboBox[bandid]->addItem("PEQ");
    eqBandTypeComboBox[bandid]->addItem("VEQ");
    eqBandTypeComboBox[bandid]->addItem("HShv");
    eqBandTypeComboBox[bandid]->addItem("HCut");
    hlayout->addWidget(eqBandTypeComboBox[bandid]);

    eqBandResetPushButton[bandid] = new QPushButton("Reset");
    eqBandResetPushButton[bandid]->setMinimumSize(64*dpiRatio, 24*dpiRatio);
    eqBandResetPushButton[bandid]->setStyleSheet(buttonStyle);
    eqBandResetPushButton[bandid]->setFont(m_fonts->boldFont8);
    eqBandResetPushButton[bandid]->setProperty("bandid", bandid);
    hlayout->addWidget(eqBandResetPushButton[bandid]);

    vlayout->addLayout(hlayout);

    //gain
    hlayout = new QHBoxLayout;
    hlayout->setContentsMargins(4, 4, 4, 4);
    hlayout->setSpacing(4);

    label = new QLabel("Gain");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont12);
    label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    hlayout->addWidget(label);

    eqBandGainLine[bandid] = new QLineEdit("-15dB");
    eqBandGainLine[bandid]->setStyleSheet(lineStyle);
    eqBandGainLine[bandid]->setAlignment(Qt::AlignCenter);
    eqBandGainLine[bandid]->setMinimumSize(48*dpiRatio, 24*dpiRatio);
    eqBandGainLine[bandid]->setMaximumWidth(48*dpiRatio);
    eqBandGainLine[bandid]->setFont(m_fonts->boldFont8);
    eqBandGainLine[bandid]->setProperty("bandid", bandid);
    hlayout->addWidget(eqBandGainLine[bandid]);

    QSpacerItem *horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);
    hlayout->addItem(horizontalSpacer);

    vlayout->addLayout(hlayout);

    eqBandGainSlider[bandid] = new QSlider;
    eqBandGainSlider[bandid]->setOrientation(Qt::Orientation::Horizontal);
    eqBandGainSlider[bandid]->setMinimumSize(32*dpiRatio, 32*dpiRatio);
    eqBandGainSlider[bandid]->setStyleSheet(sliderStyle);
    eqBandGainSlider[bandid]->setMaximum(120);
    eqBandGainSlider[bandid]->setValue(0);
    eqBandGainSlider[bandid]->setPageStep(0);
    eqBandGainSlider[bandid]->setProperty("bandid", bandid);
    vlayout->addWidget(eqBandGainSlider[bandid]);

    //freq
    hlayout = new QHBoxLayout;
    hlayout->setContentsMargins(4, 4, 4, 4);
    hlayout->setSpacing(4);

    label = new QLabel("Freq");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont12);
    label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    hlayout->addWidget(label);

    eqBandFreqLine[bandid] = new QLineEdit("20.0Hz");
    eqBandFreqLine[bandid]->setStyleSheet(lineStyle);
    eqBandFreqLine[bandid]->setAlignment(Qt::AlignCenter);
    eqBandFreqLine[bandid]->setMinimumSize(48*dpiRatio, 24*dpiRatio);
    eqBandFreqLine[bandid]->setMaximumWidth(48*dpiRatio);
    eqBandFreqLine[bandid]->setFont(m_fonts->boldFont8);
    eqBandFreqLine[bandid]->setProperty("bandid", bandid);
    hlayout->addWidget(eqBandFreqLine[bandid]);

    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);
    hlayout->addItem(horizontalSpacer);

    vlayout->addLayout(hlayout);

    eqBandFreqSlider[bandid] = new QSlider;
    eqBandFreqSlider[bandid]->setOrientation(Qt::Orientation::Horizontal);
    eqBandFreqSlider[bandid]->setMinimumSize(32*dpiRatio, 32*dpiRatio);
    eqBandFreqSlider[bandid]->setStyleSheet(sliderStyle);
    eqBandFreqSlider[bandid]->setMaximum(200);
    eqBandFreqSlider[bandid]->setValue(0);
    eqBandFreqSlider[bandid]->setPageStep(0);
    eqBandFreqSlider[bandid]->setProperty("bandid", bandid);
    vlayout->addWidget(eqBandFreqSlider[bandid]);

    //qual
    hlayout = new QHBoxLayout;
    hlayout->setContentsMargins(4, 4, 4, 4);
    hlayout->setSpacing(4);

    label = new QLabel("Qual");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont12);
    label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    hlayout->addWidget(label);

    eqBandQualLine[bandid] = new QLineEdit("10");
    eqBandQualLine[bandid]->setStyleSheet(lineStyle);
    eqBandQualLine[bandid]->setAlignment(Qt::AlignCenter);
    eqBandQualLine[bandid]->setMinimumSize(48*dpiRatio, 24*dpiRatio);
    eqBandQualLine[bandid]->setMaximumWidth(48*dpiRatio);
    eqBandQualLine[bandid]->setFont(m_fonts->boldFont8);
    eqBandQualLine[bandid]->setProperty("bandid", bandid);
    hlayout->addWidget(eqBandQualLine[bandid]);

    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);
    hlayout->addItem(horizontalSpacer);

    vlayout->addLayout(hlayout);

    eqBandQualSlider[bandid] = new QSlider;
    eqBandQualSlider[bandid]->setOrientation(Qt::Orientation::Horizontal);
    eqBandQualSlider[bandid]->setMinimumSize(32*dpiRatio, 32*dpiRatio);
    eqBandQualSlider[bandid]->setStyleSheet(sliderStyle);
    eqBandQualSlider[bandid]->setMaximum(71);
    eqBandQualSlider[bandid]->setValue(0);
    eqBandQualSlider[bandid]->setPageStep(0);
    eqBandQualSlider[bandid]->setProperty("bandid", bandid);
    vlayout->addWidget(eqBandQualSlider[bandid]);

    //end

    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    vlayout->addItem(verticalSpacer);

    bandWidgetList[bandid]->setLayout(vlayout);

    return bandWidgetList[bandid];
}

void EqualizerWidget::connectSignals(){
    QPushButton::connect(eqActivePushButton, &QPushButton::clicked,
                     this, &EqualizerWidget::emitEqOnOffClicked);
    for(int idy=0;idy<6;idy++){
        QComboBox::connect(eqBandTypeComboBox[idy], &QComboBox::currentIndexChanged,
                         this, &EqualizerWidget::emitEqBandTypeChanged);
        QPushButton::connect(eqBandResetPushButton[idy], &QPushButton::clicked,
                         this, &EqualizerWidget::emitEqBandResetClicked);
        QSlider::connect(eqBandGainSlider[idy], &QSlider::valueChanged,
                         this, &EqualizerWidget::emitEqBandGainChanged);
        QSlider::connect(eqBandFreqSlider[idy], &QSlider::valueChanged,
                         this, &EqualizerWidget::emitEqBandFreqChanged);
        QSlider::connect(eqBandQualSlider[idy], &QSlider::valueChanged,
                         this, &EqualizerWidget::emitEqBandQualChanged);
    }
}

void EqualizerWidget::disconnectSignals(){
    QPushButton::disconnect(eqActivePushButton, &QPushButton::clicked,
                     this, &EqualizerWidget::emitEqOnOffClicked);
    for(int idy=0;idy<6;idy++){
        QComboBox::disconnect(eqBandTypeComboBox[idy], &QComboBox::currentIndexChanged,
                         this, &EqualizerWidget::emitEqBandTypeChanged);
        QPushButton::disconnect(eqBandResetPushButton[idy], &QPushButton::clicked,
                         this, &EqualizerWidget::emitEqBandResetClicked);
        QSlider::disconnect(eqBandGainSlider[idy], &QSlider::valueChanged,
                         this, &EqualizerWidget::emitEqBandGainChanged);
        QSlider::disconnect(eqBandFreqSlider[idy], &QSlider::valueChanged,
                         this, &EqualizerWidget::emitEqBandFreqChanged);
        QSlider::disconnect(eqBandQualSlider[idy], &QSlider::valueChanged,
                         this, &EqualizerWidget::emitEqBandQualChanged);
    }
}

void EqualizerWidget::setIdx(int idx_){
    idx = idx_;
    if(idx > 39){
        //set 6 bands for fx, bus, mtx, lr and mc
        bandWidgetList[4]->setHidden(false);
        bandWidgetList[5]->setHidden(false);
        eqBandTypeComboBox[0]->clear();
        eqBandTypeComboBox[0]->addItem("LCut");
        eqBandTypeComboBox[0]->addItem("LShv");
        eqBandTypeComboBox[0]->addItem("PEQ");
        eqBandTypeComboBox[0]->addItem("VEQ");
        eqBandTypeComboBox[0]->addItem("HShv");
        eqBandTypeComboBox[0]->addItem("HCut");
        eqBandTypeComboBox[5]->clear();
        eqBandTypeComboBox[5]->addItem("LCut");
        eqBandTypeComboBox[5]->addItem("LShv");
        eqBandTypeComboBox[5]->addItem("PEQ");
        eqBandTypeComboBox[5]->addItem("VEQ");
        eqBandTypeComboBox[5]->addItem("HShv");
        eqBandTypeComboBox[5]->addItem("HCut");
        if(idx > 63){
            //add special bands for mtx, lr and mc
            eqBandTypeComboBox[0]->addItem("BU6");
            eqBandTypeComboBox[0]->addItem("BU12");
            eqBandTypeComboBox[0]->addItem("BS12");
            eqBandTypeComboBox[0]->addItem("LR12");
            eqBandTypeComboBox[0]->addItem("BU18");
            eqBandTypeComboBox[0]->addItem("BU24");
            eqBandTypeComboBox[0]->addItem("BS24");
            eqBandTypeComboBox[0]->addItem("LR24");
            eqBandTypeComboBox[5]->addItem("BU6");
            eqBandTypeComboBox[5]->addItem("BU12");
            eqBandTypeComboBox[5]->addItem("BS12");
            eqBandTypeComboBox[5]->addItem("LR12");
            eqBandTypeComboBox[5]->addItem("BU18");
            eqBandTypeComboBox[5]->addItem("BU24");
            eqBandTypeComboBox[5]->addItem("BS24");
            eqBandTypeComboBox[5]->addItem("LR24");
        }
    } else {
        //set 4 bands for ch and aux in
        bandWidgetList[4]->setHidden(true);
        bandWidgetList[5]->setHidden(true);
        eqBandTypeComboBox[0]->clear();
        eqBandTypeComboBox[0]->addItem("LCut");
        eqBandTypeComboBox[0]->addItem("LShv");
        eqBandTypeComboBox[0]->addItem("PEQ");
        eqBandTypeComboBox[0]->addItem("VEQ");
        eqBandTypeComboBox[0]->addItem("HShv");
        eqBandTypeComboBox[0]->addItem("HCut");
    }
};

void EqualizerWidget::eqOnOffRecieved(int state){
    QPushButton::disconnect(eqActivePushButton, &QPushButton::clicked,
                     this, &EqualizerWidget::emitEqOnOffClicked);
    eqActivePushButton->setChecked(state);
    //message to graphics
    QPushButton::connect(eqActivePushButton, &QPushButton::clicked,
                     this, &EqualizerWidget::emitEqOnOffClicked);
}

void EqualizerWidget::eqBandTypeRecieved(int bandId, int type){
    QComboBox::disconnect(eqBandTypeComboBox[bandId], &QComboBox::currentIndexChanged,
                     this, &EqualizerWidget::emitEqBandTypeChanged);
    eqBandTypeComboBox[bandId]->setCurrentIndex(type);
    //back to work!
    //message to graphics
    QComboBox::connect(eqBandTypeComboBox[bandId], &QComboBox::currentIndexChanged,
                     this, &EqualizerWidget::emitEqBandTypeChanged);
}

void EqualizerWidget::eqBandGainRecieved(int bandId, float gain){
    QSlider::disconnect(eqBandGainSlider[bandId], &QSlider::valueChanged,
                     this, &EqualizerWidget::emitEqBandGainChanged);
    eqBandGainSlider[bandId]->setValue(int(gain * 120.f));
    eqBandGainLine[bandId]->setText(QString::number(
                                     round2((gain - 0.5f) * 30, 2))
                                 + "dB");
    //message to graphics
    QSlider::connect(eqBandGainSlider[bandId], &QSlider::valueChanged,
                     this, &EqualizerWidget::emitEqBandGainChanged);
}

void EqualizerWidget::eqBandFreqRecieved(int bandId, float freq){
    QSlider::disconnect(eqBandFreqSlider[bandId], &QSlider::valueChanged,
                     this, &EqualizerWidget::emitEqBandFreqChanged);
    eqBandFreqSlider[bandId]->setValue(int(freq * 200.f));
    eqBandFreqLine[bandId]->setText(QString::fromStdString(
                                        c201logScaleToFrequencyString(freq)
                                        ));
    //message to graphics
    QSlider::connect(eqBandFreqSlider[bandId], &QSlider::valueChanged,
                     this, &EqualizerWidget::emitEqBandFreqChanged);
}

void EqualizerWidget::eqBandQualRecieved(int bandId, float qual){
    QSlider::disconnect(eqBandQualSlider[bandId], &QSlider::valueChanged,
                     this, &EqualizerWidget::emitEqBandQualChanged);
    eqBandQualSlider[bandId]->setValue(int(qual * 71.f));
    eqBandQualLine[bandId]->setText(QString::number(
                                        qualFactorValueToFactor(qual)
                                        ));
    //message to graphics
    QSlider::connect(eqBandQualSlider[bandId], &QSlider::valueChanged,
                     this, &EqualizerWidget::emitEqBandQualChanged);
}

void EqualizerWidget::emitEqOnOffClicked(){
    emit eqOnOffChanged(eqActivePushButton->isChecked());
    //message to graphics
}

void EqualizerWidget::emitEqResetClicked(){
    //back to work!
}

void EqualizerWidget::emitEqRtaClicked(){
    //back to work!
}

void EqualizerWidget::emitEqBandTypeChanged(){
    QComboBox *eqTypeComboBox = reinterpret_cast<QComboBox*>(sender());
    int bandid = eqTypeComboBox->property("bandid").toInt();
    int type = eqTypeComboBox->currentIndex();
    emit eqBandTypeChanged(bandid, type);
    //back to work!
}

void EqualizerWidget::emitEqBandResetClicked(){
    int bandid = reinterpret_cast<QSlider*>(sender())->property("bandid").toInt();
    //back to work!
}

void EqualizerWidget::emitEqBandGainChanged(){
    QSlider *eqGainSlider = reinterpret_cast<QSlider*>(sender());
    int bandid = eqGainSlider->property("bandid").toInt();
    float value = float(eqGainSlider->value()) / 120.f;
    eqBandGainLine[bandid]->setText(QString::number(
                                     round2((value - 0.5f) * 30, 2))
                                 + "dB");
    eqTable->setBandGain(bandid, value);
    emit eqBandGainChanged(bandid, value);
}

void EqualizerWidget::emitEqBandFreqChanged(){
    QSlider *eqFreqSlider = reinterpret_cast<QSlider*>(sender());
    int bandid = eqFreqSlider->property("bandid").toInt();
    float value = float(eqFreqSlider->value()) / 200.f;
    eqBandFreqLine[bandid]->setText(QString::fromStdString(
                                    c201logScaleToFrequencyString(value)
                                    ));
    eqTable->setBandFreq(bandid, value);
    emit eqBandFreqChanged(bandid, value);
}

void EqualizerWidget::emitEqBandQualChanged(){
    QSlider *eqQualSlider = reinterpret_cast<QSlider*>(sender());
    int bandid = eqQualSlider->property("bandid").toInt();
    float value = float(eqQualSlider->value()) / 71.f;
    eqBandQualLine[bandid]->setText(QString::number(
                                    qualFactorValueToFactor(value)
                                    ));
    eqTable->setBandQual(bandid, qualFactorValueToFactor(value));
    emit eqBandQualChanged(bandid, value);
}

/**/
/*PEQ GRAPHICS*/
/**/

eqTablePaint::eqTablePaint(float dpiRatio_) : QWidget() {
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

void eqTablePaint::paintEvent(QPaintEvent *event)
{
    const QRect rect = event->region().boundingRect();
    QSize paintCacheSize;
    int x;
    int y;
    if(rect.width() > rect.height() * 3) {
        paintCacheSize = QSize(rect.height() * 3, rect.height());
        x = (rect.width() / 2) - (rect.height() * 1.5);
        y = 0;
    } else {
        paintCacheSize = QSize(rect.width(), rect.width() / 3);
        x = 0;
        y = (rect.height() / 2) - (rect.width() / 6);
    }

    if (squaresCache == nullptr || squaresCache->size() != paintCacheSize) {
        delete squaresCache;
        squaresCache = new QPixmap(paintCacheSize);

        QColor clearColor(0, 0, 0, 0);
        squaresCache->fill(clearColor);

        QPainter painter(squaresCache);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.translate(0, 0);
        painter.scale(1, 1);
        paintSquares(painter, paintCacheSize);
        painter.end();
    }

    if (lineCache == nullptr || lineCache->size() != paintCacheSize || repaint) {
        delete lineCache;
        lineCache = new QPixmap(paintCacheSize);

        QColor clearColor(0, 0, 0, 0);
        lineCache->fill(clearColor);

        QPainter painter(lineCache);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.translate(0, 0);
        painter.scale(1, 1);
        paintLine(painter, paintCacheSize);
        painter.end();
        repaint = false;
    }

    QPainter painter(this);

    painter.translate(0, rect.height());
    painter.scale(1, -1);
    painter.drawPixmap(x, y, *squaresCache);
    painter.drawPixmap(x, y, *lineCache);
}

void eqTablePaint::paintSquares(QPainter &painter, QSize paintCacheSize){
    painter.setPen(backColor);
    painter.fillRect(0, 0, paintCacheSize.width(), paintCacheSize.height(), backColor);

    painter.setPen(lineColor);
    painter.drawRect(0, 0, paintCacheSize.width(), paintCacheSize.height());
    painter.drawRect(1, 1, paintCacheSize.width() - 2, paintCacheSize.height() - 2);

    painter.setPen(squaresColor);
    qreal scale = paintCacheSize.height() / qreal(8);
    for (int i = 1;i < 8; i++){
        painter.drawLine(2, i * scale, paintCacheSize.width() - 2, i * scale);
    }
    painter.drawLine(paintCacheSize.width() * 0.0600f, 2, paintCacheSize.width() * 0.0600f, paintCacheSize.height() - 2);
    painter.drawLine(paintCacheSize.width() * 0.1000f, 2, paintCacheSize.width() * 0.1000f, paintCacheSize.height() - 2);
    painter.drawLine(paintCacheSize.width() * 0.1325f, 2, paintCacheSize.width() * 0.1325f, paintCacheSize.height() - 2);
    painter.drawLine(paintCacheSize.width() * 0.1600f, 2, paintCacheSize.width() * 0.1600f, paintCacheSize.height() - 2);
    painter.drawLine(paintCacheSize.width() * 0.1820f, 2, paintCacheSize.width() * 0.1820f, paintCacheSize.height() - 2);
    painter.drawLine(paintCacheSize.width() * 0.2000f, 2, paintCacheSize.width() * 0.2000f, paintCacheSize.height() - 2);
    painter.drawLine(paintCacheSize.width() * 0.2175f, 2, paintCacheSize.width() * 0.2175f, paintCacheSize.height() - 2);
    painter.drawLine(paintCacheSize.width() * 0.2335f, 2, paintCacheSize.width() * 0.2335f, paintCacheSize.height() - 2);
    painter.drawLine(paintCacheSize.width() * 0.3335f, 2, paintCacheSize.width() * 0.3335f, paintCacheSize.height() - 2);
    painter.drawLine(paintCacheSize.width() * 0.3925f, 2, paintCacheSize.width() * 0.3925f, paintCacheSize.height() - 2);
    painter.drawLine(paintCacheSize.width() * 0.4340f, 2, paintCacheSize.width() * 0.4340f, paintCacheSize.height() - 2);
    painter.drawLine(paintCacheSize.width() * 0.4675f, 2, paintCacheSize.width() * 0.4675f, paintCacheSize.height() - 2);
    painter.drawLine(paintCacheSize.width() * 0.4925f, 2, paintCacheSize.width() * 0.4925f, paintCacheSize.height() - 2);
    painter.drawLine(paintCacheSize.width() * 0.5150f, 2, paintCacheSize.width() * 0.5150f, paintCacheSize.height() - 2);
    painter.drawLine(paintCacheSize.width() * 0.5340f, 2, paintCacheSize.width() * 0.5340f, paintCacheSize.height() - 2);
    painter.drawLine(paintCacheSize.width() * 0.5500f, 2, paintCacheSize.width() * 0.5500f, paintCacheSize.height() - 2);
    painter.drawLine(paintCacheSize.width() * 0.5665f, 2, paintCacheSize.width() * 0.5665f, paintCacheSize.height() - 2);
    painter.drawLine(paintCacheSize.width() * 0.6665f, 2, paintCacheSize.width() * 0.6665f, paintCacheSize.height() - 2);
    painter.drawLine(paintCacheSize.width() * 0.7250f, 2, paintCacheSize.width() * 0.7250f, paintCacheSize.height() - 2);
    painter.drawLine(paintCacheSize.width() * 0.7660f, 2, paintCacheSize.width() * 0.7660f, paintCacheSize.height() - 2);
    painter.drawLine(paintCacheSize.width() * 0.8000f, 2, paintCacheSize.width() * 0.8000f, paintCacheSize.height() - 2);
    painter.drawLine(paintCacheSize.width() * 0.8250f, 2, paintCacheSize.width() * 0.8250f, paintCacheSize.height() - 2);
    painter.drawLine(paintCacheSize.width() * 0.8490f, 2, paintCacheSize.width() * 0.8490f, paintCacheSize.height() - 2);
    painter.drawLine(paintCacheSize.width() * 0.8680f, 2, paintCacheSize.width() * 0.8680f, paintCacheSize.height() - 2);
    painter.drawLine(paintCacheSize.width() * 0.8850f, 2, paintCacheSize.width() * 0.8850f, paintCacheSize.height() - 2);
    painter.drawLine(paintCacheSize.width() * 0.9000f, 2, paintCacheSize.width() * 0.9000f, paintCacheSize.height() - 2);
}

void eqTablePaint::paintLine(QPainter &painter, QSize paintCacheSize){
    for(int i=0;i<fftsize;i++){
        eqspline[i] = 0.5f;
    }

    for(int i=0;i<6;i++){
        float neweqspline[fftsize];
        for(int x=0;x<fftsize;x++){
            neweqspline[x] = 0.f;
        }

        if(eqgain[i] < 0.f)
            eqqual[i] = -eqqual[i];

        for(int x=0;x<fftsize;x++){
            neweqspline[x] = 0.f;
        }

        neweqspline[eqfreq[i]] = neweqspline[eqfreq[i]] + eqgain[i];


        for(int x=0;x<fftsize;x++){
            eqspline[x] = eqspline[x] + neweqspline[x];
        }
    }

    qreal freqScale = paintCacheSize.width() / qreal(fftsize);
    qreal gainScale = paintCacheSize.height() / 8 * 6;
    qreal gainScaleOver = paintCacheSize.height() / 8;
    painter.setPen(linePen);
    QPainterPath painterpath(QPointF(0, gainScale * eqspline[0] + gainScaleOver));
    for(int i=0;i<fftsize;i++){
        painterpath.lineTo(QPointF(i * freqScale, gainScale * eqspline[i] + gainScaleOver));
    }
    painterpath.lineTo(QPointF(paintCacheSize.width(), gainScale * eqspline[fftsize - 1] + gainScaleOver));

    painter.drawPath(painterpath);

    QTransform transform = painter.transform();
    transform.scale(2.5, 2.);
    painter.setTransform(transform);
}
