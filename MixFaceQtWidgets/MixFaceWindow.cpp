#include "MixFaceWindow.h"
#include <QtNetwork/QNetworkInterface>

int main(int argc, char *argv[])
{
    QApplication *mixFace = new QApplication(argc, argv);
    DebugLibrary *debug = new DebugLibrary(argc, argv);

    MixFaceWindow *mixFaceWindow = new MixFaceWindow(mixFace,debug);
    mixFaceWindow->showMaximized();
    return mixFace->exec();
}

MixFaceWindow::MixFaceWindow(QApplication *mixFace_, DebugLibrary *debug_)
    : QMainWindow(),
      debug(debug_),
      mixFace(mixFace_)
{
    dpiRatio = float(this->logicalDpiX())/96;
    if (mixFace->arguments().contains("-dpi",Qt::CaseInsensitive)) {
        QRegExp rxdpi("-dpi");
        rxdpi.setCaseSensitivity(Qt::CaseInsensitive);
        int idxdpi = mixFace->arguments().indexOf(rxdpi)+1;
        dpiRatio = QString(mixFace->arguments().value(idxdpi)).toFloat()/96;
    }
    if (dpiRatio < 1) dpiRatio = 1;
    if (dpiRatio > 2) dpiRatio = 2;
    debug->sendMessage(QString("MixFaceWindow::MixFaceWindow DPI ratio set to " + QString::number(dpiRatio)).toStdString(),1);

    debug->sendMessage(QString("MixFaceWindow::MixFaceWindow Init MixFaceFonts...").toStdString(),5);
    mf_fonts = new MixFaceFonts;
    debug->sendMessage(QString("MixFaceWindow::MixFaceWindow Init MixFaceLibrary...").toStdString(),3);
    mf_library = new MixFaceLibrary(this, debug);

    initUI();

    mf_library->valueChanged.connect(signal_type_thr_int(&MixFaceWindow::valueChanged, this, boost::arg<1>(), boost::arg<2>(), boost::arg<3>()));

    for(int idx = 0;idx<80;idx++)
        for (int idy = 0;idy < 7;idy++)
            if(idx!=70&&idx!=71&&(idx<48||idx>63))
                assignedFader[idx][idy] = true;
            else
                assignedFader[idx][idy] = false;

    QTimer *metersDemoTimer = new QTimer();
    QTimer::connect(metersDemoTimer, &QTimer::timeout, this, &MixFaceWindow::metersDemo);
    metersDemoTimer->start(5);
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

    topArea = new QScrollArea;
    topArea->setFrameShape(QFrame::NoFrame);
    topArea->setLineWidth(0);
    topArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    topArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    topArea->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    topArea->setWidgetResizable(true);
    topArea->setMaximumHeight(32*dpiRatio);
    topArea->setMinimumHeight(32*dpiRatio);

    verticalLayout->addWidget(topArea);

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

    rightArea = new QScrollArea;
    QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Expanding);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(rightArea->sizePolicy().hasHeightForWidth());
    rightArea->setSizePolicy(sizePolicy2);
    rightArea->setMaximumSize(QSize(64*dpiRatio, 16777215));
    rightArea->setFrameShape(QFrame::NoFrame);
    rightArea->setLineWidth(0);
    rightArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    rightArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    rightArea->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    rightArea->setWidgetResizable(true);
    rightArea->setAlignment(Qt::AlignHCenter|Qt::AlignTop);

    horizontalLayout->addWidget(rightArea);

    verticalLayout->addLayout(horizontalLayout);

    debug->sendMessage(QString("MixFaceWindow::initTopAreaBar...").toStdString(),4);
    initTopAreaBar();
    debug->sendMessage(QString("MixFaceWindow::initControlAreaWidgets...").toStdString(),4);
    initControlAreaWidgets();
    debug->sendMessage(QString("MixFaceWindow::initMainAreaWidgets...").toStdString(),4);
    initMainAreaWidgets();
    debug->sendMessage(QString("MixFaceWindow::initRightAreaBar...").toStdString(),4);
    initRightAreaBar();
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
}

MixFaceWindow::~MixFaceWindow() {}

void MixFaceWindow::resizeEvent(QResizeEvent *e){
    QMainWindow::resizeEvent(e);
    bar->setGeometry(QRect(0,(this->geometry().height())-17,(this->geometry().width())-(64*dpiRatio)-(128*dpiRatio),14));
}

void MixFaceWindow::connection(){
    connected = mf_library->connectTo(mf_topArea->lineIp->text().toStdString());
    if (connected) {
        mf_topArea->syncAction->setEnabled(true);
        mf_topArea->lineIp->setDisabled(true);
        //mf_library->sendSyncMessages();
        mf_topArea->connectAction->setText("Disconnect...");
        mf_topArea->consoleName->setText(QString::fromStdString(mf_library->linker->xinfo[1]));
        mf_topArea->consoleName->setStyleSheet("QLabel {"
                                   "color: rgb(255,255,255);"
                                   "background-color: rgb(96, 96, 96);"
                                   "border: 0px solid rgb(0,0,0);"
                                   "border-radius: 0px;"
                                   "}");
        mf_topArea->console->setText(QString::fromStdString(mf_library->linker->xinfo[2]));
        demo=false;
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
        demo=true;
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
    scrollWidget[idx] = new FaderWidget(dpiRatio, debug, mf_fonts, this);
    scrollWidget[idx]->setChannelNativeName(QString::fromStdString(mf_library->channelNameFromIdx(idx)));
    scrollWidget[idx]->setFaderType(ftype);
    scrollWidget[idx]->setProperty("idx", idx);
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
    mainWidget[idx] = new FaderWidget(dpiRatio, debug, mf_fonts, this);
    mainWidget[idx]->setChannelNativeName(QString::fromStdString(mf_library->channelNameFromIdx(idx)));
    mainWidget[idx]->setFaderType(mf_types.getFaderType(idx));
    mainWidget[idx]->setProperty("idx", idx);
    connect(mainWidget[idx], &FaderWidget::faderChanged, this, &MixFaceWindow::faderChanged);
    connect(mainWidget[idx], &FaderWidget::panChanged, this, &MixFaceWindow::panChanged);
    connect(mainWidget[idx], &FaderWidget::muteChanged, this, &MixFaceWindow::muteClicked);
    connect(mainWidget[idx], &FaderWidget::soloChanged, this, &MixFaceWindow::soloClicked);
    connect(mainWidget[idx], &FaderWidget::srcClicked, this, &MixFaceWindow::buttonSrcClicked);
    connect(mainWidget[idx], &FaderWidget::eqClicked, this, &MixFaceWindow::buttonEqClicked);
    connect(mainWidget[idx], &FaderWidget::dynClicked, this, &MixFaceWindow::buttonDynClicked);
    connect(mainWidget[idx], &FaderWidget::iconButtonClicked, this, &MixFaceWindow::iconButtonClicked);
    if (idx==70)
        connect(mf_library, &MixFaceLibrary::processMeter6, scrollWidget[idx], &FaderWidget::setMeter);
}

void MixFaceWindow::initRightAreaBar(){
    mf_rightArea = new MixFaceRightWidget(dpiRatio, debug, mf_fonts, this);
    rightArea->setWidget(mf_rightArea);
    connect(mf_rightArea, &MixFaceRightWidget::assignMainFader, this, &MixFaceWindow::assignMainFader);
    connect(mf_rightArea, &MixFaceRightWidget::assignScrollFaders, this, &MixFaceWindow::assignScrollFaders);
}

void MixFaceWindow::initTopAreaBar(){
    mf_topArea = new MixFaceTopWidget(dpiRatio, debug, mf_fonts, this);
    topArea->setWidget(mf_topArea);
    connect(mf_topArea, &MixFaceTopWidget::connect, this, &MixFaceWindow::connection);
    connect(mf_topArea, &MixFaceTopWidget::sync, mf_library, &MixFaceLibrary::sendSyncMessages);
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
        name = QString::fromStdString(mf_library->channelNameFromIdx(idx));
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
    mf_picker->showIconPopup(idx, mf_library->db.configicon[idx], mf_library->db.configcolor[idx],QString::fromStdString(mf_library->channelNameFromIdx(idx)),QString::fromStdString(mf_library->db.configname[idx]));
}

void MixFaceWindow::logoChanged(int idx, int value){
    scrollWidget[idx]->setLogo(value);
    mf_library->db.configicon[idx] = value;
}

void MixFaceWindow::colorChanged(int idx, int value){
    scrollWidget[idx]->setColor(value);
    mf_library->db.configcolor[idx] = value;
}

void MixFaceWindow::nameChanged(int idx, QString value){
    scrollWidget[idx]->setName(value);
    mf_library->db.configname[idx] = value.toStdString();
}

void MixFaceWindow::buttonSrcClicked(){
    int idx = reinterpret_cast<QObject *>(sender())->property("idx").toInt();

    if (dynWidget != nullptr) { delete dynWidget; dynWidget = nullptr; }
    if (eqWidget != nullptr) { delete eqWidget; eqWidget = nullptr; }
    if (srcWidget != nullptr) { delete srcWidget; srcWidget = nullptr; }
    if (srcFader != nullptr) {
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
            srcFader = new FaderWidget(dpiRatio, debug, mf_fonts, this);
            srcLayout->addWidget(srcFader);
            srcFader->setChannelNativeName(QString::fromStdString(mf_library->channelNameFromIdx(idx)));
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

        srcWidget = new SourceWidget(idx, dpiRatio, mf_fonts);
        srcLayout->addWidget(srcWidget);

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

    if (dynWidget != nullptr) { delete dynWidget; dynWidget = nullptr; }
    if (eqWidget != nullptr) { delete eqWidget; eqWidget = nullptr; }
    if (srcWidget != nullptr) { delete srcWidget; srcWidget = nullptr; }
    if (srcFader != nullptr) {
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
            srcFader = new FaderWidget(dpiRatio, debug, mf_fonts, this);
            srcLayout->addWidget(srcFader);
            srcFader->setChannelNativeName(QString::fromStdString(mf_library->channelNameFromIdx(idx)));
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

        eqWidget = new EqualizerWidget(idx, dpiRatio, mf_fonts);
        srcLayout->addWidget(eqWidget);

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

    if (dynWidget != nullptr) { delete dynWidget; dynWidget = nullptr; }
    if (eqWidget != nullptr) { delete eqWidget; eqWidget = nullptr; }
    if (srcWidget != nullptr) { delete srcWidget; srcWidget = nullptr; }
    if (srcFader != nullptr) {
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
            srcFader = new FaderWidget(dpiRatio, debug, mf_fonts, this);
            srcLayout->addWidget(srcFader);
            srcFader->setChannelNativeName(QString::fromStdString(mf_library->channelNameFromIdx(idx)));
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

        dynWidget = new DynamicsWidget(idx, dpiRatio, mf_fonts);
        srcLayout->addWidget(dynWidget);

        scrollArea->setHidden(true);
        bar->setHidden(true);
        srcArea->setVisible(true);
        m_mode = mDyn;
    } else {
        srcArea->setHidden(true);
        scrollArea->setVisible(true);
        bar->setVisible(true);
        m_mode = mCtrl;
    }
}

void MixFaceWindow::panChanged(float value)
{
    value = value/10000;
    int idx = reinterpret_cast<QObject *>(sender())->property("idx").toInt();
    MessageType mtype = pan;
    int send = 0;
    if (currentIdy > 0 && currentIdy < 17 && idx < 48) {
        mtype = sendpan;
        send = currentIdy;
        mf_library->db.sendpan[idx][currentIdy] = value;
    } else {
        mf_library->db.pan[idx] = value;
    }

    if (connected) {
        ChannelType chtype = mf_library->getChannelTypeFromIdx(idx);
        int chN = mf_library->getChannelNumberFromIdx(idx);
        string msg = mf_library->getOscAddress(mtype, chtype, chN, send);
        mf_library->linker->sendFloat(msg.c_str(),value);
    }

    if ((idx>47&&idx<65)||idx==70||idx==71) {
        valueChanged(pan,idx,currentIdy);
    }
}

void MixFaceWindow::faderChanged(float value)
{
    value = value/10000;
    int idx = reinterpret_cast<QObject *>(sender())->property("idx").toInt();
    MessageType mtype = fader;
    int send = 0;
    if (currentIdy == 0 || idx > 47)
        mf_library->db.fader[idx] = value;
    else if (currentIdy > 0 && currentIdy < 17 && idx < 48) {
        send = currentIdy;
        mtype = sendlevel;
        mf_library->db.sendlevel[idx][currentIdy] = value;
    } else if (currentIdy == 17 && idx < 48) {
        mtype = mlevel;
        mf_library->db.mlevel[idx] = value;
    }

    if (connected) {
        ChannelType chtype = mf_library->getChannelTypeFromIdx(idx);
        int chN = mf_library->getChannelNumberFromIdx(idx);
        string msg = mf_library->getOscAddress(mtype, chtype, chN, send);
        mf_library->linker->sendFloat(msg.c_str(), value);
    }

    if ((idx>47&&idx<65)||idx==70||idx==71)
        valueChanged(fader,idx,currentIdy);
}

void MixFaceWindow::muteClicked(int value)
{
    int idx = reinterpret_cast<QObject *>(sender())->property("idx").toInt();
    int send = 0;
    MessageType mtype = on;
    if (currentIdy == 0 || idx > 47)
        mf_library->db.on[idx] = value;
    else if (currentIdy > 0 && currentIdy < 17 && idx < 48) {
        send = currentIdy;
        mtype = sendon;
        mf_library->db.sendon[idx][currentIdy] = value;
    } else if (currentIdy == 17 && idx < 48) {
        mtype = monoon;
        mf_library->db.monoon[idx] = value;
    }

    if (connected) {
        ChannelType chtype = mf_library->getChannelTypeFromIdx(idx);
        int chN = mf_library->getChannelNumberFromIdx(idx);
        string msg = mf_library->getOscAddress(mtype, chtype, chN, send);
        mf_library->linker->sendFloat(msg.c_str(), value);
    }

    if ((idx>47&&idx<65)||idx==70||idx==71) {
        valueChanged(on,idx,currentIdy);
    }
}

void MixFaceWindow::soloClicked(int value)
{
    int idx = reinterpret_cast<QObject *>(sender())->property("idx").toInt();
    mf_library->db.solo[idx] = value;
    if (connected) for (int ids=0;ids<80;ids++)
    {
        QString msg;
        if (ids<10) msg = ("/-stat/solosw/0" + QString::number(ids+1));
        else msg = ("/-stat/solosw/" + QString::number(ids+1));
        int value = mf_library->db.solo[ids];
        QByteArray oscAddressArray = msg.toLatin1();
        const char *oscAddress = oscAddressArray;
        mf_library->linker->sendInt(oscAddress,value);
    }
    if ((idx>47&&idx<65)||idx==70||idx==71) {
        valueChanged(solo,idx,currentIdy);
    }
}

void MixFaceWindow::windowRenew() {
    for (int idx=0;idx<80;idx++) {
        disconnect(scrollWidget[idx], &FaderWidget::faderChanged, this, &MixFaceWindow::faderChanged);
        if (currentIdy==0||idx>47) {
            scrollWidget[idx]->setFaderValue(mf_library->db.fader[idx]);
        }
        if (currentIdy==17&&idx<48) {
            scrollWidget[idx]->setFaderValue(mf_library->db.fader[idx]);
        }
        if (currentIdy!=17&&currentIdy!=0&&idx<48) {
            scrollWidget[idx]->setFaderValue(mf_library->db.fader[idx]);
        }
        connect(scrollWidget[idx], &FaderWidget::faderChanged, this, &MixFaceWindow::faderChanged);
    }
}

void MixFaceWindow::valueChanged(int imtype, int idx, int idy) {
    MessageType mtype = MessageType(imtype);
    switch (mtype) {
    case stereoon:
        break;
    case monoon:
        if(currentIdy==17&&idx<48) {
            if (mf_library->db.monoon[idx]==0)
                scrollWidget[idx]->setMute(true);
            else
                scrollWidget[idx]->setMute(false);
        }
        break;
    case mlevel:
        if(currentIdy==17&&idx<48) {
            disconnect(scrollWidget[idx], &FaderWidget::faderChanged, this, &MixFaceWindow::faderChanged);
            scrollWidget[idx]->setFaderValue(mf_library->db.mlevel[idx]);
            connect(scrollWidget[idx], &FaderWidget::faderChanged, this, &MixFaceWindow::faderChanged);
        }
        break;
    case fader:
        if(currentIdy==0||idx>47) {
            disconnect(scrollWidget[idx], &FaderWidget::faderChanged, this, &MixFaceWindow::faderChanged);
            scrollWidget[idx]->setFaderValue(mf_library->db.fader[idx]);
            connect(scrollWidget[idx], &FaderWidget::faderChanged, this, &MixFaceWindow::faderChanged);
            if ((idx>47&&idx<64)||idx==70||idx==71) {
                disconnect(mainWidget[idx], &FaderWidget::faderChanged, this, &MixFaceWindow::faderChanged);
                mainWidget[idx]->setFaderValue(mf_library->db.fader[idx]);
                connect(mainWidget[idx], &FaderWidget::faderChanged, this, &MixFaceWindow::faderChanged);
            }
        }
        break;
    case pan:
        if(currentIdy==0||idx>47) {
            disconnect(scrollWidget[idx], &FaderWidget::panChanged, this, &MixFaceWindow::panChanged);
            scrollWidget[idx]->setPanValue(mf_library->db.pan[idx]);
            connect(scrollWidget[idx], &FaderWidget::panChanged, this, &MixFaceWindow::panChanged);
            if ((idx>47&&idx<64)||idx==70||idx==71) {
                disconnect(mainWidget[idx], &FaderWidget::panChanged, this, &MixFaceWindow::panChanged);
                mainWidget[idx]->setPanValue(mf_library->db.pan[idx]);
                connect(mainWidget[idx], &FaderWidget::panChanged, this, &MixFaceWindow::panChanged);
            }
        }
        break;
    case on:
        if(currentIdy==0||idx>47) {
            if (mf_library->db.on[idx]==0) {
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
        if(mf_library->db.solo[idx]==0) {
            scrollWidget[idx]->setSolo(false);
            if((idx<64&&idx>47)||idx==70||idx==71)
                mainWidget[idx]->setSolo(false);
        } else {
            scrollWidget[idx]->setSolo(true);
            if((idx<64&&idx>47)||idx==70||idx==71)
                mainWidget[idx]->setSolo(true);
        }
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
        break;
    case gatethr:
        break;
    case gaterange:
        break;
    case gatemode:
        break;
    case gateattack:
        break;
    case gatehold:
        break;
    case gaterelease:
        break;
    case gatekeysrc:
        break;
    case gatefilteron:
        break;
    case gatefiltertype:
        break;
    case gatefilterf:
        break;
    case dynon:
        break;
    case dynthr:
        break;
    case dynratio:
        break;
    case dynmix:
        break;
    case dynmgain:
        break;
    case dynattack:
        break;
    case dynhold:
        break;
    case dynrelease:
        break;
    case dynmode:
        break;
    case dynknee:
        break;
    case dynenv:
        break;
    case dyndet:
        break;
    case dynauto:
        break;
    case dynkeysrc:
        break;
    case dynfilteron:
        break;
    case dynfiltertype:
        break;
    case dynfilterf:
        break;
    case eq1type:
        break;
    case eq1g:
        break;
    case eq1f:
        break;
    case eq1q:
        break;
    case eq2type:
        break;
    case eq2g:
        break;
    case eq2f:
        break;
    case eq2q:
        break;
    case eq3type:
        break;
    case eq3g:
        break;
    case eq3f:
        break;
    case eq3q:
        break;
    case eq4type:
        break;
    case eq4g:
        break;
    case eq4f:
        break;
    case eq4q:
        break;
    case eq5type:
        break;
    case eq5g:
        break;
    case eq5f:
        break;
    case eq5q:
        break;
    case eq6type:
        break;
    case eq6g:
        break;
    case eq6f:
        break;
    case eq6q:
        break;
    case sendlevel:
        if(currentIdy==idy&&idx<48) {
            disconnect(scrollWidget[idx], &FaderWidget::faderChanged, this, &MixFaceWindow::faderChanged);
            scrollWidget[idx]->setFaderValue(mf_library->db.sendlevel[idx][idy]);
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
            if (mf_library->db.sendon[idx][idy]==0)
                scrollWidget[idx]->setMute(true);
            else
                scrollWidget[idx]->setMute(false);
        }
        break;
    case configcolor:
        break;
    case configicon:
        break;
    case configname:
        break;
    case merror:
        debug->sendMessage(QString("Error change value!").toStdString(),0);
        break;
    }
}

void MixFaceWindow::metersDemo(){
    if (demo){
        if(demoissum&&demoStep==1) demoissum = false;
        else if (!demoissum&&demoStep==-110) demoissum = true;
        if(demoissum) demoStep = demoStep + 0.25;
        else demoStep = demoStep - 0.25;
        for (int idx = 0;idx<80;idx++) {
            scrollWidget[idx]->setMeter(pow(10,demoStep/20),pow(10,demoStep/20),0,0);
            if (idx==70||idx==71||(idx>47&&idx<64)) mainWidget[idx]->setMeter(pow(10,demoStep/20),pow(10,demoStep/20),0,0);
        }
    }
}
