#include "MixFaceTopWidget.h"

MixFaceTopWidget::MixFaceTopWidget(float dpiRatio_, DebugLibrary *debug_, MixFaceFonts *m_fonts_, QMainWindow *mainWindow_)
    : QWidget(),
      mainWindow(mainWindow_),
      m_fonts(m_fonts_),
      debug(debug_),
      dpiRatio(dpiRatio_)
{
    initTopAreaBar();
    initSettingsPopup();
}

void MixFaceTopWidget::initTopAreaBar(){
    QVBoxLayout *verticalLayout;
    QFrame *topFrame;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;

    verticalLayout = new QVBoxLayout(this);
    verticalLayout->setSpacing(0);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout_5"));
    verticalLayout->setContentsMargins(0, 0, 0, 0);

    topFrame = new QFrame(this);
    topFrame->setObjectName(QString::fromUtf8("topFrame"));
    topFrame->setMinimumSize(QSize(0, 30));
    topFrame->setStyleSheet(QString::fromUtf8(""));
    topFrame->setFrameShape(QFrame::Box);
    topFrame->setFrameShadow(QFrame::Plain);

    horizontalLayout = new QHBoxLayout(topFrame);
    horizontalLayout->setSpacing(4);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    horizontalLayout->setContentsMargins(4, 0, 4, 0);

    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout->addItem(horizontalSpacer);

    settings = new QPushButton("Settings");
    settings->setStyleSheet("QPushButton {"
                            "color: rgb(255,255,255);"
                            "background-color: rgb(96, 96, 96);"
                            "border: 1px solid rgb(32,32,32);"
                            "border-radius: 0px;}"
                            "QPushButton:pressed {"
                            "background-color: rgba(128, 128, 128, 255);"
                            "}");
    settings->setFont(m_fonts->boldFont8);
    settings->setMaximumSize(64*dpiRatio,28*dpiRatio);
    settings->setMinimumSize(64*dpiRatio,28*dpiRatio);
    horizontalLayout->addWidget(settings);

    verticalLayout->addWidget(topFrame);
    this->setLayout(verticalLayout);

    this->setMaximumHeight(32*dpiRatio);
    this->setMinimumHeight(32*dpiRatio);

    QPushButton::connect(settings, &QPushButton::clicked, this, &MixFaceTopWidget::settingsButtonClicked);

    topFrame->setStyleSheet("QFrame {"
                                "background-color: rgb(96, 96, 96);"
                                "}");
}

void MixFaceTopWidget::initSettingsPopup(){
    settingsPopup = new QFrame();
    settingsPopup->setWindowFlag(Qt::Popup);
    settingsPopup->setFrameStyle(QAbstractScrollArea::WinPanel);
    settingsPopup->setFont(m_fonts->nativeFont);
    settingsPopup->resize(1,1);
    settingsPopup->setStyleSheet("QFrame {"
                                 "background-color: rgb(96, 96, 96);"
                                 "border: 2px solid rgb(32,32,32);"
                                 "border-radius: 0px;"
                                 "}");

    QVBoxLayout *vlayout = new QVBoxLayout(settingsPopup);

    QHBoxLayout *labelLayout = new QHBoxLayout;
    labelLayout->setContentsMargins(0, 0, 0, 0);
    labelLayout->addSpacing(Qt::Horizontal);
    console = new QLabel("MixFace");
    console->setStyleSheet("QLabel {"
                           "color: rgb(255,255,255);"
                           "background-color: rgb(96, 96, 96);"
                           "border: 0px solid rgb(0,0,0);"
                           "border-radius: 0px;"
                           "}");
    console->setFont(m_fonts->boldFont12);
    console->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    labelLayout->addWidget(console);
    labelLayout->addSpacing(Qt::Horizontal);
    vlayout->addLayout(labelLayout);

    labelLayout = new QHBoxLayout;
    labelLayout->setContentsMargins(0, 0, 0, 0);
    labelLayout->addSpacing(Qt::Horizontal);
    consoleName = new QLabel("Demo mode...");
    consoleName->setStyleSheet("QLabel {"
                               "color: rgb(255,255,255);"
                               "background-color: rgb(96, 96, 96);"
                               "border: 0px solid rgb(0,0,0);"
                               "border-radius: 0px;"
                               "}");
    consoleName->setFont(m_fonts->italicFont12);
    consoleName->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    labelLayout->addWidget(consoleName);
    labelLayout->addSpacing(Qt::Horizontal);
    vlayout->addLayout(labelLayout);

    connectAction = new QPushButton("Connect/Disconnect");
    connectAction->setStyleSheet("QPushButton {"
                                  "color: rgb(255,255,255);"
                                  "background-color: rgb(96, 96, 96);"
                                  "border: 1px solid rgb(32,32,32);"
                                  "border-radius: 0px;}"
                                  "QPushButton:pressed {"
                                  "background-color: rgba(128, 128, 128, 255);"
                                  "}");
    connectAction->setFont(m_fonts->italicFont);
    vlayout->addWidget(connectAction);
    QPushButton::connect(connectAction, &QPushButton::clicked, this, &MixFaceTopWidget::connect);

    lineIp = new QLineEdit;
    lineIp->setStyleSheet("QLineEdit {"
                          "color: rgb(255,255,255);"
                          "background-color: rgb(96, 96, 96);"
                          "border: 1px solid rgb(32,32,32);"
                          "border-radius: 0px;"
                          "}"
                          "QLineEdit:disabled {"
                          "color: rgb(32,32,32);"
                          "background-color: rgb(128, 128, 128);"
                          "}");
    lineIp->setAlignment(Qt::AlignCenter);
    lineIp->setFont(m_fonts->nativeFont12);
    vlayout->addWidget(lineIp);

    syncAction = new QPushButton("Sync...");
    syncAction->setStyleSheet("QPushButton {"
                              "color: rgb(255,255,255);"
                              "background-color: rgb(96, 96, 96);"
                              "border: 1px solid rgb(32,32,32);"
                              "border-radius: 0px;}"
                              "QPushButton:pressed {"
                              "background-color: rgb(128, 128, 128);"
                              "}"
                              "QPushButton:disabled {"
                              "color: rgb(32,32,32);"
                              "background-color: rgb(128, 128, 128);"
                              "}");
    syncAction->setFont(m_fonts->italicFont);
    vlayout->addWidget(syncAction);
    QPushButton::connect(syncAction, &QPushButton::clicked, this, &MixFaceTopWidget::sync);

    QPushButton *settingsAction = new QPushButton("Exit");
    settingsAction->setStyleSheet("QPushButton {"
                                  "color: rgb(255,255,255);"
                                  "background-color: rgb(96, 96, 96);"
                                  "border: 1px solid rgb(32,32,32);"
                                  "border-radius: 0px;}"
                                  "QPushButton:pressed {"
                                  "background-color: rgba(128, 128, 128, 255);"
                                  "}");
    settingsAction->setFont(m_fonts->italicFont);
    vlayout->addWidget(settingsAction);
    QPushButton::connect(settingsAction, &QPushButton::clicked, mainWindow, &QMainWindow::close);

    vlayout->addSpacing(0);
}

void MixFaceTopWidget::settingsButtonClicked(){
    if (settingsPopup->geometry().width()<10) {
        settingsPopup->show();
        settingsPopup->hide();
    }

    QRect rect = settingsPopup->geometry();
    const QRect buttonGeometry = settings->geometry();
    const QRect windowGeometry = mainWindow->geometry();
    int height = rect.height();
    int width = rect.width();
    rect.setX(buttonGeometry.left()+windowGeometry.left()+buttonGeometry.width()-width);
    rect.setY(buttonGeometry.top()+windowGeometry.top());
    rect.setWidth(width);
    rect.setHeight(height);
    settingsPopup->setGeometry(rect);

    if (settingsPopup->isHidden()) settingsPopup->show();
}
