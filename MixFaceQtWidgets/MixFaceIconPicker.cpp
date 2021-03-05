#include "MixFaceIconPicker.h"

MixFaceIconPicker::MixFaceIconPicker(float dpiRatio_, DebugLibrary *debug_,
                                     MixFaceFonts *m_fonts_, QMainWindow *mainWindow_)
    : dpiRatio(dpiRatio_), mainWindow(mainWindow_), debug(debug_), m_fonts(m_fonts_)
{
    initIconPopup();
}

void MixFaceIconPicker::initIconPopup(){
    this->setWindowFlag(Qt::Popup);
    this->setFrameStyle(QAbstractScrollArea::WinPanel);
    this->setFont(m_fonts->nativeFont);
    this->resize(1,1);
    this->setStyleSheet("QFrame {"
                                 "background-color: rgb(96, 96, 96);"
                                 "border: 2px solid rgb(32,32,32);"
                                 "border-radius: 0px;"
                                 "}");

    QVBoxLayout *vlayout = new QVBoxLayout(this);

    QHBoxLayout *hlayout = new QHBoxLayout;

    hlayout->addSpacing(Qt::Horizontal);

    iconButton = new QPushButton;
    iconButton->setStyleSheet("QPushButton {"
                        "background-color: rgb(64, 64, 64);"
                        "border: 3px solid rgb(0,0,0);}");
    iconButton->setMinimumSize(108,108);
    iconButton->setIcon(QIcon(":/fader/icons/" + QString::number(1) + "W"));
    iconButton->setIconSize(QSize(72,72));
    hlayout->addWidget(iconButton);

    nativeName = new QLabel("Ch 18");
    nativeName->setStyleSheet("QLabel {"
                           "color: rgb(255,255,255);"
                           "background-color: rgb(96, 96, 96);"
                           "border: 0px solid rgb(0,0,0);"
                           "border-radius: 0px;"
                           "}");
    nativeName->setFont(m_fonts->boldFont);
    nativeName->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    hlayout->addWidget(nativeName);

    channelName = new QLineEdit("Ch 18");
    channelName->setFont(m_fonts->boldFont);
    hlayout->addWidget(channelName);

    QPushButton *rename = new QPushButton;
    rename->setText("Rename");
    hlayout->addWidget(rename);
    QPushButton::connect(rename,&QPushButton::clicked,this,&MixFaceIconPicker::emitNameChanged);

    hlayout->addSpacing(Qt::Horizontal);

    vlayout->addLayout(hlayout);

    iconsList = new QListWidget;
    iconsList->setResizeMode(QListView::Adjust);
    iconsList->setViewMode(QListView::IconMode);
    iconsList->setUniformItemSizes(true);
    iconsList->setGridSize(QSize(108,108));
    iconsList->setIconSize(QSize(72,72));
    iconsList->setSpacing(4);
    iconsList->setMovement(QListView::Static);
    iconsList->setItemAlignment(Qt::AlignCenter);
    QScroller::grabGesture(iconsList, QScroller::LeftMouseButtonGesture);
    QListWidget::connect(iconsList,&QListWidget::currentRowChanged,this,&MixFaceIconPicker::emitLogoChanged);
    iconsList->setStyleSheet("QListWidget::item:selected, QListWidget::item:selected:!active {"
                             "color: rgb(255, 255, 255);"
                             "background-color: rgb(48,48,48);"
                             "border: 3px solid rgb(0,0,0);"
                             "}"
                             "QListWidget::item:hover, QListWidget::item:disabled:hover, QListWidget::item:hover:!active {"
                             "color: rgb(255, 255, 255);"
                             "background-color: rgb(80,80,80);"
                             "border: 3px solid rgb(32,32,32);"
                             "}");

    for (int i = 1;i < 75; i++){
        QListWidgetItem *item = new QListWidgetItem;
        item->setIcon(QIcon(":/fader/icons/" + QString::number(i) + "W"));
        iconsList->insertItem(i,item);
    }

    vlayout->addWidget(iconsList);

    hlayout = new QHBoxLayout;
    hlayout->addStretch();
    QPushButton *button = new QPushButton("Close");
    button->setMinimumSize(96*dpiRatio, 32*dpiRatio);
    button->setStyleSheet("QPushButton {"
                          "color: rgb(0,0,0);"
                          "background-color: rgb(196, 196, 196);"
                          "border: 2px solid rgb(32,32,32);"
                          "border-radius: 0px;}"
                          "QPushButton:pressed {"
                          "background-color: rgb(128, 128, 128);}");
    button->setFont(m_fonts->boldFont);
    QPushButton::connect(button,&QPushButton::clicked,this,&QFrame::hide);
    hlayout->addWidget(button);

    vlayout->addLayout(hlayout);
}

void MixFaceIconPicker::showIconPopup(int idx_, int logoId, int colorId, QString nativeName_, QString name){
    idx = idx_;
    QIcon cico(":/f_icons/" + QString::number(logoId));
    iconButton->setIcon(cico);
    iconsList->setCurrentRow(logoId-1);
    //color needed
    Q_UNUSED(colorId)
    nativeName->setText(nativeName_);
    channelName->setText(name);

    QRect rect = this->geometry();
    rect.setX(mainWindow->geometry().left() + (98*dpiRatio));
    rect.setY(mainWindow->geometry().top() + (98*dpiRatio));
    rect.setWidth(mainWindow->geometry().width() - (196*dpiRatio));
    rect.setHeight(mainWindow->geometry().height() - (196*dpiRatio));
    this->setGeometry(rect);

    if (!this->isVisible()) this->show();
}

void MixFaceIconPicker::emitLogoChanged(int idl){
    idl++;
    iconButton->setIcon(QIcon(":/fader/icons/" + QString::number(idl) + "W"));
    emit logoChanged(idx, idl);
}

void MixFaceIconPicker::emitColorChanged(int idc){
    idc++;
    emit colorChanged(idx, idc);
}

void MixFaceIconPicker::emitNameChanged(){
    emit nameChanged(idx, channelName->text());
}
