#include "MixFaceWindow.h"
#include "MixFaceStatic.h"
#include <QtNetwork/QNetworkInterface>

int main(int argc, char *argv[]) {
    qputenv("QT_ENABLE_HIGHDPI_SCALING", "0"); //Disable Auto High DPI
    QApplication *mixFace = new QApplication(argc, argv);
    MixFaceWindow *mixFaceWindow = new MixFaceWindow(new DebugLibrary(argc, argv));
    mixFaceWindow->showMaximized();
    return mixFace->exec();
}

MixFaceWindow::MixFaceWindow(DebugLibrary *debug)
    : QMainWindow(), debug(debug) {
    dpiRatio = float(this->logicalDpiX())/96;
    if (debug->dpi != -1.f) dpiRatio = debug->dpi;
    debug->sendMessage(QString("MixFaceWindow::MixFaceWindow DPI ratio set to " + QString::number(dpiRatio)).toStdString(),1);

    debug->sendMessage(QString("MixFaceWindow::MixFaceWindow Init MixFaceFonts...").toStdString(),5);
    mf_fonts = new MixFaceFonts;

    debug->sendMessage("MixFaceWindow::MixFaceWindow Init MixFaceLibrary...",3);
    mf_library = new MixFaceLibrary(debug);

    mf_metersTimer = new MixFaceMetersTimer;
    mf_demoTimer = new MixFaceDemoTimer;
    initUI();

    for(int idx = 0;idx<80;idx++)
        for (int idy = 0;idy < 7;idy++)
            if(idx!=70&&idx!=71&&(idx<48||idx>63))
                assignedFader[idx][idy] = true;
            else
                assignedFader[idx][idy] = false;

    mf_metersTimer->start(34);
    mf_demoTimer->start(5);
}

void MixFaceWindow::initUI(){
    debug->sendMessage(QString("MixFaceWindow::initUI Init ui...").toStdString(),1);

    this->resize(1280, 720);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setEnabled(true);

    bar = new QScrollBar(this);
    bar->setOrientation(Qt::Horizontal);
    bar->setGeometry(QRect(0,(this->geometry().height())-17,(this->geometry().width())-(64*dpiRatio)-(128*dpiRatio),14));

    verticalLayout = new QVBoxLayout(centralWidget);
    verticalLayout->setSpacing(0);
    verticalLayout->setContentsMargins(0, 0, 0, 0);

    mf_topArea = new MixFaceTopWidget(dpiRatio, debug, mf_fonts, this);
    mf_topArea->setMaximumHeight(32*dpiRatio);
    mf_topArea->setMinimumHeight(32*dpiRatio);
    verticalLayout->addWidget(mf_topArea);
    connect(mf_topArea, &MixFaceTopWidget::connect, this, &MixFaceWindow::connection);
    connect(mf_topArea, &MixFaceTopWidget::sync, this, &MixFaceWindow::sendSyncMessages);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setSpacing(0);
    horizontalLayout->setContentsMargins(0, 0, 0, 0);

    scrollArea = new QScrollArea;
    QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());
    scrollArea->setSizePolicy(sizePolicy);
    scrollArea->setMinimumSize(QSize(1, 1));
    scrollArea->setBaseSize(QSize(0, 0));
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setLineWidth(0);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QScrollBar *sbar = scrollArea->horizontalScrollBar();
    connect(sbar, &QScrollBar::rangeChanged,bar,&QScrollBar::setRange);
    connect(bar, &QScrollBar::valueChanged,sbar,&QScrollBar::setValue);
    connect(sbar, &QScrollBar::valueChanged,bar,&QScrollBar::setValue);
    bar->setStyleSheet("QScrollBar::horizontal {"
                              "border: none;"
                              "background-color: transparent;"
                              "height: 12px;"
                              "margin-left: 3px;"
                              "margin-right: 3px;"
                              "}"
                              "QScrollBar::handle:horizontal {"
                              "background-color: rgba(128, 128, 128, 128);"
                              "border: 1px solid rgba(255,255,255,128);"
                              "border-radius: 7px;"
                              "min-width: 120px;"
                              "}"
                              "QScrollBar::add-line:horizontal,"
                              "QScrollBar::sub-line:horizontal {"
                              "border: none;"
                              "background-color: none;"
                              "color: none;"
                              "}"
                              "QScrollBar::left-arrow:horizontal,"
                              "QScrollBar::right-arrow:horizontal,"
                              "QScrollBar::add-page:horizontal,"
                              "QScrollBar::sub-page:horizontal {"
                              "border: none;"
                              "background: none;"
                              "color: none;"
                              "}"
                              "QScrollBar::corner {"
                              "background-color: transparent;"
                              "border: none;"
                              "}");
    scrollArea->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    scrollArea->setWidgetResizable(true);
    scrollArea->setAlignment(Qt::AlignCenter);
    QScroller::grabGesture(scrollArea, QScroller::LeftMouseButtonGesture);

    horizontalLayout->addWidget(scrollArea);

    srcArea = new QScrollArea;
    srcArea->setFrameShape(QFrame::NoFrame);
    srcArea->setLineWidth(0);
    srcArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    srcArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    srcArea->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    srcArea->setWidgetResizable(true);
    srcArea->setAlignment(Qt::AlignCenter);

    srcLayout = new QHBoxLayout;
    srcLayout->setContentsMargins(0, 0, 0, 0);
    srcLayout->setSpacing(0);
    srcArea->setLayout(srcLayout);

    horizontalLayout->addWidget(srcArea);

    mainArea = new QScrollArea;
    QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Expanding);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(mainArea->sizePolicy().hasHeightForWidth());
    mainArea->setSizePolicy(sizePolicy1);
    mainArea->setFrameShape(QFrame::NoFrame);
    mainArea->setLineWidth(0);
    mainArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainArea->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    mainArea->setWidgetResizable(true);
    mainArea->setAlignment(Qt::AlignCenter);

    horizontalLayout->addWidget(mainArea);

    QWidget *enwidget = new QWidget;
    enwidget->setMinimumSize(32,32);
    enwidget->setStyleSheet("QWidget {"
                        "border: 0px solid rgb(0,0,0);"
                        "background-color: rgb(96,96,96);"
                        "}");
    QVBoxLayout *envlayout = new QVBoxLayout;
    envlayout->setContentsMargins(0, 0, 0, 0);
    mf_rightArea = new MixFaceRightWidget(dpiRatio, debug, mf_fonts, this);
    connect(mf_rightArea, &MixFaceRightWidget::assignMainFader, this, &MixFaceWindow::assignMainFader);
    connect(mf_rightArea, &MixFaceRightWidget::assignScrollFaders, this, &MixFaceWindow::assignScrollFaders);
    envlayout->addWidget(mf_rightArea);
    enwidget->setLayout(envlayout);
    horizontalLayout->addWidget(enwidget);

    verticalLayout->addLayout(horizontalLayout);

    debug->sendMessage(QString("MixFaceWindow::initControlAreaWidgets...").toStdString(),4);
    initControlAreaWidgets();
    debug->sendMessage(QString("MixFaceWindow::initMainAreaWidgets...").toStdString(),4);
    initMainAreaWidgets();
    debug->sendMessage(QString("MixFaceWindow::initMixFaceIconPicker...").toStdString(),4);
    mf_picker = new MixFaceIconPicker(dpiRatio, debug, mf_fonts, this);
    connect(mf_picker,&MixFaceIconPicker::logoChanged, this, &MixFaceWindow::logoChanged);
    connect(mf_picker,&MixFaceIconPicker::colorChanged, this, &MixFaceWindow::colorChanged);
    connect(mf_picker,&MixFaceIconPicker::nameChanged, this, &MixFaceWindow::nameChanged);

    mf_topArea->syncAction->setDisabled(true);
    srcArea->setWidget(nullptr);
    srcArea->setHidden(true);
    dynWidget = nullptr;
    srcWidget = nullptr;
    eqWidget = nullptr;
    srcFader = nullptr;

    mf_topArea->lineIp->setText("127.0.0.1");
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address : QNetworkInterface::allAddresses()) {
      if (address.protocol() == QAbstractSocket::IPv4Protocol &&
          address != localhost) {
          debug->sendMessage(QString("MixFaceWindow::MixFaceWindow getting ip address " + address.toString()).toStdString(),4);
          mf_topArea->lineIp->setText(address.toString());
      }
    }

    debug->sendMessage(QString("MixFaceWindow::initUI Set main window widget...").toStdString(),1);
    this->setCentralWidget(centralWidget);

    returnThread = new ReturnThread;
    returnThread->moveToThread(qApp->thread());
    returnThread->start();
    connect(returnThread, &ReturnThread::connectedSignal,
            this, &MixFaceWindow::threadConnected);
    connect(returnThread, &ReturnThread::valueSignal,
            this, &MixFaceWindow::threadValueChanged);
    connect(returnThread, &ReturnThread::meter1Signal,
            this, &MixFaceWindow::threadMeter1);
    connect(returnThread, &ReturnThread::meter2Signal,
            this, &MixFaceWindow::threadMeter2);
    connect(returnThread, &ReturnThread::meter3Signal,
            this, &MixFaceWindow::threadMeter3);

    mf_library->valueChanged.connect(
                signal_type_thr_int(&MixFaceWindow::slotValueChanged, this,
                                    boost::arg<1>(), boost::arg<2>(), boost::arg<3>()));
    mf_library->slotConnected.connect(
                signal_type_bool(&MixFaceWindow::slotConnected,
                                 this,boost::arg<1>()));
    mf_library->newMeters1.connect(
                signal_type_float_array(&MixFaceWindow::slotMeter1,
                                        this,boost::arg<1>()));
    mf_library->newMeters2.connect(
                signal_type_float_array(&MixFaceWindow::slotMeter2,
                                        this,boost::arg<1>()));
    mf_library->newMeters3.connect(
                signal_type_float_array(&MixFaceWindow::slotMeter3,
                                        this,boost::arg<1>()));
}

MixFaceWindow::~MixFaceWindow() {}

void MixFaceWindow::resizeEvent(QResizeEvent *e){
    QMainWindow::resizeEvent(e);
    bar->setGeometry(QRect(0,(this->geometry().height())-17,(this->geometry().width())-(64*dpiRatio)-(128*dpiRatio),14));
}

void MixFaceWindow::connection(){
    mf_library->threadConnect(mf_topArea->lineIp->text().toStdString());
}

void MixFaceWindow::slotConnected(bool state){
    returnThread->connectedSlot(state);
}

void MixFaceWindow::threadConnected(bool state){
    connected = state;
    if (state) {
        mf_topArea->syncAction->setEnabled(true);
        mf_topArea->lineIp->setDisabled(true);
        //mf_library->threadSendSyncMessages();
        mf_topArea->connectAction->setText("Disconnect...");
        mf_topArea->consoleName->setText(QString::fromStdString(mf_library->xinfo[1]));
        mf_topArea->consoleName->setStyleSheet("QLabel {"
                                   "color: rgb(255,255,255);"
                                   "background-color: rgb(96, 96, 96);"
                                   "border: 0px solid rgb(0,0,0);"
                                   "border-radius: 0px;"
                                   "}");
        mf_topArea->console->setText(QString::fromStdString(mf_library->xinfo[2]));
        mf_demoTimer->connected = state;
    } else {
        mf_topArea->syncAction->setDisabled(true);
        mf_topArea->lineIp->setEnabled(true);
        mf_topArea->connectAction->setText("Try connect...");
        mf_topArea->consoleName->setText("Connection issue!");
        mf_topArea->consoleName->setStyleSheet("QLabel {"
                                   "color: rgb(255,96,96);"
                                   "background-color: rgb(96, 96, 96);"
                                   "border: 0px solid rgb(0,0,0);"
                                   "border-radius: 0px;"
                                   "}");
        mf_topArea->console->setText("MixFace");
        mf_demoTimer->connected = state;
    }
}

void MixFaceWindow::initControlAreaWidgets() {
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->setSpacing(0);
    for(int idx=0;idx<80;idx++) {
      initControlAreaWidgetIdx(idx, mf_types.getFaderType(idx));
      hlayout->addWidget(scrollWidget[idx]);
    }
    controlAreaWidget = new QWidget;
    controlAreaWidget->setLayout(hlayout);
    scrollArea->setWidget(controlAreaWidget);
    scrollArea->setStyleSheet("background-color: rgb(96, 96, 96);");
}

void MixFaceWindow::initControlAreaWidgetIdx(int idx, FaderType ftype) {
    scrollWidget[idx] = new FaderWidget(dpiRatio, mf_fonts, mf_metersTimer, debug);
    mf_demoTimer->AddFader(scrollWidget[idx]);
    scrollWidget[idx]->setChannelNativeName(QString::fromStdString(channelNameFromIdx(idx)));
    scrollWidget[idx]->setFaderType(ftype);
    scrollWidget[idx]->setProperty("idx", idx);
    scrollWidget[idx]->setFaderValue(mf_library->db->fader[idx]);
    scrollWidget[idx]->setPanValue(mf_library->db->pan[idx]);
    if (mf_library->db->on[idx]==0)
        scrollWidget[idx]->setMute(true);
    scrollWidget[idx]->setSolo(mf_library->db->solo[idx]);
    scrollWidget[idx]->setLogo(mf_library->db->configicon[idx]);
    scrollWidget[idx]->setColor(mf_library->db->configcolor[idx]);
    scrollWidget[idx]->setName(QString::fromStdString(mf_library->db->configname[idx]));
    connect(scrollWidget[idx], &FaderWidget::faderChanged, this, &MixFaceWindow::faderChanged);
    connect(scrollWidget[idx], &FaderWidget::panChanged, this, &MixFaceWindow::panChanged);
    connect(scrollWidget[idx], &FaderWidget::muteChanged, this, &MixFaceWindow::muteClicked);
    connect(scrollWidget[idx], &FaderWidget::soloChanged, this, &MixFaceWindow::soloClicked);
    connect(scrollWidget[idx], &FaderWidget::srcClicked, this, &MixFaceWindow::buttonSrcClicked);
    connect(scrollWidget[idx], &FaderWidget::eqClicked, this, &MixFaceWindow::buttonEqClicked);
    connect(scrollWidget[idx], &FaderWidget::dynClicked, this, &MixFaceWindow::buttonDynClicked);
    connect(scrollWidget[idx], &FaderWidget::iconButtonClicked, this, &MixFaceWindow::iconButtonClicked);
}

void MixFaceWindow::initMainAreaWidgets() {
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->setSpacing(0);
    for(int idx=48;idx<64;idx++){
        initMainAreaWidgetIdx(idx);
        hlayout->addWidget(mainWidget[idx]);
        mainWidget[idx]->setVisible(false);
    }
    initMainAreaWidgetIdx(70);
    hlayout->addWidget(mainWidget[70]);
    initMainAreaWidgetIdx(71);
    hlayout->addWidget(mainWidget[71]);
    mainWidget[71]->setVisible(false);
    mainAreaWidget = new QWidget;
    mainAreaWidget->setLayout(hlayout);
    mainArea->setWidget(mainAreaWidget);
}

void MixFaceWindow::initMainAreaWidgetIdx(int idx) {
    mainWidget[idx] = new FaderWidget(dpiRatio, mf_fonts, mf_metersTimer, debug);
    mf_demoTimer->AddFader(mainWidget[idx]);
    mainWidget[idx]->setChannelNativeName(QString::fromStdString(channelNameFromIdx(idx)));
    mainWidget[idx]->setFaderType(mf_types.getFaderType(idx));
    mainWidget[idx]->setProperty("idx", idx);
    mainWidget[idx]->setFaderValue(mf_library->db->fader[idx]);
    mainWidget[idx]->setPanValue(mf_library->db->pan[idx]);
    if (mf_library->db->on[idx]==0)
        mainWidget[idx]->setMute(true);
    mainWidget[idx]->setSolo(mf_library->db->solo[idx]);
    mainWidget[idx]->setLogo(mf_library->db->configicon[idx]);
    mainWidget[idx]->setColor(mf_library->db->configcolor[idx]);
    mainWidget[idx]->setName(QString::fromStdString(mf_library->db->configname[idx]));
    connect(mainWidget[idx], &FaderWidget::faderChanged, this, &MixFaceWindow::faderChanged);
    connect(mainWidget[idx], &FaderWidget::panChanged, this, &MixFaceWindow::panChanged);
    connect(mainWidget[idx], &FaderWidget::muteChanged, this, &MixFaceWindow::muteClicked);
    connect(mainWidget[idx], &FaderWidget::soloChanged, this, &MixFaceWindow::soloClicked);
    connect(mainWidget[idx], &FaderWidget::srcClicked, this, &MixFaceWindow::buttonSrcClicked);
    connect(mainWidget[idx], &FaderWidget::eqClicked, this, &MixFaceWindow::buttonEqClicked);
    connect(mainWidget[idx], &FaderWidget::dynClicked, this, &MixFaceWindow::buttonDynClicked);
    connect(mainWidget[idx], &FaderWidget::iconButtonClicked, this, &MixFaceWindow::iconButtonClicked);
}

void MixFaceWindow::assignMainFader(int idx, int idy) {
    for (int id=48;id<64;id++) {
        mainWidget[id]->setVisible(false);
    }
    mainWidget[70]->setVisible(false);
    mainWidget[71]->setVisible(false);

    mainWidget[idx]->setVisible(true);

    QString name = "LR";
    if (idx!=70)
        name = QString::fromStdString(channelNameFromIdx(idx));
    mf_rightArea->busButton->setText(name);

    currentIdy = idy;
    windowRenew();
}

void MixFaceWindow::assignScrollFaders(int idy) {
    for(int idx = 0;idx<80;idx++){
        scrollWidget[idx]->setVisible(assignedFader[idx][idy]);
    }
}

void MixFaceWindow::iconButtonClicked(){
    int idx = reinterpret_cast<QObject *>(sender())->property("idx").toInt();
    mf_picker->showIconPopup(idx, mf_library->db->configicon[idx],
                                 mf_library->db->configcolor[idx],
                                 QString::fromStdString(channelNameFromIdx(idx)),
                                 QString::fromStdString(mf_library->db->configname[idx]));
}

void MixFaceWindow::logoChanged(int idx, int value){
    scrollWidget[idx]->setLogo(value);
    if ((idx>47&&idx<64)||idx==70||idx==71) {
        mainWidget[idx]->setLogo(value);
    } else if (srcArea->isVisible() && srcFader->property("idx").toInt() == idx) {
        srcFader->setLogo(value);
    }
    mf_library->db->configicon[idx] = value;

    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(configicon, chtype, chN, 0);
        mf_library->sendInt(msg.c_str(), value);
    }
}

void MixFaceWindow::colorChanged(int idx, int value){
    scrollWidget[idx]->setColor(value);
    if ((idx>47&&idx<64)||idx==70||idx==71)
        mainWidget[idx]->setColor(value);
    else if (srcArea->isVisible() && srcFader->property("idx").toInt() == idx)
        srcFader->setColor(value);
    mf_library->db->configcolor[idx] = value;

    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(configcolor, chtype, chN, 0);
        mf_library->sendInt(msg.c_str(), value);
    }
}

void MixFaceWindow::nameChanged(int idx, QString value){
    scrollWidget[idx]->setName(value);
    if ((idx>47&&idx<64)||idx==70||idx==71) {
        mainWidget[idx]->setName(value);
    } else if (srcArea->isVisible() && srcFader->property("idx").toInt() == idx) {
        srcFader->setName(value);
    }
    mf_library->db->configname[idx] = value.toStdString();

    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(configname, chtype, chN, 0);
        mf_library->sendString(msg.c_str(), value.toStdString());
    }
}

void MixFaceWindow::buttonSrcClicked(){
    int idx = reinterpret_cast<QObject *>(sender())->property("idx").toInt();

    if (srcWidget != nullptr) { srcWidget->setHidden(true); }
    if (eqWidget != nullptr) { eqWidget->setHidden(true); }
    if (dynWidget != nullptr) { dynWidget->setHidden(true); }

    if (srcFader != nullptr) {
        mf_demoTimer->RemoveFader(srcFader);
        disconnect(srcFader, &FaderWidget::faderChanged, this, &MixFaceWindow::faderChanged);
        disconnect(srcFader, &FaderWidget::panChanged, this, &MixFaceWindow::panChanged);
        disconnect(srcFader, &FaderWidget::muteChanged, this, &MixFaceWindow::muteClicked);
        disconnect(srcFader, &FaderWidget::soloChanged, this, &MixFaceWindow::soloClicked);
        disconnect(srcFader, &FaderWidget::srcClicked, this, &MixFaceWindow::buttonSrcClicked);
        disconnect(srcFader, &FaderWidget::eqClicked, this, &MixFaceWindow::buttonEqClicked);
        disconnect(srcFader, &FaderWidget::dynClicked, this, &MixFaceWindow::buttonDynClicked);
        disconnect(srcFader, &FaderWidget::iconButtonClicked, this, &MixFaceWindow::iconButtonClicked);
        delete srcFader;
        srcFader = nullptr;
    }

    if (m_mode != mSrc){
        if(idx!=70&&idx!=71&&(idx<48||idx>63)){
            srcFader = new FaderWidget(dpiRatio, mf_fonts, mf_metersTimer, debug);
            mf_demoTimer->AddFader(srcFader);
            srcLayout->addWidget(srcFader);
            srcFader->setChannelNativeName(QString::fromStdString(channelNameFromIdx(idx)));
            srcFader->setFaderType(mf_types.getFaderType(idx));
            srcFader->setProperty("idx", idx);
            connect(srcFader, &FaderWidget::faderChanged, this, &MixFaceWindow::faderChanged);
            connect(srcFader, &FaderWidget::panChanged, this, &MixFaceWindow::panChanged);
            connect(srcFader, &FaderWidget::muteChanged, this, &MixFaceWindow::muteClicked);
            connect(srcFader, &FaderWidget::soloChanged, this, &MixFaceWindow::soloClicked);
            connect(srcFader, &FaderWidget::srcClicked, this, &MixFaceWindow::buttonSrcClicked);
            connect(srcFader, &FaderWidget::eqClicked, this, &MixFaceWindow::buttonEqClicked);
            connect(srcFader, &FaderWidget::dynClicked, this, &MixFaceWindow::buttonDynClicked);
            connect(srcFader, &FaderWidget::iconButtonClicked, this, &MixFaceWindow::iconButtonClicked);
        }

        if (srcWidget == nullptr)
            srcWidget = new SourceWidget(dpiRatio, mf_fonts);
        else
            srcWidget->setVisible(true);
        srcLayout->addWidget(srcWidget);
        srcWidget->setIdx(idx);

        scrollArea->setHidden(true);
        bar->setHidden(true);
        srcArea->setVisible(true);
        m_mode = mSrc;
    } else {
        srcArea->setHidden(true);
        scrollArea->setVisible(true);
        bar->setVisible(true);
        m_mode = mCtrl;
    }
}

void MixFaceWindow::buttonEqClicked(){
    int idx = reinterpret_cast<QObject *>(sender())->property("idx").toInt();

    if (srcWidget != nullptr) { srcWidget->setHidden(true); }
    if (eqWidget != nullptr) { eqWidget->setHidden(true); }
    if (dynWidget != nullptr) { dynWidget->setHidden(true); }

    if (srcFader != nullptr) {
        mf_demoTimer->RemoveFader(srcFader);
        disconnect(srcFader, &FaderWidget::faderChanged, this, &MixFaceWindow::faderChanged);
        disconnect(srcFader, &FaderWidget::panChanged, this, &MixFaceWindow::panChanged);
        disconnect(srcFader, &FaderWidget::muteChanged, this, &MixFaceWindow::muteClicked);
        disconnect(srcFader, &FaderWidget::soloChanged, this, &MixFaceWindow::soloClicked);
        disconnect(srcFader, &FaderWidget::srcClicked, this, &MixFaceWindow::buttonSrcClicked);
        disconnect(srcFader, &FaderWidget::eqClicked, this, &MixFaceWindow::buttonEqClicked);
        disconnect(srcFader, &FaderWidget::dynClicked, this, &MixFaceWindow::buttonDynClicked);
        disconnect(srcFader, &FaderWidget::iconButtonClicked, this, &MixFaceWindow::iconButtonClicked);
        delete srcFader;
        srcFader = nullptr;
    }

    if (m_mode != mEq){
        if(idx!=70&&idx!=71&&(idx<48||idx>63)){
            srcFader = new FaderWidget(dpiRatio, mf_fonts, mf_metersTimer, debug);
            mf_demoTimer->AddFader(srcFader);
            srcLayout->addWidget(srcFader);
            srcFader->setChannelNativeName(QString::fromStdString(channelNameFromIdx(idx)));
            srcFader->setFaderType(mf_types.getFaderType(idx));
            srcFader->setProperty("idx", idx);
            connect(srcFader, &FaderWidget::faderChanged, this, &MixFaceWindow::faderChanged);
            connect(srcFader, &FaderWidget::panChanged, this, &MixFaceWindow::panChanged);
            connect(srcFader, &FaderWidget::muteChanged, this, &MixFaceWindow::muteClicked);
            connect(srcFader, &FaderWidget::soloChanged, this, &MixFaceWindow::soloClicked);
            connect(srcFader, &FaderWidget::srcClicked, this, &MixFaceWindow::buttonSrcClicked);
            connect(srcFader, &FaderWidget::eqClicked, this, &MixFaceWindow::buttonEqClicked);
            connect(srcFader, &FaderWidget::dynClicked, this, &MixFaceWindow::buttonDynClicked);
            connect(srcFader, &FaderWidget::iconButtonClicked, this, &MixFaceWindow::iconButtonClicked);
        }

        if (eqWidget == nullptr)
            eqWidget = new EqualizerWidget(dpiRatio, mf_fonts);
        else
            eqWidget->setVisible(true);
        srcLayout->addWidget(eqWidget);
        eqWidget->setIdx(idx);

        scrollArea->setHidden(true);
        bar->setHidden(true);
        srcArea->setVisible(true);
        m_mode = mEq;
    } else {
        srcArea->setHidden(true);
        scrollArea->setVisible(true);
        bar->setVisible(true);
        m_mode = mCtrl;
    }
}

void MixFaceWindow::buttonDynClicked(){
    int idx = reinterpret_cast<QObject *>(sender())->property("idx").toInt();

    if (srcWidget != nullptr) { srcWidget->setHidden(true); }
    if (eqWidget != nullptr) { eqWidget->setHidden(true); }
    if (dynWidget != nullptr) { dynWidget->setHidden(true); }

    if (srcFader != nullptr) {
        mf_demoTimer->RemoveFader(srcFader);
        disconnect(srcFader, &FaderWidget::faderChanged, this, &MixFaceWindow::faderChanged);
        disconnect(srcFader, &FaderWidget::panChanged, this, &MixFaceWindow::panChanged);
        disconnect(srcFader, &FaderWidget::muteChanged, this, &MixFaceWindow::muteClicked);
        disconnect(srcFader, &FaderWidget::soloChanged, this, &MixFaceWindow::soloClicked);
        disconnect(srcFader, &FaderWidget::srcClicked, this, &MixFaceWindow::buttonSrcClicked);
        disconnect(srcFader, &FaderWidget::eqClicked, this, &MixFaceWindow::buttonEqClicked);
        disconnect(srcFader, &FaderWidget::dynClicked, this, &MixFaceWindow::buttonDynClicked);
        disconnect(srcFader, &FaderWidget::iconButtonClicked, this, &MixFaceWindow::iconButtonClicked);
        delete srcFader;
        srcFader = nullptr;
    }

    if (m_mode != mDyn){
        if(idx!=70&&idx!=71&&(idx<48||idx>63)){
            srcFader = new FaderWidget(dpiRatio, mf_fonts,
                                       mf_metersTimer, debug);
            mf_demoTimer->AddFader(srcFader);
            srcLayout->addWidget(srcFader);
            srcFader->setChannelNativeName(
                        QString::fromStdString(channelNameFromIdx(idx))
                        );
            srcFader->setFaderType(mf_types.getFaderType(idx));
            srcFader->setProperty("idx", idx);
            connect(srcFader, &FaderWidget::faderChanged,
                    this, &MixFaceWindow::faderChanged);
            connect(srcFader, &FaderWidget::panChanged,
                    this, &MixFaceWindow::panChanged);
            connect(srcFader, &FaderWidget::muteChanged,
                    this, &MixFaceWindow::muteClicked);
            connect(srcFader, &FaderWidget::soloChanged,
                    this, &MixFaceWindow::soloClicked);
            connect(srcFader, &FaderWidget::srcClicked,
                    this, &MixFaceWindow::buttonSrcClicked);
            connect(srcFader, &FaderWidget::eqClicked,
                    this, &MixFaceWindow::buttonEqClicked);
            connect(srcFader, &FaderWidget::dynClicked,
                    this, &MixFaceWindow::buttonDynClicked);
            connect(srcFader, &FaderWidget::iconButtonClicked,
                    this, &MixFaceWindow::iconButtonClicked);
        }

        if (dynWidget == nullptr) {
            dynWidget = new DynamicsWidget(dpiRatio, mf_fonts);

            connect(dynWidget, &DynamicsWidget::compOnOffChanged,
                    this, &MixFaceWindow::compOnOffChanged);
            connect(dynWidget, &DynamicsWidget::compTresholdChanged,
                    this, &MixFaceWindow::compTresholdChanged);
            connect(dynWidget, &DynamicsWidget::compRatioChanged,
                    this, &MixFaceWindow::compRatioChanged);
            connect(dynWidget, &DynamicsWidget::compMixChanged,
                    this, &MixFaceWindow::compMixChanged);
            connect(dynWidget, &DynamicsWidget::compGainChanged,
                    this, &MixFaceWindow::compGainChanged);
            connect(dynWidget, &DynamicsWidget::compKneeChanged,
                    this, &MixFaceWindow::compKneeChanged);
            connect(dynWidget, &DynamicsWidget::compModeCompExpChanged,
                    this, &MixFaceWindow::compModeCompExpChanged);
            connect(dynWidget, &DynamicsWidget::compEnvLinLogChanged,
                    this, &MixFaceWindow::compEnvLinLogChanged);
            connect(dynWidget, &DynamicsWidget::compDetPeakRmsChanged,
                    this, &MixFaceWindow::compDetPeakRmsChanged);
            connect(dynWidget, &DynamicsWidget::compAttackChanged,
                    this, &MixFaceWindow::compAttackChanged);
            connect(dynWidget, &DynamicsWidget::compHoldChanged,
                    this, &MixFaceWindow::compHoldChanged);
            connect(dynWidget, &DynamicsWidget::compReleaseChanged,
                    this, &MixFaceWindow::compReleaseChanged);
            connect(dynWidget, &DynamicsWidget::compAutoTimeChanged,
                    this, &MixFaceWindow::compAutoTimeChanged);
            connect(dynWidget, &DynamicsWidget::compKeySourceChanged,
                    this, &MixFaceWindow::compKeySourceChanged);
            connect(dynWidget, &DynamicsWidget::compFilterChanged,
                    this, &MixFaceWindow::compFilterChanged);
            connect(dynWidget, &DynamicsWidget::compFilterSoloChanged,
                    this, &MixFaceWindow::compFilterSoloChanged);
            connect(dynWidget, &DynamicsWidget::compFilterTypeChanged,
                    this, &MixFaceWindow::compFilterTypeChanged);
            connect(dynWidget, &DynamicsWidget::compFilterFrequencyChanged,
                    this, &MixFaceWindow::compFilterFrequencyChanged);

            connect(dynWidget, &DynamicsWidget::gateOnOffChanged,
                    this, &MixFaceWindow::gateOnOffChanged);
            connect(dynWidget, &DynamicsWidget::gateTresholdChanged,
                    this, &MixFaceWindow::gateTresholdChanged);
            connect(dynWidget, &DynamicsWidget::gateRangeChanged,
                    this, &MixFaceWindow::gateRangeChanged);
            connect(dynWidget, &DynamicsWidget::gateModeChanged,
                    this, &MixFaceWindow::gateModeChanged);
            connect(dynWidget, &DynamicsWidget::gateAttackChanged,
                    this, &MixFaceWindow::gateAttackChanged);
            connect(dynWidget, &DynamicsWidget::gateHoldChanged,
                    this, &MixFaceWindow::gateHoldChanged);
            connect(dynWidget, &DynamicsWidget::gateReleaseChanged,
                    this, &MixFaceWindow::gateReleaseChanged);
            connect(dynWidget, &DynamicsWidget::gateKeySourceChanged,
                    this, &MixFaceWindow::gateKeySourceChanged);
            connect(dynWidget, &DynamicsWidget::gateFilterChanged,
                    this, &MixFaceWindow::gateFilterChanged);
            connect(dynWidget, &DynamicsWidget::gateFilterSoloChanged,
                    this, &MixFaceWindow::gateFilterSoloChanged);
            connect(dynWidget, &DynamicsWidget::gateFilterTypeChanged,
                    this, &MixFaceWindow::gateFilterTypeChanged);
            connect(dynWidget, &DynamicsWidget::gateFilterFrequencyChanged,
                    this, &MixFaceWindow::gateFilterFrequencyChanged);
        } else
            dynWidget->setVisible(true);
        srcLayout->addWidget(dynWidget);
        dynWidget->setIdx(idx);

        scrollArea->setHidden(true);
        bar->setHidden(true);
        srcArea->setVisible(true);
        m_mode = mDyn;

        if (connected)
            mf_library->threadSendDynRequestsMessages(idx);

        dynWidget->compOnOffRecieved(mf_library->db->dynon[idx]);
        dynWidget->compTresholdRecieved(mf_library->db->dynthr[idx]);
        dynWidget->compRatioRecieved(mf_library->db->dynratio[idx]);
        dynWidget->compMixRecieved(mf_library->db->dynmix[idx]);
        dynWidget->compGainRecieved(mf_library->db->dynmgain[idx]);
        dynWidget->compAttackRecieved(mf_library->db->dynattack[idx]);
        dynWidget->compHoldRecieved(mf_library->db->dynhold[idx]);
        dynWidget->compReleaseRecieved(mf_library->db->dynrelease[idx]);
        dynWidget->compModeCompExpRecieved(mf_library->db->dynmode[idx]);
        dynWidget->compKneeRecieved(mf_library->db->dynknee[idx]);
        dynWidget->compEnvLinLogRecieved(mf_library->db->dynenv[idx]);
        dynWidget->compDetPeakRmsRecieved(mf_library->db->dyndet[idx]);
        dynWidget->compAutoTimeRecieved(mf_library->db->dynauto[idx]);
        dynWidget->compKeySourceRecieved(mf_library->db->dynkeysrc[idx]);
        dynWidget->compFilterRecieved(mf_library->db->dynfilteron[idx]);
        dynWidget->compFilterSoloRecieved(mf_library->db->keysolo);
        dynWidget->compFilterTypeRecieved(mf_library->db->dynfiltertype[idx]);
        dynWidget->compFilterFrequencyRecieved(mf_library->db->dynfilterf[idx]);

        dynWidget->gateOnOffRecieved(mf_library->db->gateon[idx]);
        dynWidget->gateTresholdRecieved(mf_library->db->gatethr[idx]);
        dynWidget->gateRangeRecieved(mf_library->db->gaterange[idx]);
        dynWidget->gateModeRecieved(mf_library->db->gatemode[idx]);
        dynWidget->gateAttackRecieved(mf_library->db->gateattack[idx]);
        dynWidget->gateHoldRecieved(mf_library->db->gatehold[idx]);
        dynWidget->gateReleaseRecieved(mf_library->db->gaterelease[idx]);
        dynWidget->gateKeySourceRecieved(mf_library->db->gatekeysrc[idx]);
        dynWidget->gateFilterRecieved(mf_library->db->gatefilteron[idx]);
        dynWidget->gateFilterSoloRecieved(mf_library->db->keysolo);
        dynWidget->gateFilterTypeRecieved(mf_library->db->gatefiltertype[idx]);
        dynWidget->gateFilterFrequencyRecieved(mf_library->db->gatefilterf[idx]);
    } else {
        srcArea->setHidden(true);
        scrollArea->setVisible(true);
        bar->setVisible(true);
        m_mode = mCtrl;
    }
}

void MixFaceWindow::compOnOffChanged(int value){
    int idx = dynWidget->getIdx();
    mf_library->db->dynon[idx] = value;
    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(dynon, chtype, chN, 0);
        mf_library->sendInt(msg.c_str(), value);
    }
}

void MixFaceWindow::compTresholdChanged(float value){
    int idx = dynWidget->getIdx();
    mf_library->db->dynthr[idx] = value;
    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(dynthr, chtype, chN, 0);
        mf_library->sendFloat(msg.c_str(), value);
    }
}

void MixFaceWindow::compRatioChanged(int value){
    int idx = dynWidget->getIdx();
    mf_library->db->dynratio[idx] = value;
    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(dynratio, chtype, chN, 0);
        mf_library->sendInt(msg.c_str(), value);
    }
}

void MixFaceWindow::compMixChanged(float value){
    int idx = dynWidget->getIdx();
    mf_library->db->dynmix[idx] = value;
    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(dynmix, chtype, chN, 0);
        mf_library->sendFloat(msg.c_str(), value);
    }
}

void MixFaceWindow::compGainChanged(float value){
    int idx = dynWidget->getIdx();
    mf_library->db->dynmgain[idx] = value;
    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(dynmgain, chtype, chN, 0);
        mf_library->sendFloat(msg.c_str(), value);
    }
}

void MixFaceWindow::compKneeChanged(float value){
    int idx = dynWidget->getIdx();
    mf_library->db->dynknee[idx] = value;
    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(dynknee, chtype, chN, 0);
        mf_library->sendFloat(msg.c_str(), value);
    }
}

void MixFaceWindow::compModeCompExpChanged(int value){
    int idx = dynWidget->getIdx();
    mf_library->db->dynmode[idx] = value;
    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(dynmode, chtype, chN, 0);
        mf_library->sendInt(msg.c_str(), value);
    }
}

void MixFaceWindow::compEnvLinLogChanged(int value){
    int idx = dynWidget->getIdx();
    mf_library->db->dynenv[idx] = value;
    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(dynenv, chtype, chN, 0);
        mf_library->sendInt(msg.c_str(), value);
    }
}

void MixFaceWindow::compDetPeakRmsChanged(int value){
    int idx = dynWidget->getIdx();
    mf_library->db->dyndet[idx] = value;
    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(dyndet, chtype, chN, 0);
        mf_library->sendInt(msg.c_str(), value);
    }
}

void MixFaceWindow::compAttackChanged(float value){
    int idx = dynWidget->getIdx();
    mf_library->db->dynattack[idx] = value;
    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(dynattack, chtype, chN, 0);
        mf_library->sendFloat(msg.c_str(), value);
    }
}

void MixFaceWindow::compHoldChanged(float value){
    int idx = dynWidget->getIdx();
    mf_library->db->dynhold[idx] = value;
    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(dynhold, chtype, chN, 0);
        mf_library->sendFloat(msg.c_str(), value);
    }
}

void MixFaceWindow::compReleaseChanged(float value){
    int idx = dynWidget->getIdx();
    mf_library->db->dynrelease[idx] = value;
    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(dynrelease, chtype, chN, 0);
        mf_library->sendFloat(msg.c_str(), value);
    }
}

void MixFaceWindow::compAutoTimeChanged(int value){
    int idx = dynWidget->getIdx();
    mf_library->db->dynauto[idx] = value;
    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(dynauto, chtype, chN, 0);
        mf_library->sendInt(msg.c_str(), value);
    }
}

void MixFaceWindow::compKeySourceChanged(int value){
    int idx = dynWidget->getIdx();
    mf_library->db->dynkeysrc[idx] = value;
    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(dynkeysrc, chtype, chN, 0);
        mf_library->sendInt(msg.c_str(), value);
    }
}

void MixFaceWindow::compFilterChanged(int value){
    int idx = dynWidget->getIdx();
    mf_library->db->dynfilteron[idx] = value;
    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(dynfilteron, chtype, chN, 0);
        mf_library->sendInt(msg.c_str(), value);
    }
}

void MixFaceWindow::compFilterSoloChanged(int value){
    mf_library->db->keysolo = value;
    if (connected) {
        mf_library->sendInt(msgTypeStr.keysolo, value);
    }
}

void MixFaceWindow::compFilterTypeChanged(int value){
    int idx = dynWidget->getIdx();
    mf_library->db->dynfiltertype[idx] = value;
    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(dynfiltertype, chtype, chN, 0);
        mf_library->sendInt(msg.c_str(), value);
    }
}

void MixFaceWindow::compFilterFrequencyChanged(float value){
    int idx = dynWidget->getIdx();
    mf_library->db->dynfilterf[idx] = value;
    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(dynfilterf, chtype, chN, 0);
        mf_library->sendFloat(msg.c_str(), value);
    }
}

void MixFaceWindow::gateOnOffChanged(int value){
    int idx = dynWidget->getIdx();
    mf_library->db->gateon[idx] = value;
    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(gateon, chtype, chN, 0);
        mf_library->sendInt(msg.c_str(), value);
    }
}

void MixFaceWindow::gateTresholdChanged(float value){
    int idx = dynWidget->getIdx();
    mf_library->db->gatethr[idx] = value;
    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(gatethr, chtype, chN, 0);
        mf_library->sendFloat(msg.c_str(), value);
    }
}

void MixFaceWindow::gateRangeChanged(float value){
    int idx = dynWidget->getIdx();
    mf_library->db->gaterange[idx] = value;
    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(gaterange, chtype, chN, 0);
        mf_library->sendFloat(msg.c_str(), value);
    }
}

void MixFaceWindow::gateModeChanged(int value){
    int idx = dynWidget->getIdx();
    mf_library->db->gatemode[idx] = value;
    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(gatemode, chtype, chN, 0);
        mf_library->sendInt(msg.c_str(), value);
    }
}

void MixFaceWindow::gateAttackChanged(float value){
    int idx = dynWidget->getIdx();
    mf_library->db->gateattack[idx] = value;
    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(gateattack, chtype, chN, 0);
        mf_library->sendFloat(msg.c_str(), value);
    }
}

void MixFaceWindow::gateHoldChanged(float value){
    int idx = dynWidget->getIdx();
    mf_library->db->gatehold[idx] = value;
    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(gatehold, chtype, chN, 0);
        mf_library->sendFloat(msg.c_str(), value);
    }
}

void MixFaceWindow::gateReleaseChanged(float value){
    int idx = dynWidget->getIdx();
    mf_library->db->gaterelease[idx] = value;
    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(gaterelease, chtype, chN, 0);
        mf_library->sendFloat(msg.c_str(), value);
    }
}

void MixFaceWindow::gateKeySourceChanged(int value){
    int idx = dynWidget->getIdx();
    mf_library->db->gatekeysrc[idx] = value;
    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(gatekeysrc, chtype, chN, 0);
        mf_library->sendInt(msg.c_str(), value);
    }
}

void MixFaceWindow::gateFilterChanged(int value){
    int idx = dynWidget->getIdx();
    mf_library->db->gatefilteron[idx] = value;
    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(gatefilteron, chtype, chN, 0);
        mf_library->sendInt(msg.c_str(), value);
    }
}

void MixFaceWindow::gateFilterSoloChanged(int value){
    mf_library->db->keysolo = value;
    if (connected) {
        mf_library->sendInt(msgTypeStr.keysolo, value);
    }
}

void MixFaceWindow::gateFilterTypeChanged(int value){
    int idx = dynWidget->getIdx();
    mf_library->db->gatefiltertype[idx] = value;
    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(gatefiltertype, chtype, chN, 0);
        mf_library->sendInt(msg.c_str(), value);
    }
}

void MixFaceWindow::gateFilterFrequencyChanged(float value){
    int idx = dynWidget->getIdx();
    mf_library->db->gatefilterf[idx] = value;
    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(gatefilterf, chtype, chN, 0);
        mf_library->sendFloat(msg.c_str(), value);
    }
}

void MixFaceWindow::panChanged(float value)
{
    int idx = reinterpret_cast<QObject *>(sender())->property("idx").toInt();
    MessageType mtype = pan;
    int send = 0;
    if (currentIdy > 0 && currentIdy < 17 && idx < 48) {
        mtype = sendpan;
        send = currentIdy;
        mf_library->db->sendpan[idx][currentIdy] = value;
    } else {
        mf_library->db->pan[idx] = value;
    }

    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(mtype, chtype, chN, send);
        mf_library->sendFloat(msg.c_str(),value);
    }

    if ((idx>47&&idx<65)||idx==70||idx==71) {
        threadValueChanged(pan,idx,currentIdy);
    }
}

void MixFaceWindow::faderChanged(float value)
{
    value = value/10000;
    int idx = reinterpret_cast<QObject *>(sender())->property("idx").toInt();
    MessageType mtype = fader;
    int send = 0;
    if (currentIdy == 0 || idx > 47)
        mf_library->db->fader[idx] = value;
    else if (currentIdy > 0 && currentIdy < 17 && idx < 48) {
        send = currentIdy;
        mtype = sendlevel;
        mf_library->db->sendlevel[idx][currentIdy] = value;
    } else if (currentIdy == 17 && idx < 48) {
        mtype = mlevel;
        mf_library->db->mlevel[idx] = value;
    }

    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(mtype, chtype, chN, send);
        mf_library->sendFloat(msg.c_str(), value);
    }

    if ((idx>47&&idx<65)||idx==70||idx==71)
        threadValueChanged(fader,idx,currentIdy);
}

void MixFaceWindow::muteClicked(int value)
{
    int idx = reinterpret_cast<QObject *>(sender())->property("idx").toInt();
    int send = 0;
    MessageType mtype = on;
    if (currentIdy == 0 || idx > 47)
        mf_library->db->on[idx] = value;
    else if (currentIdy > 0 && currentIdy < 17 && idx < 48) {
        send = currentIdy;
        mtype = sendon;
        mf_library->db->sendon[idx][currentIdy] = value;
    } else if (currentIdy == 17 && idx < 48) {
        mtype = monoon;
        mf_library->db->monoon[idx] = value;
    }

    if (connected) {
        ChannelType chtype = getChannelTypeFromIdx(idx);
        int chN = getChannelNumberFromIdx(idx);
        string msg = getOscAddress(mtype, chtype, chN, send);
        mf_library->sendInt(msg.c_str(), value);
    }

    if ((idx>47&&idx<65)||idx==70||idx==71) {
        threadValueChanged(on,idx,currentIdy);
    }
}

void MixFaceWindow::soloClicked(int value)
{
    int idx = reinterpret_cast<QObject *>(sender())->property("idx").toInt();
    mf_library->db->solo[idx] = value;
    if (connected) for (int ids=0;ids<80;ids++)
    {
        QString msg;
        if (ids<10) msg = ("/-stat/solosw/0" + QString::number(ids+1));
        else msg = ("/-stat/solosw/" + QString::number(ids+1));
        int value = mf_library->db->solo[ids];
        QByteArray oscAddressArray = msg.toLatin1();
        const char *oscAddress = oscAddressArray;
        mf_library->sendInt(oscAddress,value);
    }
    if ((idx>47&&idx<65)||idx==70||idx==71) {
        threadValueChanged(solo,idx,currentIdy);
    }
}

void MixFaceWindow::windowRenew() {
    for (int idx=0;idx<80;idx++) {
        disconnect(scrollWidget[idx], &FaderWidget::faderChanged, this, &MixFaceWindow::faderChanged);
        if (currentIdy==0||idx>47) {
            scrollWidget[idx]->setFaderValue(mf_library->db->fader[idx]);
        }
        if (currentIdy==17&&idx<48) {
            scrollWidget[idx]->setFaderValue(mf_library->db->fader[idx]);
        }
        if (currentIdy!=17&&currentIdy!=0&&idx<48) {
            scrollWidget[idx]->setFaderValue(mf_library->db->fader[idx]);
        }
        connect(scrollWidget[idx], &FaderWidget::faderChanged, this, &MixFaceWindow::faderChanged);
    }
}

void MixFaceWindow::slotValueChanged(int imtype, int idx, int idy) {
    returnThread->valueSlot(imtype, idx, idy);
}

void MixFaceWindow::threadValueChanged(int imtype, int idx, int idy) {
    MessageType mtype = MessageType(imtype);
    switch (mtype) {
    case stereoon:
        break;
    case monoon:
        if(currentIdy==17&&idx<48) {
            if (mf_library->db->monoon[idx]==0)
                scrollWidget[idx]->setMute(true);
            else
                scrollWidget[idx]->setMute(false);
        }
        break;
    case mlevel:
        if(currentIdy==17&&idx<48) {
            disconnect(scrollWidget[idx], &FaderWidget::faderChanged, this, &MixFaceWindow::faderChanged);
            scrollWidget[idx]->setFaderValue(mf_library->db->mlevel[idx]);
            connect(scrollWidget[idx], &FaderWidget::faderChanged, this, &MixFaceWindow::faderChanged);
        }
        break;
    case fader:
        if(currentIdy==0||idx>47) {
            disconnect(scrollWidget[idx], &FaderWidget::faderChanged, this, &MixFaceWindow::faderChanged);
            scrollWidget[idx]->setFaderValue(mf_library->db->fader[idx]);
            connect(scrollWidget[idx], &FaderWidget::faderChanged, this, &MixFaceWindow::faderChanged);
            if ((idx>47&&idx<64)||idx==70||idx==71) {
                disconnect(mainWidget[idx], &FaderWidget::faderChanged, this, &MixFaceWindow::faderChanged);
                mainWidget[idx]->setFaderValue(mf_library->db->fader[idx]);
                connect(mainWidget[idx], &FaderWidget::faderChanged, this, &MixFaceWindow::faderChanged);
            }
        }
        break;
    case pan:
        if(currentIdy==0||idx>47) {
            disconnect(scrollWidget[idx], &FaderWidget::panChanged, this, &MixFaceWindow::panChanged);
            scrollWidget[idx]->setPanValue(mf_library->db->pan[idx]);
            connect(scrollWidget[idx], &FaderWidget::panChanged, this, &MixFaceWindow::panChanged);
            if ((idx>47&&idx<64)||idx==70||idx==71) {
                disconnect(mainWidget[idx], &FaderWidget::panChanged, this, &MixFaceWindow::panChanged);
                mainWidget[idx]->setPanValue(mf_library->db->pan[idx]);
                connect(mainWidget[idx], &FaderWidget::panChanged, this, &MixFaceWindow::panChanged);
            }
        }
        break;
    case on:
        if(currentIdy==0||idx>47) {
            if (mf_library->db->on[idx]==0) {
                scrollWidget[idx]->setMute(true);
                if ((idx>47&&idx<64)||idx==70||idx==71) {
                    mainWidget[idx]->setMute(true);
                }
            } else {
                scrollWidget[idx]->setMute(false);
                if ((idx>47&&idx<64)||idx==70||idx==71) {
                    mainWidget[idx]->setMute(false);
                }
            }
        }
        break;
    case solo:
        if(mf_library->db->solo[idx]==0) {
            scrollWidget[idx]->setSolo(false);
            if((idx<64&&idx>47)||idx==70||idx==71)
                mainWidget[idx]->setSolo(false);
        } else {
            scrollWidget[idx]->setSolo(true);
            if((idx<64&&idx>47)||idx==70||idx==71)
                mainWidget[idx]->setSolo(true);
        }
        break;
    case keysolo:
        if (m_mode == mDyn && dynWidget != nullptr)
            dynWidget->compFilterSoloRecieved(mf_library->db->keysolo);
        break;
    case chlink:
        break;
    case auxlink:
        break;
    case buslink:
        break;
    case mtxlink:
        break;
    case phantom:
        break;
    case invert:
        break;
    case source:
        break;
    case gain:
        break;
    case trim:
        break;
    case hpf:
        break;
    case hpon:
        break;
    case delayon:
        break;
    case delaytime:
        break;
    case inserton:
        break;
    case insertsel:
        break;
    case insertpos:
        break;
    case gateon:
        if (m_mode == mDyn && dynWidget != nullptr && dynWidget->getIdx() == idx)
            dynWidget->gateOnOffRecieved(mf_library->db->gateon[idx]);
        break;
    case gatethr:
        if (m_mode == mDyn && dynWidget != nullptr && dynWidget->getIdx() == idx)
            dynWidget->gateTresholdRecieved(mf_library->db->gatethr[idx]);
        break;
    case gaterange:
        if (m_mode == mDyn && dynWidget != nullptr && dynWidget->getIdx() == idx)
            dynWidget->gateRangeRecieved(mf_library->db->gaterange[idx]);
        break;
    case gatemode:
        if (m_mode == mDyn && dynWidget != nullptr && dynWidget->getIdx() == idx)
            dynWidget->gateModeRecieved(mf_library->db->gatemode[idx]);
        break;
    case gateattack:
        if (m_mode == mDyn && dynWidget != nullptr && dynWidget->getIdx() == idx)
            dynWidget->gateAttackRecieved(mf_library->db->gateattack[idx]);
        break;
    case gatehold:
        if (m_mode == mDyn && dynWidget != nullptr && dynWidget->getIdx() == idx)
            dynWidget->gateHoldRecieved(mf_library->db->gatehold[idx]);
        break;
    case gaterelease:
        if (m_mode == mDyn && dynWidget != nullptr && dynWidget->getIdx() == idx)
            dynWidget->gateReleaseRecieved(mf_library->db->gaterelease[idx]);
        break;
    case gatekeysrc:
        if (m_mode == mDyn && dynWidget != nullptr && dynWidget->getIdx() == idx)
            dynWidget->gateKeySourceRecieved(mf_library->db->gatekeysrc[idx]);
        break;
    case gatefilteron:
        if (m_mode == mDyn && dynWidget != nullptr && dynWidget->getIdx() == idx)
            dynWidget->gateFilterRecieved(mf_library->db->gatefilteron[idx]);
        break;
    case gatefiltertype:
        if (m_mode == mDyn && dynWidget != nullptr && dynWidget->getIdx() == idx)
            dynWidget->gateFilterTypeRecieved(mf_library->db->gatefiltertype[idx]);
        break;
    case gatefilterf:
        if (m_mode == mDyn && dynWidget != nullptr && dynWidget->getIdx() == idx)
            dynWidget->gateFilterFrequencyRecieved(mf_library->db->gatefilterf[idx]);
        break;
    case dynon:
        if (m_mode == mDyn && dynWidget != nullptr && dynWidget->getIdx() == idx)
            dynWidget->compOnOffRecieved(mf_library->db->dynon[idx]);
        break;
    case dynthr:
        if (m_mode == mDyn && dynWidget != nullptr && dynWidget->getIdx() == idx)
            dynWidget->compTresholdRecieved(mf_library->db->dynthr[idx]);
        break;
    case dynratio:
        if (m_mode == mDyn && dynWidget != nullptr && dynWidget->getIdx() == idx)
            dynWidget->compRatioRecieved(mf_library->db->dynratio[idx]);
        break;
    case dynmix:
        if (m_mode == mDyn && dynWidget != nullptr && dynWidget->getIdx() == idx)
            dynWidget->compMixRecieved(mf_library->db->dynmix[idx]);
        break;
    case dynmgain:
        if (m_mode == mDyn && dynWidget != nullptr && dynWidget->getIdx() == idx)
            dynWidget->compGainRecieved(mf_library->db->dynmgain[idx]);
        break;
    case dynattack:
        if (m_mode == mDyn && dynWidget != nullptr && dynWidget->getIdx() == idx)
            dynWidget->compAttackRecieved(mf_library->db->dynattack[idx]);
        break;
    case dynhold:
        if (m_mode == mDyn && dynWidget != nullptr && dynWidget->getIdx() == idx)
            dynWidget->compHoldRecieved(mf_library->db->dynhold[idx]);
        break;
    case dynrelease:
        if (m_mode == mDyn && dynWidget != nullptr && dynWidget->getIdx() == idx)
            dynWidget->compReleaseRecieved(mf_library->db->dynrelease[idx]);
        break;
    case dynmode:
        if (m_mode == mDyn && dynWidget != nullptr && dynWidget->getIdx() == idx)
            dynWidget->compModeCompExpRecieved(mf_library->db->dynmode[idx]);
        break;
    case dynknee:
        if (m_mode == mDyn && dynWidget != nullptr && dynWidget->getIdx() == idx)
            dynWidget->compKneeRecieved(mf_library->db->dynknee[idx]);
        break;
    case dynenv:
        if (m_mode == mDyn && dynWidget != nullptr && dynWidget->getIdx() == idx)
            dynWidget->compEnvLinLogRecieved(mf_library->db->dynenv[idx]);
        break;
    case dyndet:
        if (m_mode == mDyn && dynWidget != nullptr && dynWidget->getIdx() == idx)
            dynWidget->compDetPeakRmsRecieved(mf_library->db->dyndet[idx]);
        break;
    case dynauto:
        if (m_mode == mDyn && dynWidget != nullptr && dynWidget->getIdx() == idx)
            dynWidget->compAutoTimeRecieved(mf_library->db->dynauto[idx]);
        break;
    case dynkeysrc:
        if (m_mode == mDyn && dynWidget != nullptr && dynWidget->getIdx() == idx)
            dynWidget->compKeySourceRecieved(mf_library->db->dynkeysrc[idx]);
        break;
    case dynfilteron:
        if (m_mode == mDyn && dynWidget != nullptr && dynWidget->getIdx() == idx)
            dynWidget->compFilterRecieved(mf_library->db->dynfilteron[idx]);
        break;
    case dynfiltertype:
        if (m_mode == mDyn && dynWidget != nullptr && dynWidget->getIdx() == idx)
            dynWidget->compFilterTypeRecieved(mf_library->db->dynfiltertype[idx]);
        break;
    case dynfilterf:
        if (m_mode == mDyn && dynWidget != nullptr && dynWidget->getIdx() == idx)
            dynWidget->compFilterFrequencyRecieved(mf_library->db->dynfilterf[idx]);
        break;
    case eqon:
        if (m_mode == mEq && eqWidget != nullptr && eqWidget->getIdx() == idx)
            eqWidget->eqOnOffRecieved(mf_library->db->eqon[idx]);
        break;
    case eq1type:
        if (m_mode == mEq && eqWidget != nullptr && eqWidget->getIdx() == idx)
            eqWidget->eqBandTypeRecieved(0, mf_library->db->eq1type[idx]);
        break;
    case eq1g:
        if (m_mode == mEq && eqWidget != nullptr && eqWidget->getIdx() == idx)
            eqWidget->eqBandGainRecieved(0, mf_library->db->eq1g[idx]);
        break;
    case eq1f:
        if (m_mode == mEq && eqWidget != nullptr && eqWidget->getIdx() == idx)
            eqWidget->eqBandFreqRecieved(0, mf_library->db->eq1f[idx]);
        break;
    case eq1q:
        if (m_mode == mEq && eqWidget != nullptr && eqWidget->getIdx() == idx)
            eqWidget->eqBandQualRecieved(0, mf_library->db->eq1q[idx]);
        break;
    case eq2type:
        if (m_mode == mEq && eqWidget != nullptr && eqWidget->getIdx() == idx)
            eqWidget->eqBandTypeRecieved(1, mf_library->db->eq2type[idx]);
        break;
    case eq2g:
        if (m_mode == mEq && eqWidget != nullptr && eqWidget->getIdx() == idx)
            eqWidget->eqBandGainRecieved(1, mf_library->db->eq2g[idx]);
        break;
    case eq2f:
        if (m_mode == mEq && eqWidget != nullptr && eqWidget->getIdx() == idx)
            eqWidget->eqBandFreqRecieved(1, mf_library->db->eq2f[idx]);
        break;
    case eq2q:
        if (m_mode == mEq && eqWidget != nullptr && eqWidget->getIdx() == idx)
            eqWidget->eqBandQualRecieved(1, mf_library->db->eq2q[idx]);
        break;
    case eq3type:
        if (m_mode == mEq && eqWidget != nullptr && eqWidget->getIdx() == idx)
            eqWidget->eqBandTypeRecieved(2, mf_library->db->eq3type[idx]);
        break;
    case eq3g:
        if (m_mode == mEq && eqWidget != nullptr && eqWidget->getIdx() == idx)
            eqWidget->eqBandGainRecieved(2, mf_library->db->eq3g[idx]);
        break;
    case eq3f:
        if (m_mode == mEq && eqWidget != nullptr && eqWidget->getIdx() == idx)
            eqWidget->eqBandFreqRecieved(2, mf_library->db->eq3f[idx]);
        break;
    case eq3q:
        if (m_mode == mEq && eqWidget != nullptr && eqWidget->getIdx() == idx)
            eqWidget->eqBandQualRecieved(2, mf_library->db->eq3q[idx]);
        break;
    case eq4type:
        if (m_mode == mEq && eqWidget != nullptr && eqWidget->getIdx() == idx)
            eqWidget->eqBandTypeRecieved(3, mf_library->db->eq4type[idx]);
        break;
    case eq4g:
        if (m_mode == mEq && eqWidget != nullptr && eqWidget->getIdx() == idx)
            eqWidget->eqBandGainRecieved(3, mf_library->db->eq4g[idx]);
        break;
    case eq4f:
        if (m_mode == mEq && eqWidget != nullptr && eqWidget->getIdx() == idx)
            eqWidget->eqBandFreqRecieved(3, mf_library->db->eq4f[idx]);
        break;
    case eq4q:
        if (m_mode == mEq && eqWidget != nullptr && eqWidget->getIdx() == idx)
            eqWidget->eqBandQualRecieved(3, mf_library->db->eq4q[idx]);
        break;
    case eq5type:
        if (m_mode == mEq && eqWidget != nullptr && eqWidget->getIdx() == idx)
            eqWidget->eqBandTypeRecieved(4, mf_library->db->eq5type[idx]);
        break;
    case eq5g:
        if (m_mode == mEq && eqWidget != nullptr && eqWidget->getIdx() == idx)
            eqWidget->eqBandGainRecieved(4, mf_library->db->eq5g[idx]);
        break;
    case eq5f:
        if (m_mode == mEq && eqWidget != nullptr && eqWidget->getIdx() == idx)
            eqWidget->eqBandFreqRecieved(4, mf_library->db->eq5f[idx]);
        break;
    case eq5q:
        if (m_mode == mEq && eqWidget != nullptr && eqWidget->getIdx() == idx)
            eqWidget->eqBandQualRecieved(4, mf_library->db->eq5q[idx]);
        break;
    case eq6type:
        if (m_mode == mEq && eqWidget != nullptr && eqWidget->getIdx() == idx)
            eqWidget->eqBandTypeRecieved(5, mf_library->db->eq6type[idx]);
        break;
    case eq6g:
        if (m_mode == mEq && eqWidget != nullptr && eqWidget->getIdx() == idx)
            eqWidget->eqBandGainRecieved(5, mf_library->db->eq6g[idx]);
        break;
    case eq6f:
        if (m_mode == mEq && eqWidget != nullptr && eqWidget->getIdx() == idx)
            eqWidget->eqBandFreqRecieved(5, mf_library->db->eq6f[idx]);
        break;
    case eq6q:
        if (m_mode == mEq && eqWidget != nullptr && eqWidget->getIdx() == idx)
            eqWidget->eqBandQualRecieved(5, mf_library->db->eq6q[idx]);
        break;
    case sendlevel:
        if(currentIdy==idy&&idx<48) {
            disconnect(scrollWidget[idx], &FaderWidget::faderChanged, this, &MixFaceWindow::faderChanged);
            scrollWidget[idx]->setFaderValue(mf_library->db->sendlevel[idx][idy]);
            connect(scrollWidget[idx], &FaderWidget::faderChanged, this, &MixFaceWindow::faderChanged);
        }
        break;
    case sendpan:
        break;
    case sendpanfollow:
        break;
    case sendtype:
        break;
    case sendon:
        if(currentIdy==idy&&idx<48) {
            if (mf_library->db->sendon[idx][idy]==0)
                scrollWidget[idx]->setMute(true);
            else
                scrollWidget[idx]->setMute(false);
        }
        break;
    case configcolor:
        scrollWidget[idx]->setColor(mf_library->db->configcolor[idx]);
        if ((idx>47&&idx<64)||idx==70||idx==71) {
            mainWidget[idx]->setColor(mf_library->db->configcolor[idx]);
        } else if (srcArea->isVisible() && srcFader->property("idx").toInt() == idx) {
            srcFader->setColor(mf_library->db->configcolor[idx]);
        }
        break;
    case configicon:
        scrollWidget[idx]->setLogo(mf_library->db->configicon[idx]);
        if ((idx>47&&idx<64)||idx==70||idx==71) {
            mainWidget[idx]->setLogo(mf_library->db->configicon[idx]);
        } else if (srcArea->isVisible() && srcFader->property("idx").toInt() == idx) {
            srcFader->setLogo(mf_library->db->configicon[idx]);
        }
        break;
    case configname:
        if (QString::fromStdString(mf_library->db->configname[idx]).length() <= 0)
            QString::fromStdString(mf_library->db->configname[idx]) = QString::fromStdString(channelNameFromIdx(idx));
        scrollWidget[idx]->setName(QString::fromStdString(mf_library->db->configname[idx]));
        if ((idx>47&&idx<64)||idx==70||idx==71) {
            mainWidget[idx]->setName(QString::fromStdString(mf_library->db->configname[idx]));
        } else if (srcArea->isVisible() && srcFader->property("idx").toInt() == idx) {
            srcFader->setName(QString::fromStdString(mf_library->db->configname[idx]));
        }
        break;
    case merror:
        debug->sendMessage(QString("Error change value!").toStdString(),0);
        break;
    }
}

void MixFaceWindow::slotMeter1(float *array){
    returnThread->meter1Slot(array);
}

void MixFaceWindow::threadMeter1(float *array){
    for(int i = 0;i < 32; i++){
        //32 input channels
        scrollWidget[i]->setMeter(array[i], array[i+1]);
        //32 gate gain reductions
        scrollWidget[i]->setGateMeter(array[i + 32]);
        //32 dynamics gain reductions
        scrollWidget[i]->setDynamicsMeter(array[i + 64]);
    }
}

void MixFaceWindow::slotMeter2(float *array){
    returnThread->meter2Slot(array);
}

void MixFaceWindow::threadMeter2(float *array){
    //meters
    //16 bus masters
    mainWidget[48]->setMeter(array[0], array[1]);
    mainWidget[49]->setMeter(array[1], array[0]);
    mainWidget[50]->setMeter(array[2], array[3]);
    mainWidget[51]->setMeter(array[3], array[2]);
    mainWidget[52]->setMeter(array[4], array[5]);
    mainWidget[53]->setMeter(array[5], array[4]);
    mainWidget[54]->setMeter(array[6], array[7]);
    mainWidget[55]->setMeter(array[7], array[6]);
    mainWidget[56]->setMeter(array[8], array[9]);
    mainWidget[57]->setMeter(array[9], array[8]);
    mainWidget[58]->setMeter(array[10], array[11]);
    mainWidget[59]->setMeter(array[11], array[10]);
    mainWidget[60]->setMeter(array[12], array[13]);
    mainWidget[61]->setMeter(array[13], array[12]);
    mainWidget[62]->setMeter(array[14], array[15]);
    mainWidget[63]->setMeter(array[15], array[14]);
    scrollWidget[48]->setMeter(array[0], array[1]);
    scrollWidget[49]->setMeter(array[1], array[0]);
    scrollWidget[50]->setMeter(array[2], array[3]);
    scrollWidget[51]->setMeter(array[3], array[2]);
    scrollWidget[52]->setMeter(array[4], array[5]);
    scrollWidget[53]->setMeter(array[5], array[4]);
    scrollWidget[54]->setMeter(array[6], array[7]);
    scrollWidget[55]->setMeter(array[7], array[6]);
    scrollWidget[56]->setMeter(array[8], array[9]);
    scrollWidget[57]->setMeter(array[9], array[8]);
    scrollWidget[58]->setMeter(array[10], array[11]);
    scrollWidget[59]->setMeter(array[11], array[10]);
    scrollWidget[60]->setMeter(array[12], array[13]);
    scrollWidget[61]->setMeter(array[13], array[12]);
    scrollWidget[62]->setMeter(array[14], array[15]);
    scrollWidget[63]->setMeter(array[15], array[14]);
    //matrix
    scrollWidget[64]->setMeter(array[16], array[17]);
    scrollWidget[65]->setMeter(array[17], array[16]);
    scrollWidget[66]->setMeter(array[18], array[19]);
    scrollWidget[67]->setMeter(array[19], array[18]);
    scrollWidget[68]->setMeter(array[20], array[21]);
    scrollWidget[69]->setMeter(array[21], array[20]);
    //main lr
    mainWidget[70]->setMeter(array[22], array[23]);
    scrollWidget[70]->setMeter(array[22], array[23]);
    //mono mc
    mainWidget[71]->setMeter(array[24], 0);
    scrollWidget[71]->setMeter(array[24], 0);

    //dynamics gain reduction
    //16 bus masters
    mainWidget[48]->setDynamicsMeter(array[25]);
    mainWidget[49]->setDynamicsMeter(array[26]);
    mainWidget[50]->setDynamicsMeter(array[27]);
    mainWidget[51]->setDynamicsMeter(array[28]);
    mainWidget[52]->setDynamicsMeter(array[29]);
    mainWidget[53]->setDynamicsMeter(array[30]);
    mainWidget[54]->setDynamicsMeter(array[31]);
    mainWidget[55]->setDynamicsMeter(array[32]);
    mainWidget[56]->setDynamicsMeter(array[33]);
    mainWidget[57]->setDynamicsMeter(array[34]);
    mainWidget[58]->setDynamicsMeter(array[35]);
    mainWidget[59]->setDynamicsMeter(array[36]);
    mainWidget[60]->setDynamicsMeter(array[37]);
    mainWidget[61]->setDynamicsMeter(array[38]);
    mainWidget[62]->setDynamicsMeter(array[39]);
    mainWidget[63]->setDynamicsMeter(array[40]);
    scrollWidget[48]->setDynamicsMeter(array[25]);
    scrollWidget[49]->setDynamicsMeter(array[26]);
    scrollWidget[50]->setDynamicsMeter(array[27]);
    scrollWidget[51]->setDynamicsMeter(array[28]);
    scrollWidget[52]->setDynamicsMeter(array[29]);
    scrollWidget[53]->setDynamicsMeter(array[30]);
    scrollWidget[54]->setDynamicsMeter(array[31]);
    scrollWidget[55]->setDynamicsMeter(array[32]);
    scrollWidget[56]->setDynamicsMeter(array[33]);
    scrollWidget[57]->setDynamicsMeter(array[34]);
    scrollWidget[58]->setDynamicsMeter(array[35]);
    scrollWidget[59]->setDynamicsMeter(array[36]);
    scrollWidget[60]->setDynamicsMeter(array[37]);
    scrollWidget[61]->setDynamicsMeter(array[38]);
    scrollWidget[62]->setDynamicsMeter(array[39]);
    scrollWidget[63]->setDynamicsMeter(array[40]);
    //matrix
    scrollWidget[64]->setDynamicsMeter(array[41]);
    scrollWidget[65]->setDynamicsMeter(array[42]);
    scrollWidget[66]->setDynamicsMeter(array[43]);
    scrollWidget[67]->setDynamicsMeter(array[44]);
    scrollWidget[68]->setDynamicsMeter(array[45]);
    scrollWidget[69]->setDynamicsMeter(array[46]);
    //main lr
    mainWidget[70]->setDynamicsMeter(array[47]);
    scrollWidget[70]->setDynamicsMeter(array[47]);
    //mono mc
    mainWidget[71]->setDynamicsMeter(array[48]);
    scrollWidget[71]->setDynamicsMeter(array[48]);
}

void MixFaceWindow::slotMeter3(float *array){
    returnThread->meter3Slot(array);
}

void MixFaceWindow::threadMeter3(float *array){
    //6 aux sends NOT USED FOR NOW
    //8 aux returns
    scrollWidget[32]->setMeter(array[6], array[7]);
    scrollWidget[33]->setMeter(array[7], array[6]);
    scrollWidget[34]->setMeter(array[8], array[9]);
    scrollWidget[35]->setMeter(array[9], array[8]);
    scrollWidget[36]->setMeter(array[10], array[11]);
    scrollWidget[37]->setMeter(array[11], array[10]);
    scrollWidget[38]->setMeter(array[12], array[13]);
    scrollWidget[39]->setMeter(array[13], array[12]);
    //4x2 st fx returns
    scrollWidget[40]->setMeter(array[14], array[15]);
    scrollWidget[41]->setMeter(array[15], array[14]);
    scrollWidget[42]->setMeter(array[16], array[17]);
    scrollWidget[43]->setMeter(array[17], array[16]);
    scrollWidget[44]->setMeter(array[18], array[19]);
    scrollWidget[45]->setMeter(array[19], array[18]);
    scrollWidget[46]->setMeter(array[20], array[21]);
    scrollWidget[47]->setMeter(array[21], array[20]);
}
