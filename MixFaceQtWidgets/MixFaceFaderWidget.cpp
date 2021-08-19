#include "MixFaceFaderWidget.h"
#include <qglobal.h>
#include <QtCore/QVariant>
#include "MixFaceStaticMath.h"
#include <QTime>

FaderWidget::FaderWidget(float dpiRatio, MixFaceFonts *m_fonts,
                         MixFaceMetersTimer *mf_metersTimer, DebugLibrary *debug)
    : dpiRatio(dpiRatio), m_fonts(m_fonts), mf_metersTimer(mf_metersTimer), debug(debug)
{
    initWidget();
    connectWidgets();
}

void FaderWidget::initWidget(){
    dpiRatio -= 1;
    dpiRatio /= 2;
    dpiRatio += 1;
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
                             "QSlider::handle"
                             "{"
                             "background-color:"
                             "qlineargradient(spread:reflect,"
                             "x1:0.5, y1:0, x2:1, y2:0,"
                             "stop:0 rgba(0, 0, 0, 255),"
                             "stop:0.1 rgba(196, 196, 196, 255));"
                             "height: "+QString::number(36*dpiRatio)+"px;"
                             "border: 1px solid rgb(32,32,32);"
                             "border-radius: 4px;"
                             "width: "+QString::number(18*dpiRatio)+"px;"
                             "}"
                             "QSlider::handle:hover"
                             "{"
                             "background-color:"
                             "qlineargradient(spread:reflect,"
                             "x1:0.5, y1:0, x2:1, y2:0,"
                             "stop:0 rgba(0, 0, 0, 255),"
                             "stop:0.1 rgba(196, 196, 196, 255));"
                             "height: "+QString::number(36*dpiRatio)+"px;"
                             "border: 1px solid rgb(223,223,223);"
                             "border-radius: 4px;"
                             "width: "+QString::number(18*dpiRatio)+"px;"
                             "}"
                             "QSlider::groove"
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

    m_vmeter = new MixFaceVolumeMeter(this, debug, dpiRatio);
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
    volSlider->setStyleSheet(QString("QSlider {"
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
                                                 "margin-top: " + QString::number(22*dpiRatio) + "px;\n"
                                                 "}\n"
                                                 "\n"
                                                 "QSlider::add-page\n"
                                                 "{\n"
                                                 "background: rgb(96, 196, 96);\n"
                                                 "border: 1px solid rgb(0,0,0);\n"
                                                 "border-radius: 4px;\n"
                                                 "margin-bottom: " + QString::number(22*dpiRatio) + "px;\n"
                                                 "}\n"
                                                 "\n"
                                                 "QSlider::handle\n"
                                                 "{\n"
                                                 "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(196, 196, 196, 255), stop:0.12 rgba(64, 64, 64, 255), stop:0.480377 rgba(255, 255, 255, 255), stop:0.489564 rgba(0, 0, 0, 255), stop:0.510436 rgba(0, 0, 0, 255), stop:0.52 rgba(255, 255, 255, 255), stop:0.88 rgba(64, 64, 64, 255), stop:1 rgba(196, 196, 196, 255));\n"
                                                 "border: 1px solid rgb(32,32,32);\n"
                                                 "border-radius: 5px;\n"
                                                 "height: " + QString::number(44*dpiRatio) + "px;\n"
                                                 "margin: 0 -" + QString::number(18*dpiRatio) + "px;\n"
                                                 "}"
                                                 "QSlider::handle:pressed\n"
                                                 "{\n"
                                                 "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(196, 196, 196, 255), stop:0.12 rgba(64, 64, 64, 255), stop:0.480377 rgba(255, 255, 255, 255), stop:0.489564 rgba(0, 0, 0, 255), stop:0.510436 rgba(0, 0, 0, 255), stop:0.52 rgba(255, 255, 255, 255), stop:0.88 rgba(64, 64, 64, 255), stop:1 rgba(196, 196, 196, 255));\n"
                                                 "border: 1px solid rgb(223,223,223);\n"
                                                 "border-radius: 5px;\n"
                                                 "height: " + QString::number(44*dpiRatio) + "px;\n"
                                                 "margin: 0 -" + QString::number(18*dpiRatio) + "px;\n"
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
    botLayout->setContentsMargins(4, 4*dpiRatio, 4, 4);
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
    icon->setStyleSheet("QPushButton {"
                        "background-color: rgb(64, 64, 64);"
                        "border: 0px solid rgb(0,0,0);}");

    botArea = new QWidget;
    botArea->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);
    botArea->setMinimumHeight(96);
    botArea->setMaximumHeight(96*dpiRatio);
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

    mf_metersTimer->AddVolumeMeter(m_vmeter);
    mf_metersTimer->AddDynMeter(m_dmeter);
}

void FaderWidget::connectWidgets(){
    QPushButton::connect(src, &QPushButton::clicked, this,
                         &FaderWidget::srcClicked);
    QPushButton::connect(eq, &QPushButton::clicked, this,
                         &FaderWidget::eqClicked);
    QPushButton::connect(dyn, &QPushButton::clicked, this,
                         &FaderWidget::dynClicked);
    QPushButton::connect(mute, &QPushButton::clicked, this,
                         &FaderWidget::emitMuteChanged);
    QSlider::connect(panSlider, &QSlider::valueChanged,
                     this, &FaderWidget::emitPanChanged);
    QSlider::connect(panSlider, &QSlider::sliderReleased,
                     this, &FaderWidget::sliderReleased);
    QLineEdit::connect(db,&QLineEdit::editingFinished,
                       this,&FaderWidget::dbEditingFinished);
    QSlider::connect(volSlider, &QSlider::valueChanged,
                     this, &FaderWidget::emitFaderChanged);
    QPushButton::connect(solo, &QPushButton::clicked, this,
                         &FaderWidget::emitSoloChanged);
    QPushButton::connect(channelName,&QPushButton::clicked,this,
                         &FaderWidget::iconButtonClicked);
    QPushButton::connect(icon,&QPushButton::clicked,this,
                         &FaderWidget::iconButtonClicked);
}

FaderWidget::~FaderWidget(){
    QPushButton::disconnect(src, &QPushButton::clicked, this,
                            &FaderWidget::srcClicked);
    QPushButton::disconnect(eq, &QPushButton::clicked, this,
                            &FaderWidget::eqClicked);
    QPushButton::disconnect(dyn, &QPushButton::clicked, this,
                            &FaderWidget::dynClicked);
    QPushButton::disconnect(mute, &QPushButton::clicked, this,
                            &FaderWidget::emitMuteChanged);
    QSlider::disconnect(panSlider, &QSlider::valueChanged,
                        this, &FaderWidget::emitPanChanged);
    QLineEdit::disconnect(db,&QLineEdit::editingFinished,
                          this, &FaderWidget::dbEditingFinished);
    QSlider::disconnect(volSlider, &QSlider::valueChanged, this,
                        &FaderWidget::emitFaderChanged);
    QPushButton::disconnect(solo, &QPushButton::clicked, this,
                            &FaderWidget::emitSoloChanged);
    QPushButton::disconnect(channelName,&QPushButton::clicked,this,
                            &FaderWidget::iconButtonClicked);
    QPushButton::disconnect(icon,&QPushButton::clicked,this,
                            &FaderWidget::iconButtonClicked);
    mf_metersTimer->RemoveVolumeMeter(m_vmeter);
    mf_metersTimer->RemoveDynMeter(m_dmeter);
}

void FaderWidget::resizeEvent(QResizeEvent *e){
    QWidget::resizeEvent(e);
    int heightV = volArea->geometry().height() - 20;
    volSlider->setGeometry(QRect(42*dpiRatio, 10, 10*dpiRatio, heightV));
    m_vmeter->setGeometry(QRect(16*dpiRatio, 10, 10*dpiRatio, heightV));
    ticks->setGeometry(QRect(68*dpiRatio, 10, 10*dpiRatio, heightV));
}

void FaderWidget::setFaderType(FaderType ftype) {
    //lock fader features
    switch(ftype){
    case f_channel:
        setColor(0);
        m_dmeter->enableGate(true);
        break;
    case f_auxin:
        setLogo(55);
        setColor(1);
        dyn->setDisabled(true);
        dyn->setStyleSheet("QPushButton {"
                           "color: rgba(0,0,0,0);"
                           "background-color: rgba(0, 0, 0, 0);"
                           "border: 0px solid rgb(0,0,0);"
                           "border-radius: 0px;}");
        break;
    case f_fxreturn:
        setLogo(61);
        setColor(3);
        dyn->setDisabled(true);
        dyn->setStyleSheet("QPushButton {"
                           "color: rgba(0,0,0,0);"
                           "background-color: rgba(0, 0, 0, 0);"
                           "border: 0px solid rgb(0,0,0);"
                           "border-radius: 0px;}");
        break;
    case f_bus:
        setLogo(71);
        setColor(4);
        break;
    case f_matrix:
        setLogo(72);
        setColor(5);
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
        setLogo(66);
        m_vmeter->setChannels(2);
        setColor(7);
        break;
    case f_mc:
        setLogo(67);
        setColor(8);
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
        setLogo(70);
        setColor(9);
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
    QSlider::disconnect(volSlider, &QSlider::valueChanged,
                     this, &FaderWidget::emitFaderChanged);
    QLineEdit::disconnect(db,&QLineEdit::editingFinished,
                          this, &FaderWidget::dbEditingFinished);
    volSlider->setValue(value*10000);

    //Calculate db value
    //Resize one digit after point and set -inf
    if (value > 0.f)
        db->setText(QString::number(round2(float2db(value), 1)) + QString(" db"));
    else db->setText("-inf db");
    QLineEdit::connect(db,&QLineEdit::editingFinished,
                       this,&FaderWidget::dbEditingFinished);
    QSlider::connect(volSlider, &QSlider::valueChanged,
                     this, &FaderWidget::emitFaderChanged);
}

void FaderWidget::emitFaderChanged() {
    emit faderChanged(volSlider->value());

    //Calculate db value
    //Resize one digit after point and set -inf
    QLineEdit::disconnect(db,&QLineEdit::editingFinished,
                          this, &FaderWidget::dbEditingFinished);
    if (volSlider->value()>0)
        db->setText(QString::number(round2(float2db(float(volSlider->value()) / 10000), 1)) + " db");
    else db->setText("-inf db");
    QLineEdit::connect(db,&QLineEdit::editingFinished,
                          this, &FaderWidget::dbEditingFinished);
}

void FaderWidget::dbEditingFinished(){
    QSlider::disconnect(volSlider, &QSlider::valueChanged, this,
                        &FaderWidget::emitFaderChanged);
    QString dbs = db->text();
    dbs.remove(" ");
    dbs.remove("d");
    dbs.remove("b");
    volSlider->setValue(db2float(dbs.toFloat()) * 10000);
    QSlider::connect(volSlider, &QSlider::valueChanged, this,
                        &FaderWidget::emitFaderChanged);
    emit faderChanged(db2float(dbs.toFloat()) * 10000);
}

void FaderWidget::emitPanChanged(){
    emit panChanged(float(panSlider->value()) / 10000);
}

void FaderWidget::sliderReleased(){
    int currentSliderReleasedTime = QTime::currentTime().msec() + (QTime::currentTime().second() * 1000) + (QTime::currentTime().minute() * 60000) + (QTime::currentTime().hour() * 3600000);
    debug->sendMessage(QString::number(lastSliderReleasedTime).toStdString(), 0);
    debug->sendMessage(QString::number(currentSliderReleasedTime).toStdString(), 0);
    if((currentSliderReleasedTime - lastSliderReleasedTime) < 300)
        panSlider->setValue(5000);
    lastSliderReleasedTime = currentSliderReleasedTime;
}

void FaderWidget::setPanValue(float value) {
    QSlider::disconnect(panSlider, &QSlider::valueChanged,
                        this, &FaderWidget::emitPanChanged);
    panSlider->setValue(value*10000);
    QSlider::connect(panSlider, &QSlider::valueChanged,
                     this,&FaderWidget::emitPanChanged);
}

void FaderWidget::setMute(bool value) {mute->setChecked(value);}

void FaderWidget::setSolo(bool value) {solo->setChecked(value);}

void FaderWidget::setLogo(int value) {
    if (value >= 1 && value <= 74) iconNumber = value;
    if (iconNumber != 1) icon->setVisible(true);
    else icon->setHidden(true);

    icon->setIcon(QIcon(":/fader/icons/" + QString::number(iconNumber) + BWIcon));
    icon->setIconSize(QSize(72,72));
}

void FaderWidget::setColor(int value) {
    colorNumber = value;
    QString fgcolor;
    QString bgcolor;
    QString fontColor;

    switch(colorNumber){
    case 0:
        BWIcon = "W";
        fontColor = "rgb(255,255,255)";
        fgcolor = "rgb(255,255,255)";
        bgcolor = "rgb(64,64,64)";
        break;
    case 1:
        BWIcon = "B";
        fontColor = "rgb(0,0,0)";
        bgcolor = "rgb(255,64,64)";
        fgcolor = "rgb(32,32,32)";
        break;
    case 2:
        BWIcon = "B";
        fontColor = "rgb(0,0,0)";
        bgcolor = "rgb(64,255,64)";
        fgcolor = "rgb(32,32,32)";
        break;
    case 3:
        BWIcon = "B";
        fontColor = "rgb(0,0,0)";
        bgcolor = "rgb(255,255,64)";
        fgcolor = "rgb(32,32,32)";
        break;
    case 4:
        BWIcon = "B";
        fontColor = "rgb(0,0,0)";
        bgcolor = "rgb(96,96,255)";
        fgcolor = "rgb(32,32,32)";
        break;
    case 5:
        BWIcon = "B";
        fontColor = "rgb(0,0,0)";
        bgcolor = "rgb(255,64,255)";
        fgcolor = "rgb(32,32,32)";
        break;
    case 6:
        BWIcon = "B";
        fontColor = "rgb(0,0,0)";
        bgcolor = "rgb(64,255,255)";
        fgcolor = "rgb(32,32,32)";
        break;
    case 7:
        BWIcon = "B";
        fontColor = "rgb(0,0,0)";
        bgcolor = "rgb(255,255,255)";
        fgcolor = "rgb(32,32,32)";
        break;
    case 8:
        BWIcon = "B";
        fontColor = "rgb(0,0,0)";
        bgcolor = "rgb(192,192,192)";
        fgcolor = "rgb(32,32,32)";
        break;
    case 9:
        BWIcon = "W";
        fontColor = "rgb(255,255,255)";
        bgcolor = "rgb(32,32,32)";
        fgcolor = "rgb(255,0,0)";
        break;
    case 10:
        BWIcon = "W";
        fontColor = "rgb(255,255,255)";
        bgcolor = "rgb(32,32,32)";
        fgcolor = "rgb(0,255,0)";
        break;
    case 11:
        BWIcon = "W";
        fontColor = "rgb(255,255,255)";
        bgcolor = "rgb(32,32,32)";
        fgcolor = "rgb(255,255,0)";
        break;
    case 12:
        BWIcon = "W";
        fontColor = "rgb(255,255,255)";
        bgcolor = "rgb(32,32,32)";
        fgcolor = "rgb(32,32,255)";
        break;
    case 13:
        BWIcon = "W";
        fontColor = "rgb(255,255,255)";
        bgcolor = "rgb(32,32,32)";
        fgcolor = "rgb(255,0,255)";
        break;
    case 14:
        BWIcon = "W";
        fontColor = "rgb(255,255,255)";
        bgcolor = "rgb(32,32,32)";
        fgcolor = "rgb(0,255,255)";
        break;
    case 15:
        BWIcon = "W";
        fontColor = "rgb(255,255,255)";
        bgcolor = "rgb(32,32,32)";
        fgcolor = "rgb(255,255,255)";
        break;
    default:
        BWIcon = "W";
        fontColor = "rgb(255,255,255)";
        fgcolor = "rgb(255,255,255)";
        bgcolor = "rgb(64,64,64)";
        break;
    }

    //set color by number
    channelName->setStyleSheet("QPushButton {"
                               "background-color: " + bgcolor + ";"
                               "color: " + fontColor + ";"
                               "border: 0px solid rgb(0,0,0);}");
    icon->setStyleSheet("QPushButton {"
                        "background-color: " + bgcolor + ";"
                        "border: 0px solid rgb(0,0,0);}");
    botArea->setStyleSheet("QWidget {"
                           "border: 2px solid " + fgcolor + ";"
                           "background-color: " + bgcolor + ";"
                           "}");

    setLogo(iconNumber);
}

void FaderWidget::setName(QString value) {
    if (value.length() > 10) {
            channelName->setFont(m_fonts->boldFont8);
            if (value.length() > 17) value.resize(17);
    } else
        channelName->setFont(m_fonts->boldFont12);

    if (value.length() < 1)
        channelName->setText(nativeName->text());
    else
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

void FaderWidget::setMeter(float preL_, float preR_){
    m_vmeter->setMeter(preL_, preR_);
}

void FaderWidget::setDynamics(float comp){
    if(dyn->isEnabled())
        m_dmeter->setDynamics(comp);
}

void FaderWidget::setGate(float gate){
    if(dyn->isEnabled())
        m_dmeter->setGate(gate);
}

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

dynMeter::dynMeter(float dpiRatio_, QWidget *parent_) : QWidget() {
    this->setParent(parent_);
    dpiRatio = dpiRatio_;

    backColor.setRgb(96, 96, 96);
    treshColor.setRgb(255, 64, 64);

    dynMeterCache = nullptr;
    dynMeterSize = QSize(122*dpiRatio-10,36*dpiRatio-10);
}

void dynMeter::setDynamics(float comp_){
    currentComp = compGainReductionToFloatRatio(comp_);
}

void dynMeter::setGate(float gate_){
    currentGate = gateGainReductionToFloatRatio(gate_);
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
        paintCache(cachePainter, dynMeterSize);
        cachePainter.end();
    }

    QPainter painter(this);
    painter.drawPixmap(5, 5, *dynMeterCache);
    paintComp(painter,dynMeterSize);
    if(gate)
        paintGate(painter,dynMeterSize);
}

void dynMeter::paintCache(QPainter &painter, QSize dynMeterSize)
{
    painter.fillRect(QRect(0,0,dynMeterSize.width(),dynMeterSize.height()/2-2),backColor);
    if(gate){
        painter.fillRect(QRect(0,dynMeterSize.height()/2+2,dynMeterSize.width(),dynMeterSize.height()/2-2),backColor);
    }
}

void dynMeter::paintComp(QPainter &painter, QSize dynMeterSize)
{
    qreal compSize = dynMeterSize.width() * currentComp;
    if(compSize > 1)
        painter.fillRect(QRect(7,7,compSize-4,dynMeterSize.height()/2-6),treshColor);
}

void dynMeter::paintGate(QPainter &painter, QSize dynMeterSize)
{
    qreal gateSize = dynMeterSize.width() * currentGate;
    if(gateSize > 1)
        painter.fillRect(QRect(7,dynMeterSize.height()/2+9,gateSize-4,dynMeterSize.height()/2-6),treshColor);
}

