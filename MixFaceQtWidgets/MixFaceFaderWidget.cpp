#include "MixFaceFaderWidget.h"
#include <math.h>
#include <qglobal.h>
#include <QtCore/QVariant>
#include "MixFaceStaticMath.h"

FaderWidget::FaderWidget(float dpiRatio_, DebugLibrary *debug_, MixFaceFonts *m_fonts_, QMainWindow *mainWindow_)
{
    dpiRatio = dpiRatio_;
    debug = debug_;
    m_fonts = m_fonts_;
    mainWindow = mainWindow_;

    initWidget();
    connectWidgets();
}

void FaderWidget::initWidget(){
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(2, 2, 2, 2);
    vlayout->setSpacing(6);

    src = new QPushButton("SRC");
    src->setMinimumSize(96*dpiRatio, 36*dpiRatio);
    src->setStyleSheet("QPushButton {"
                       "color: rgb(0,0,0);"
                       "background-color: rgba(196, 196, 196, 255);"
                       "border: 1px solid rgb(32,32,32);"
                       "border-radius: 0px;}"
                       "QPushButton:pressed {"
                       "background-color: rgba(128, 128, 128, 255);}");
    src->setFont(m_fonts->boldFont12);
    vlayout->addWidget(src);

    eq = new QPushButton("EQ");
    eq->setMinimumSize(96*dpiRatio, 36*dpiRatio);
    eq->setStyleSheet("QPushButton {"
                      "color: rgb(0,0,0);"
                      "background-color: rgba(196, 196, 196, 255);"
                      "border: 1px solid rgb(32,32,32);"
                      "border-radius: 0px;}"
                      "QPushButton:pressed {"
                      "background-color: rgba(128, 128, 128, 255);}");
    eq->setFont(m_fonts->boldFont12);
    vlayout->addWidget(eq);

    dyn = new QPushButton;
    dyn->setMinimumSize(96*dpiRatio, 36*dpiRatio);
    dyn->setStyleSheet("QPushButton {"
                       "color: rgb(0,0,0);"
                       "background-color: rgb(128, 128, 128);"
                       "border: 1px solid rgb(32,32,32);"
                       "border-radius: 0px;}"
                       "QPushButton:pressed {"
                       "background-color: rgb(64, 64, 64);}");
    dyn->setFont(m_fonts->boldFont12);
    vlayout->addWidget(dyn);

    m_dmeter = new dynMeter(dpiRatio, dyn);

    mute = new QPushButton("MUTE");
    mute->setMinimumSize(96*dpiRatio, 32*dpiRatio);
    mute->setCheckable(true);
    mute->setStyleSheet("QPushButton {"
                        "color: rgb(0,0,0);"
                        "background-color: rgb(196, 196, 196);"
                        "border: 2px solid rgb(32,32,32);"
                        "border-radius: 0px;}"
                        "QPushButton:pressed {"
                        "background-color: rgb(128, 128, 128);}"
                        "QPushButton:checked {"
                        "background-color: rgb(222, 64, 64);}");
    mute->setFont(m_fonts->boldFont12);
    vlayout->addWidget(mute);

    panSlider = new QSlider(Qt::Orientation(Qt::Horizontal));
    panSlider->setMaximum(10000);
    panSlider->setValue(5000);
    panSlider->setStyleSheet("QSlider {"
                             "background-color: "
                             "qconicalgradient(cx:0.5, cy:0.5, angle:45, stop:0 rgba(0, 0, 0, 255),"
                             "stop:0.124538 rgba(64, 64, 64, 255),"
                             "stop:0.26 rgba(0, 0, 0, 255),"
                             "stop:0.27 rgba(192, 192, 192, 255),"
                             "stop:0.349842 rgba(255, 255, 255, 255),"
                             "stop:0.39 rgba(255, 255, 255, 255),"
                             "stop:0.48 rgba(192, 192, 192, 255),"
                             "stop:0.49 rgba(0, 0, 0, 255),"
                             "stop:0.624726 rgba(64, 64, 64, 255),"
                             "stop:0.76 rgba(0, 0, 0, 255),"
                             "stop:0.77 rgba(192, 192, 192, 255),"
                             "stop:0.850369 rgba(255, 255, 255, 255),"
                             "stop:0.9 rgba(255, 255, 255, 255),"
                             "stop:0.98 rgba(192, 192, 192, 255),"
                             "stop:0.99 rgba(0, 0, 0, 255));"
                             ""
                             "height: "+QString::number(28*dpiRatio)+"px;"
                             "border: 2px solid rgb(32,32,32);"
                             "border-radius: 5px;}"
                             "QSlider::handle:horizontal"
                             "{"
                             "background-color:"
                             "qlineargradient(spread:reflect,"
                             "x1:0.5, y1:0, x2:1, y2:0,"
                             "stop:0 rgba(0, 0, 0, 255),"
                             "stop:0.1 rgba(196, 196, 196, 255));"
                             "height: "+QString::number(36*dpiRatio)+"px;"
                             "border: 2px solid rgb(32,32,32);"
                             "border-radius: 3px;"
                             "width: "+QString::number(18*dpiRatio)+"px;"
                             "}"
                             "QSlider::groove:horizontal"
                             "{"
                             "border-radius: 0px;"
                             "}");
    vlayout->addWidget(panSlider);

    db = new QLineEdit("0 db");
    db->setStyleSheet("QLineEdit {"
                          "color: rgb(255,255,255);"
                          "background-color: rgb(96, 96, 96);"
                          "border: 2px solid rgb(32,32,32);"
                          "border-radius: 0px;"
                          "}");
    db->setAlignment(Qt::AlignCenter);
    db->setFont(m_fonts->boldFont12);
    vlayout->addWidget(db);

    volArea = new QWidget;
    volArea->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);

    m_vmeter = new volMeter;
    m_vmeter->setParent(volArea);

    ticks = new TicksPaint(dpiRatio);
    ticks->setParent(volArea);

    volSlider = new QSlider(volArea);
    volSlider->blockSignals(false);
    volSlider->setOrientation(Qt::Orientation(Qt::Vertical));
    volSlider->setMinimumWidth(44*dpiRatio);
    volSlider->setMaximumWidth(44*dpiRatio);
    volSlider->setMaximum(10000);
    volSlider->setValue(7500);
    volSlider->setPageStep(0);
    volSlider->setStyleSheet(QString::fromUtf8("QSlider {"
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
                                                 "margin-top:22px;\n"
                                                 "}\n"
                                                 "\n"
                                                 "QSlider::add-page\n"
                                                 "{\n"
                                                 "background: rgb(96, 196, 96);\n"
                                                 "border: 1px solid rgb(0,0,0);\n"
                                                 "border-radius: 4px;\n"
                                                 "margin-bottom: 22px;\n"
                                                 "}\n"
                                                 "\n"
                                                 "QSlider::handle\n"
                                                 "{\n"
                                                 "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(196, 196, 196, 255), stop:0.12 rgba(64, 64, 64, 255), stop:0.480377 rgba(255, 255, 255, 255), stop:0.489564 rgba(0, 0, 0, 255), stop:0.510436 rgba(0, 0, 0, 255), stop:0.52 rgba(255, 255, 255, 255), stop:0.88 rgba(64, 64, 64, 255), stop:1 rgba(196, 196, 196, 255));\n"
                                                 "border: 1px solid rgb(64,64,64);\n"
                                                 "border-radius: 5px;\n"
                                                 "height: 44px;\n"
                                                 "margin: 0 -18px;\n"
                                                 "}"));

    vlayout->addWidget(volArea);

    solo = new QPushButton("SOLO");
    solo->setMinimumSize(96*dpiRatio, 32*dpiRatio);
    solo->setCheckable(true);
    solo->setStyleSheet("QPushButton {"
                        "color: rgb(0,0,0);"
                        "background-color: rgb(196, 196, 196);"
                        "border: 2px solid rgb(32,32,32);"
                        "border-radius: 0px;}"
                        "QPushButton:pressed {"
                        "background-color: rgb(128, 128, 128);}"
                        "QPushButton:checked {"
                        "background-color: rgb(255, 196, 96);}");
    solo->setFont(m_fonts->boldFont12);
    vlayout->addWidget(solo);

    QVBoxLayout *botLayout = new QVBoxLayout;
    botLayout->setContentsMargins(0, 4*dpiRatio, 0, 0);
    botLayout->setSpacing(0);

    channelName = new QPushButton();
    channelName->setStyleSheet("QPushButton {"
                               "background-color: rgb(64, 64, 64);"
                               "color: rgb(255,255,255);"
                               "border: 0px solid rgb(0,0,0);}");
    channelName->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    channelName->setFont(m_fonts->boldFont12);
    botLayout->addWidget(channelName);

    icon = new QPushButton;
    icon->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    botLayout->addWidget(icon);
    icon->setHidden(true);

    QScrollArea *botArea = new QScrollArea;
    botArea->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);
    botArea->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    botArea->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    botArea->setWidgetResizable(true);
    botArea->setMinimumHeight(96*dpiRatio);
    botArea->setMaximumHeight(96*dpiRatio);
    botArea->setAlignment(Qt::AlignCenter);
    botArea->setStyleSheet("QScrollArea {"
                           "border: 0px solid rgb(0, 0, 0);"
                           "background-color: rgb(64, 64, 64);"
                           "}");
    botArea->setLayout(botLayout);
    vlayout->addWidget(botArea);

    nativeName = new QLabel();
    nativeName->setStyleSheet("QLabel {background-color: rgba(0, 0, 0, 64);"
                              "color: rgba(255, 255, 255, 196);}");
    nativeName->setFont(m_fonts->boldFont8);
    nativeName->setAlignment(Qt::AlignHCenter);
    vlayout->addWidget(nativeName);

    QFrame *frame = new QFrame;
    frame->setLayout(vlayout);
    frame->setFrameShape(QFrame::Box);
    frame->setFrameShadow(QFrame::Plain);

    QVBoxLayout *vlayout_ = new QVBoxLayout();
    vlayout_->setContentsMargins(0, 0, 0, 0);
    vlayout_->setSpacing(0);
    vlayout_->addWidget(frame);

    this->setLayout(vlayout_);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    this->setStyleSheet("QWidget {background-color: rgba(96, 96, 96,255);}");
    this->setMinimumWidth(128*dpiRatio);
    this->setMaximumWidth(128*dpiRatio);

    int heightV = volArea->geometry().height() - 20;
    volSlider->setGeometry(QRect(42*dpiRatio,10,10*dpiRatio,heightV));
    m_vmeter->setGeometry(QRect(16*dpiRatio,10,10*dpiRatio,heightV));
    ticks->setGeometry(QRect(68*dpiRatio,10,10*dpiRatio,heightV));

    metersRenew = new MetersTimer(m_vmeter,m_dmeter);
    metersRenew->start(30);
}

void FaderWidget::connectWidgets(){
    QPushButton::connect(src, &QPushButton::clicked, this, &FaderWidget::srcClicked);
    QPushButton::connect(eq, &QPushButton::clicked, this, &FaderWidget::eqClicked);
    QPushButton::connect(dyn, &QPushButton::clicked, this, &FaderWidget::dynClicked);
    QPushButton::connect(mute, &QPushButton::clicked, this, &FaderWidget::emitMuteChanged);
    QSlider::connect(panSlider, &QSlider::valueChanged, this, &FaderWidget::panChanged);
    QLineEdit::connect(db,&QLineEdit::editingFinished,this,&FaderWidget::dbEditingFinished);
    QSlider::connect(volSlider, &QSlider::valueChanged, this, &FaderWidget::emitFaderChanged);
    QPushButton::connect(solo, &QPushButton::clicked, this, &FaderWidget::emitSoloChanged);
    QPushButton::connect(channelName,&QPushButton::clicked,this,&FaderWidget::iconButtonClicked);
    QPushButton::connect(icon,&QPushButton::clicked,this,&FaderWidget::iconButtonClicked);
}

FaderWidget::~FaderWidget(){
    QPushButton::disconnect(src, &QPushButton::clicked, this, &FaderWidget::srcClicked);
    QPushButton::disconnect(eq, &QPushButton::clicked, this, &FaderWidget::eqClicked);
    QPushButton::disconnect(dyn, &QPushButton::clicked, this, &FaderWidget::dynClicked);
    QPushButton::disconnect(mute, &QPushButton::clicked, this, &FaderWidget::emitMuteChanged);
    QSlider::disconnect(panSlider, &QSlider::valueChanged, this, &FaderWidget::panChanged);
    QLineEdit::disconnect(db,&QLineEdit::editingFinished,this,&FaderWidget::dbEditingFinished);
    QSlider::disconnect(volSlider, &QSlider::valueChanged, this, &FaderWidget::emitFaderChanged);
    QPushButton::disconnect(solo, &QPushButton::clicked, this, &FaderWidget::emitSoloChanged);
    QPushButton::disconnect(channelName,&QPushButton::clicked,this,&FaderWidget::iconButtonClicked);
    QPushButton::disconnect(icon,&QPushButton::clicked,this,&FaderWidget::iconButtonClicked);

    metersRenew->stop();
    delete metersRenew;
}

void FaderWidget::resizeEvent(QResizeEvent *e){
    QWidget::resizeEvent(e);
    int heightV = volArea->geometry().height() - 20;
    volSlider->setGeometry(QRect(42*dpiRatio,10,10*dpiRatio,heightV));
    m_vmeter->setGeometry(QRect(16*dpiRatio,10,10*dpiRatio,heightV));
    ticks->setGeometry(QRect(68*dpiRatio,10,10*dpiRatio,heightV));
}

void FaderWidget::setFaderType(FaderType ftype) {
    //lock fader features
    switch(ftype){
    case f_channel:
        setColor(0);
        break;
    case f_auxin:
        setColor(1);
        setLogo(55);
        dyn->setDisabled(true);
        dyn->setStyleSheet("QPushButton {"
                           "color: rgba(0,0,0,0);"
                           "background-color: rgba(0, 0, 0, 0);"
                           "border: 0px solid rgb(0,0,0);"
                           "border-radius: 0px;}");
        break;
    case f_fxreturn:
        setColor(3);
        setLogo(61);
        dyn->setDisabled(true);
        dyn->setStyleSheet("QPushButton {"
                           "color: rgba(0,0,0,0);"
                           "background-color: rgba(0, 0, 0, 0);"
                           "border: 0px solid rgb(0,0,0);"
                           "border-radius: 0px;}");
        break;
    case f_bus:
        setColor(4);
        setLogo(71);
        break;
    case f_matrix:
        setColor(5);
        setLogo(72);
        panSlider->setDisabled(true);
        panSlider->setStyleSheet("QSlider {"
                                 "background-color: rgba(0, 0, 0, 0);"
                                 "height: "+QString::number(28*dpiRatio)+"px;"
                                 "border: 2px solid rgba(0,0,0,0);"
                                 "border-radius: 0px;}"
                                 "QSlider::handle:horizontal"
                                 "{"
                                 "background-color: rgba(0, 0, 0, 0);"
                                 "height: "+QString::number(36*dpiRatio)+"px;"
                                 "border: 0px solid rgb(0,0,0);"
                                 "border-radius: 0px;"
                                 "width: "+QString::number(16*dpiRatio)+"px;"
                                 "}"
                                 "QSlider::groove:horizontal"
                                 "{"
                                 "border-radius: 0px;"
                                 "}");
        break;
    case f_lr:
        m_vmeter->setChannels(2);
        setColor(6);
        setLogo(66);
        break;
    case f_mc:
        setColor(7);
        setLogo(67);
        panSlider->setDisabled(true);
        panSlider->setStyleSheet("QSlider {"
                                 "background-color: rgba(0, 0, 0, 0);"
                                 "height: "+QString::number(28*dpiRatio)+"px;"
                                 "border: 2px solid rgba(0,0,0,0);"
                                 "border-radius: 0px;}"
                                 "QSlider::handle:horizontal"
                                 "{"
                                 "background-color: rgba(0, 0, 0, 0);"
                                 "height: "+QString::number(36*dpiRatio)+"px;"
                                 "border: 0px solid rgb(0,0,0);"
                                 "border-radius: 0px;"
                                 "width: "+QString::number(16*dpiRatio)+"px;"
                                 "}"
                                 "QSlider::groove:horizontal"
                                 "{"
                                 "border-radius: 0px;"
                                 "}");
        break;
    case f_dca:
        setColor(8);
        setLogo(70);
        dyn->setDisabled(true);
        dyn->setStyleSheet("QPushButton {"
                           "color: rgba(0,0,0,0);"
                           "background-color: rgba(0, 0, 0, 0);"
                           "border: 0px solid rgb(0,0,0);"
                           "border-radius: 0px;}");
        eq->setDisabled(true);
        eq->setStyleSheet("QPushButton {"
                          "color: rgba(0,0,0,0);"
                          "background-color: rgba(0, 0, 0, 0);"
                          "border: 0px solid rgb(0,0,0);"
                          "border-radius: 0px;}");
        panSlider->setDisabled(true);
        panSlider->setStyleSheet("QSlider {"
                                 "background-color: rgba(0, 0, 0, 0);"
                                 "height: "+QString::number(28*dpiRatio)+"px;"
                                 "border: 2px solid rgba(0,0,0,0);"
                                 "border-radius: 0px;}"
                                 "QSlider::handle:horizontal"
                                 "{"
                                 "background-color: rgba(0, 0, 0, 0);"
                                 "height: "+QString::number(36*dpiRatio)+"px;"
                                 "border: 0px solid rgb(0,0,0);"
                                 "border-radius: 0px;"
                                 "width: "+QString::number(16*dpiRatio)+"px;"
                                 "}"
                                 "QSlider::groove:horizontal"
                                 "{"
                                 "border-radius: 0px;"
                                 "}");
        break;
    case f_error:
        qDebug()<<"Fader widget setup error!";
        break;
    }
}

void FaderWidget::setChannelNativeName(QString value) {
    nativeName->setText(value);
    setName(value);
}

void FaderWidget::setFaderValue(float value) {
    volSlider->setValue(value*10000);

    //Calculate db value
    //Resize one digit after point and set -inf
    if (value>0) db->setText(QString::number(round(float2db(value),1)) + " db");
    else db->setText("-inf db");
}

void FaderWidget::emitFaderChanged() {
    emit faderChanged(volSlider->value());

    //Calculate db value
    //Resize one digit after point and set -inf
    if (volSlider->value()>0) db->setText(QString::number(round(float2db(float(volSlider->value()) / 10000),1)) + " db");
    else db->setText("-inf db");
}

void FaderWidget::dbEditingFinished(){
    QString dbs = db->text();
    dbs.remove(" ");
    dbs.remove("d");
    dbs.remove("b");
    volSlider->setValue(db2float(dbs.toFloat()) * 10000);
}

void FaderWidget::setPanValue(float value) {panSlider->setValue(value*10000);}

void FaderWidget::setMute(bool value) {mute->setChecked(value);}

void FaderWidget::setSolo(bool value) {solo->setChecked(value);}

void FaderWidget::setLogo(int value) {
    if (value>0&&value<75) iconNumber = value;
    if (iconNumber != 1) icon->setVisible(true);
    else icon->setHidden(true);

    icon->setStyleSheet("QPushButton {"
                        "background-color: rgb(64, 64, 64);"
                        "border: 0px solid rgb(0,0,0);}");

    QIcon ico(":/f_icons/" + QString::number(iconNumber));
    icon->setIcon(ico);
    icon->setIconSize(QSize(72,72));
}

void FaderWidget::setColor(int value) {
    Q_UNUSED(value)
    //set color by number
}

void FaderWidget::setName(QString value) {
    if (value.size() > 10) {
            channelName->setFont(m_fonts->boldFont8);
            if (value.size() > 17) value.resize(17);
    }
    channelName->setText(value);
}

void FaderWidget::emitMuteChanged(){
    int value = 1;
    if (mute->isChecked()) value = 0;
    emit muteChanged(value);
}

void FaderWidget::emitSoloChanged(){
    int value = 0;
    if (solo->isChecked()) value = 1;
    emit soloChanged(value);
}

void FaderWidget::setMeter(float preL_, float preR_, float gate, float comp){
    Q_UNUSED(gate)
    Q_UNUSED(comp)
    m_vmeter->setMeter(preL_, preR_);
    m_dmeter->setlevel(preL_,preL_);
}

volMeter::volMeter() : QWidget() {
    backgroundNominalColor.setRgb(0x26, 0x7f, 0x26); // Dark green
    backgroundWarningColor.setRgb(0x7f, 0x7f, 0x26); // Dark yellow
    backgroundErrorColor.setRgb(0x7f, 0x26, 0x26);   // Dark red
    foregroundNominalColor.setRgb(0x4c, 0xff, 0x4c); // Bright green
    foregroundWarningColor.setRgb(0xff, 0xff, 0x4c); // Bright yellow
    foregroundErrorColor.setRgb(0xff, 0x4c, 0x4c);   // Bright red
    clipColor.setRgb(0xff, 0xff, 0xff);              // Bright white
    magnitudeColor.setRgb(0x00, 0x00, 0x00);         // Black
    majorTickColor.setRgb(0xff, 0xff, 0xff);         // Black
    minorTickColor.setRgb(0xcc, 0xcc, 0xcc);         // Black
    minimumLevel = -60.0;                            // -60 dB
    warningLevel = -10.0;                            // -20 dB
    errorLevel = -5.0;                               //  -9 dB
    clipLevel = -0.5;                                //  -0.5 dB
    minimumInputLevel = -90.0;                       // -50 dB
    peakDecayRate = 23.53;                           //  20 dB / 1.7 sec
    magnitudeIntegrationTime = 0.3;                  //  99% in 300 ms
    peakHoldDuration = 3.0;                         //  20 seconds
    inputPeakHoldDuration = 1.0;                     //  1 second

    channels = 1;
    displayNrAudioChannels = 1;
    currentPeak[0] = -M_INFINITE;
    currentInputPeak[0] = -M_INFINITE;
    currentInputPeak[0] = -M_INFINITE;
    currentPeak[1] = -M_INFINITE;
    currentInputPeak[1] = -M_INFINITE;
    currentInputPeak[1] = -M_INFINITE;

    elapsedTimer = new QElapsedTimer;
    elapsedTimer->start();
    setMinimumSize(displayNrAudioChannels * 20, 50);
    setMaximumWidth(displayNrAudioChannels * 20);
}

void volMeter::setMeter(float preL_, float preR_) {
    QMutexLocker locker(&dataMutex);
    uint64_t ts = elapsedTimer->nsecsElapsed();
    currentLastUpdateTime = ts;
    if (displayNrAudioChannels==1){
        currentPeak[0] = log10(preL_) * 20; //пик значение
        currentInputPeak[0] = currentPeak[0]; //индикатор сигнала
        currentMagnitude[0] = currentPeak[0]; //среднее значение
    } else {
        currentPeak[0] = log10(preL_) * 20;
        currentPeak[1] = log10(preR_) * 20;
        currentInputPeak[0] = currentPeak[0];
        currentInputPeak[1] = currentPeak[1];
        currentMagnitude[0] = currentPeak[0];
        currentMagnitude[1] = currentPeak[1];
    }
    locker.unlock();
    calculateBallistics(ts);
}

void volMeter::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    const QRect rect = event->region().boundingRect();
    int height = rect.height();

    uint64_t ts = elapsedTimer->nsecsElapsed();
    qreal timeSinceLastRedraw = (ts - lastRedrawTime) * 0.000000001;

    calculateBallistics(ts, timeSinceLastRedraw);
    bool idle = detectIdle(ts);

    QPainter painter(this);
    painter.translate(0, height);
    painter.scale(1, -1);

    for (int channelNr = 0; channelNr < displayNrAudioChannels;
         channelNr++) {
        paintVMeter(painter, channelNr * 20, 27, 18, height * 0.75 - 44,
                    displayMagnitude[channelNr],
                    displayPeak[channelNr],
                    displayPeakHold[channelNr]);
        if (idle)
            continue;
        paintInputMeter(painter, channelNr * 20, 3, 18, 18, displayInputPeakHold[channelNr]);
    }

    lastRedrawTime = ts;
}

void volMeter::paintVMeter(QPainter &painter, int x, int y, int width,
                           int height, float magnitude, float peak,
                           float peakHold)
{
    qreal scale = height / minimumLevel;

    QMutexLocker locker(&dataMutex);
    int minimumPosition = y;
    int maximumPosition = y + height;
    int magnitudePosition = int(y + height - (magnitude * scale));
    int peakPosition = int(y + height - (peak * scale));
    int peakHoldPosition = int(y + height - (peakHold * scale));
    int warningPosition = int(y + height - (warningLevel * scale));
    int errorPosition = int(y + height - (errorLevel * scale));

    int nominalLength = warningPosition - minimumPosition;
    int warningLength = errorPosition - warningPosition;
    int errorLength = maximumPosition - errorPosition;
    locker.unlock();

    if (clipping) {
        peakPosition = maximumPosition;
    }

    if (peakPosition < minimumPosition) {
        painter.fillRect(x, minimumPosition, width, nominalLength,
                         backgroundNominalColor);
        painter.fillRect(x, warningPosition, width, warningLength,
                         backgroundWarningColor);
        painter.fillRect(x, errorPosition, width, errorLength,
                         backgroundErrorColor);
    } else if (peakPosition < warningPosition) {
        painter.fillRect(x, minimumPosition, width,
                         peakPosition - minimumPosition,
                         foregroundNominalColor);
        painter.fillRect(x, peakPosition, width,
                         warningPosition - peakPosition,
                         backgroundNominalColor);
        painter.fillRect(x, warningPosition, width, warningLength,
                         backgroundWarningColor);
        painter.fillRect(x, errorPosition, width, errorLength,
                         backgroundErrorColor);
    } else if (peakPosition < errorPosition) {
        painter.fillRect(x, minimumPosition, width, nominalLength,
                         foregroundNominalColor);
        painter.fillRect(x, warningPosition, width,
                         peakPosition - warningPosition,
                         foregroundWarningColor);
        painter.fillRect(x, peakPosition, width,
                         errorPosition - peakPosition,
                         backgroundWarningColor);
        painter.fillRect(x, errorPosition, width, errorLength,
                         backgroundErrorColor);
    } else if (peakPosition < maximumPosition) {
        painter.fillRect(x, minimumPosition, width, nominalLength,
                         foregroundNominalColor);
        painter.fillRect(x, warningPosition, width, warningLength,
                         foregroundWarningColor);
        painter.fillRect(x, errorPosition, width,
                         peakPosition - errorPosition,
                         foregroundErrorColor);
        painter.fillRect(x, peakPosition, width,
                         maximumPosition - peakPosition,
                         backgroundErrorColor);
    } else {
        if (!clipping) {
            QTimer::singleShot(CLIP_FLASH_DURATION_MS, this,
                               SLOT(ClipEnding()));
            clipping = true;
        }

        int end = errorLength + warningLength + nominalLength;
        painter.fillRect(x, minimumPosition, width, end,
                         QBrush(foregroundErrorColor));
    }

    if (peakHoldPosition - 3 < minimumPosition)
        ; // Peak-hold below minimum, no drawing.
    else if (peakHoldPosition < warningPosition)
        painter.fillRect(x, peakHoldPosition - 3, width, 3,
                         foregroundNominalColor);
    else if (peakHoldPosition < errorPosition)
        painter.fillRect(x, peakHoldPosition - 3, width, 3,
                         foregroundWarningColor);
    else
        painter.fillRect(x, peakHoldPosition - 3, width, 3,
                         foregroundErrorColor);

    if (magnitudePosition - 3 >= minimumPosition)
        painter.fillRect(x, magnitudePosition - 3, width, 3,
                         magnitudeColor);
    locker.unlock();
}

void volMeter::paintInputMeter(QPainter &painter, int x, int y, int width,
                               int height, float peakHold)
{
    QMutexLocker locker(&dataMutex);
    QColor color;

    if (peakHold < minimumInputLevel)
        color = backgroundNominalColor;
    else if (peakHold < warningLevel)
        color = foregroundNominalColor;
    else if (peakHold < errorLevel)
        color = foregroundWarningColor;
    else if (peakHold <= clipLevel)
        color = foregroundErrorColor;
    else
        color = clipColor;

    painter.fillRect(x, y, width, height, color);
}

inline void volMeter::calculateBallistics(uint64_t ts,
                                          qreal timeSinceLastRedraw)
{
    QMutexLocker locker(&dataMutex);

    for (int channelNr = 0; channelNr < MAX_AUDIO_CHANNELS; channelNr++)
        calculateBallisticsForChannel(channelNr, ts,
                                      timeSinceLastRedraw);
    locker.unlock();
}

inline bool volMeter::detectIdle(uint64_t ts)
{
    double timeSinceLastUpdate = (ts - currentLastUpdateTime) * 0.000000001;
    if (timeSinceLastUpdate > 0.5) {
        resetLevels();
        return true;
    } else {
        return false;
    }
}

void volMeter::resetLevels()
{
    currentLastUpdateTime = 0;
    for (int channelNr = 0; channelNr < MAX_AUDIO_CHANNELS; channelNr++) {
        currentMagnitude[channelNr] = -M_INFINITE;
        currentPeak[channelNr] = -M_INFINITE;
        currentInputPeak[channelNr] = -M_INFINITE;

        displayMagnitude[channelNr] = -M_INFINITE;
        displayPeak[channelNr] = -M_INFINITE;
        displayPeakHold[channelNr] = -M_INFINITE;
        displayPeakHoldLastUpdateTime[channelNr] = 0;
        displayInputPeakHold[channelNr] = -M_INFINITE;
        displayInputPeakHoldLastUpdateTime[channelNr] = 0;
    }
}

inline void volMeter::calculateBallisticsForChannel(int channelNr, uint64_t ts,
                                                    qreal timeSinceLastRedraw)
{
    if (currentPeak[channelNr] >= displayPeak[channelNr] ||
            isnan(displayPeak[channelNr])) {
        // Attack of peak is immediate.
        displayPeak[channelNr] = currentPeak[channelNr];
    } else {
        // Decay of peak is 40 dB / 1.7 seconds for Fast Profile
        // 20 dB / 1.7 seconds for Medium Profile (Type I PPM)
        // 24 dB / 2.8 seconds for Slow Profile (Type II PPM)
        float decay = float(peakDecayRate * timeSinceLastRedraw);
        displayPeak[channelNr] = CLAMP(displayPeak[channelNr] - decay,
                                       currentPeak[channelNr], 0);
    }

    if (currentPeak[channelNr] >= displayPeakHold[channelNr] ||
            !isfinite(displayPeakHold[channelNr])) {
        // Attack of peak-hold is immediate, but keep track
        // when it was last updated.
        displayPeakHold[channelNr] = currentPeak[channelNr];
        displayPeakHoldLastUpdateTime[channelNr] = ts;
    } else {
        // The peak and hold falls back to peak
        // after 20 seconds.
        qreal timeSinceLastPeak =
                (uint64_t)(ts -
                           displayPeakHoldLastUpdateTime[channelNr]) *
                0.000000001;
        if (timeSinceLastPeak > peakHoldDuration) {
            displayPeakHold[channelNr] = currentPeak[channelNr];
            displayPeakHoldLastUpdateTime[channelNr] = ts;
        }
    }

    if (currentInputPeak[channelNr] >= displayInputPeakHold[channelNr] ||
            !isfinite(displayInputPeakHold[channelNr])) {
        // Attack of peak-hold is immediate, but keep track
        // when it was last updated.
        displayInputPeakHold[channelNr] = currentInputPeak[channelNr];
        displayInputPeakHoldLastUpdateTime[channelNr] = ts;
    } else {
        // The peak and hold falls back to peak after 1 second.
        qreal timeSinceLastPeak =
                (uint64_t)(
                    ts -
                    displayInputPeakHoldLastUpdateTime[channelNr]) *
                0.000000001;
        if (timeSinceLastPeak > inputPeakHoldDuration) {
            displayInputPeakHold[channelNr] = currentInputPeak[channelNr];
            displayInputPeakHoldLastUpdateTime[channelNr] = ts;
        }
    }

    if (!isfinite(displayMagnitude[channelNr])) {
        // The statements in the else-leg do not work with
        // NaN and infinite displayMagnitude.
        displayMagnitude[channelNr] = currentMagnitude[channelNr];
    } else {
        // A VU meter will integrate to the new value to 99% in 300 ms.
        // The calculation here is very simplified and is more accurate
        // with higher frame-rate.
        float attack =
                float((currentMagnitude[channelNr] -
                       displayMagnitude[channelNr]) *
                      (timeSinceLastRedraw / magnitudeIntegrationTime) *
                      0.99);
        displayMagnitude[channelNr] =
                CLAMP(displayMagnitude[channelNr] + attack,
                      (float)minimumLevel, 0);
    }
}

void volMeter::ClipEnding() { clipping = false; }

TicksPaint::TicksPaint(float dpiRatio_) : QWidget() {
    dpiRatio = dpiRatio_;
    tickFont = QFont("Arial");
    tickFont.setPixelSize(8*dpiRatio);
    tickFont.setStyleStrategy(QFont::PreferAntialias);

    majorTickColor.setRgb(0xff, 0xff, 0xff);
    minorTickColor.setRgb(0xcc, 0xcc, 0xcc);

    tickPaintCache = new QPixmap();

    setMinimumSize(20*dpiRatio, 40*dpiRatio);
    setMaximumWidth(20*dpiRatio);
}

void TicksPaint::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    const QRect rect = event->region().boundingRect();
    int height = rect.height() - 46*dpiRatio;

    QSize tickPaintCacheSize = QSize(20*dpiRatio, height);
    if (tickPaintCache == nullptr || tickPaintCache->size() != tickPaintCacheSize) {
        delete tickPaintCache;
        tickPaintCache = new QPixmap(tickPaintCacheSize);

        QColor clearColor(0, 0, 0, 0);
        tickPaintCache->fill(clearColor);

        QPainter tickPainter(tickPaintCache);
        tickPainter.translate(0, height);
        tickPainter.scale(1, -1);
        paintVTicks(tickPainter, 0, 0,
                    tickPaintCacheSize.height());
        tickPainter.end();
    }

    QPainter painter(this);

    painter.translate(0, height);
    painter.scale(1, -1);
    painter.drawPixmap(0, -23*dpiRatio, *tickPaintCache);
}

void TicksPaint::paintVTicks(QPainter &painter, int x, int y, int height)
{
    qreal scale = height / qreal(-40.0);
    painter.setFont(tickFont);
    int level = 10;
    for (int i = 0; i >= -40; i--) {
        int position = y + int((i * scale) - 1);
        if (i % 5 != 0) {
            painter.setPen(minorTickColor);
            painter.drawLine(x, position + 2, x + (3*dpiRatio), position + 2);
        } else {
            painter.setPen(majorTickColor);
            if (i != -40)
                painter.drawLine(x, position + 2, x + (4*dpiRatio), position + 2);
            else
                painter.drawLine(x, position + 1, x + (4*dpiRatio), position + 1);

            if (level == 10)
                painter.drawText(x + (8*dpiRatio), position + (7*dpiRatio), QString::number(level));
            else if (level == -70)
                painter.drawText(x + (8*dpiRatio), position - (1*dpiRatio), "-inf");
            else
                painter.drawText(x + (8*dpiRatio), position + (3*dpiRatio), QString::number(level));

            if (level >= -5) level = level - 5;
            else if (level >= -20) level = level - 10;
            else level = level - 20;
        }
    }
}

dynMeter::dynMeter(float dpiRatio_,QWidget *parent_) : QWidget() {
    this->setParent(parent_);
    dpiRatio = dpiRatio_;

    backColor.setRgb(96, 96, 96);
    treshColor.setRgb(255, 64, 64);

    dynMeterCache = nullptr;
    dynMeterSize = QSize(122*dpiRatio-10,36*dpiRatio-10);
}

void dynMeter::setlevel(float comp_, float gate_){
    currentGate = log10(gate_)*20 + 115;
    currentComp = log10(comp_)*20 + 115;
}

void dynMeter::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    if (dynMeterCache == nullptr) {
        delete dynMeterCache;
        dynMeterCache = new QPixmap(dynMeterSize);
        QColor clearColor(0, 0, 0, 0);
        dynMeterCache->fill(clearColor);
        QPainter cachePainter(dynMeterCache);
        paintCache(cachePainter,dynMeterSize);
        cachePainter.end();
    }

    QPainter painter(this);
    painter.drawPixmap(5, 5, *dynMeterCache);
    paintComp(painter,dynMeterSize);
    paintGate(painter,dynMeterSize);
}

void dynMeter::paintCache(QPainter &painter, QSize dynMeterSize)
{
    painter.fillRect(QRect(0,0,dynMeterSize.width(),dynMeterSize.height()/2-2),backColor);
    painter.fillRect(QRect(0,dynMeterSize.height()/2+2,dynMeterSize.width(),dynMeterSize.height()/2-2),backColor);
}

void dynMeter::paintComp(QPainter &painter, QSize dynMeterSize)
{
    qreal compSize = dynMeterSize.width() / 60 * currentComp;
    if (compSize > dynMeterSize.width()) compSize = dynMeterSize.width();
    if(compSize > 7)
        painter.fillRect(QRect(7,7,compSize-4,dynMeterSize.height()/2-6),treshColor);
}

void dynMeter::paintGate(QPainter &painter, QSize dynMeterSize)
{
    qreal gateSize = dynMeterSize.width() / 60 * currentGate;
    if (gateSize > dynMeterSize.width()) gateSize = dynMeterSize.width();
    if(gateSize > 7)
        painter.fillRect(QRect(7,dynMeterSize.height()/2+9,gateSize-4,dynMeterSize.height()/2-6),treshColor);
}

