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


    QPushButton *button = new QPushButton;
    button->setMinimumSize(96*dpiRatio, 32*dpiRatio);
    button->setText("Rename");
    button->setStyleSheet("QPushButton {"
                          "color: rgb(0,0,0);"
                          "background-color: rgb(196, 196, 196);"
                          "border: 2px solid rgb(32,32,32);"
                          "border-radius: 0px;}"
                          "QPushButton:pressed {"
                          "background-color: rgb(128, 128, 128);}");
    button->setFont(m_fonts->boldFont);
    hlayout->addWidget(button);
    QPushButton::connect(button,&QPushButton::clicked,this,&MixFaceIconPicker::emitNameChanged);

    hlayout->addSpacing(Qt::Horizontal);

    vlayout->addLayout(hlayout);

    iconsList = new QListWidget;
    iconsList->setResizeMode(QListView::Adjust);
    iconsList->setViewMode(QListView::IconMode);
    iconsList->setUniformItemSizes(true);
    iconsList->setGridSize(QSize(96,96));
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
                             "}"
                             "QScrollBar:vertical {"
                             "background-color: rgb(76,76,76);"
                             "width: 14px;"
                             "margin: 0px;"
                             "}"
                             "QScrollBar::handle:vertical {"
                             "background-color: rgb(122,121,122);"
                             "min-height: 20px;"
                             "margin: 2px;"
                             "border-radius: 5px;"
                             "border-width: 1px;"
                             "border: 1px solid rgb(192, 192, 192);"
                             "}"
                             "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
                             "border: none;"
                             "background: none;"
                             "height: 0px;"
                             "}"
                             "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical,QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
                             "border: none;"
                             "background: none;"
                             "color: none;"
                             "}");

    for (int i = 1;i < 75; i++){
        QListWidgetItem *item = new QListWidgetItem;
        item->setIcon(QIcon(":/fader/icons/" + QString::number(i) + "W"));
        iconsList->insertItem(i,item);
    }
    vlayout->addWidget(iconsList);

    hlayout = new QHBoxLayout;

    colorsList = new QList<QPushButton*>;
    for(int i = 0;i < 16; i++){
        QString fgcolor;
        QString bgcolor;
        switch(i){
        case 0:
            fgcolor = "rgb(255,255,255)";
            bgcolor = "rgb(64,64,64)";
            break;
        case 1:
            bgcolor = "rgb(255,64,64)";
            fgcolor = "rgb(32,32,32)";
            break;
        case 2:
            bgcolor = "rgb(64,255,64)";
            fgcolor = "rgb(32,32,32)";
            break;
        case 3:
            bgcolor = "rgb(255,255,64)";
            fgcolor = "rgb(32,32,32)";
            break;
        case 4:
            bgcolor = "rgb(96,96,255)";
            fgcolor = "rgb(32,32,32)";
            break;
        case 5:
            bgcolor = "rgb(255,64,255)";
            fgcolor = "rgb(32,32,32)";
            break;
        case 6:
            bgcolor = "rgb(64,255,255)";
            fgcolor = "rgb(32,32,32)";
            break;
        case 7:
            bgcolor = "rgb(255,255,255)";
            fgcolor = "rgb(32,32,32)";
            break;
        case 8:
            bgcolor = "rgb(192,192,192)";
            fgcolor = "rgb(32,32,32)";
            break;
        case 9:
            bgcolor = "rgb(32,32,32)";
            fgcolor = "rgb(255,0,0)";
            break;
        case 10:
            bgcolor = "rgb(32,32,32)";
            fgcolor = "rgb(0,255,0)";
            break;
        case 11:
            bgcolor = "rgb(32,32,32)";
            fgcolor = "rgb(255,255,0)";
            break;
        case 12:
            bgcolor = "rgb(32,32,32)";
            fgcolor = "rgb(32,32,255)";
            break;
        case 13:
            bgcolor = "rgb(32,32,32)";
            fgcolor = "rgb(255,0,255)";
            break;
        case 14:
            bgcolor = "rgb(32,32,32)";
            fgcolor = "rgb(0,255,255)";
            break;
        case 15:
            bgcolor = "rgb(32,32,32)";
            fgcolor = "rgb(255,255,255)";
            break;
        default:
            fgcolor = "rgb(255,255,255)";
            bgcolor = "rgb(64,64,64)";
            break;
        }

        button = new QPushButton("");
        button->setMinimumHeight(64*dpiRatio);
        button->setStyleSheet("QPushButton {"
                              "border: 2px solid " + fgcolor + ";"
                              "background-color: " + bgcolor + ";"
                              "}"
                              "QPushButton:pressed {"
                              "border: 2px solid " + bgcolor + ";"
                              "background-color: " + fgcolor + ";"
                              "}"
                              "QPushButton:checked {"
                              "border: 2px solid " + fgcolor + ";"
                              "background-color: " + bgcolor + ";"
                              "}");
        button->setProperty("colorId", i);
        QListWidget::connect(button, &QPushButton::clicked,
                             this, &MixFaceIconPicker::emitColorChanged);
        hlayout->addWidget(button);
        colorsList->push_back(button);
    }

    vlayout->addLayout(hlayout);

    hlayout = new QHBoxLayout;
    hlayout->addStretch();
    button = new QPushButton("Close");
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
    iconButton->setIcon(QIcon(":/fader/icons/" + QString::number(logoId) + "W"));
    iconsList->setCurrentRow(logoId - 1);
    iconNumber = logoId;
    setColorPreview(colorId);
    colorNumber = colorId;
    for(qsizetype i = 0;i < colorsList->count();i++){
        if(colorsList->at(i)->property("colorId").toInt() == colorId)
            colorsList->at(i)->setHidden(true);
        else
            colorsList->at(i)->setHidden(false);
    }
    nativeName->setText(nativeName_);
    channelName->setText(name);

    QRect rect = this->geometry();
    rect.setX(mainWindow->geometry().left() + (24*dpiRatio));
    rect.setY(mainWindow->geometry().top() + (24*dpiRatio));
    rect.setWidth(mainWindow->geometry().width() - (48*dpiRatio));
    rect.setHeight(mainWindow->geometry().height() - (48*dpiRatio));
    this->setGeometry(rect);

    if (!this->isVisible()) this->show();
}

void MixFaceIconPicker::emitLogoChanged(int idl){
    idl++;
    iconNumber = idl;
    iconButton->setIcon(QIcon(":/fader/icons/" + QString::number(idl) + BWIcon));
    emit logoChanged(idx, idl);
}

void MixFaceIconPicker::emitColorChanged(){
    int colorId = reinterpret_cast<QPushButton*>(sender())->property("colorId").toInt();
    setColorPreview(colorId);
    for(qsizetype i = 0;i < colorsList->count();i++){
        if(colorsList->at(i)->property("colorId").toInt() == colorId)
            colorsList->at(i)->setHidden(true);
        else
            colorsList->at(i)->setHidden(false);
    }
    emit colorChanged(idx, colorId);
}

void MixFaceIconPicker::emitNameChanged(){
    emit nameChanged(idx, channelName->text());
}

void MixFaceIconPicker::setColorPreview(int value) {
    colorNumber = value;
    QString fgcolor;
    QString bgcolor;

    switch(colorNumber){
    case 0:
        BWIcon = "W";
        fgcolor = "rgb(255,255,255)";
        bgcolor = "rgb(64,64,64)";
        break;
    case 1:
        BWIcon = "B";
        bgcolor = "rgb(255,64,64)";
        fgcolor = "rgb(32,32,32)";
        break;
    case 2:
        BWIcon = "B";
        bgcolor = "rgb(64,255,64)";
        fgcolor = "rgb(32,32,32)";
        break;
    case 3:
        BWIcon = "B";
        bgcolor = "rgb(255,255,64)";
        fgcolor = "rgb(32,32,32)";
        break;
    case 4:
        BWIcon = "B";
        bgcolor = "rgb(96,96,255)";
        fgcolor = "rgb(32,32,32)";
        break;
    case 5:
        BWIcon = "B";
        bgcolor = "rgb(255,64,255)";
        fgcolor = "rgb(32,32,32)";
        break;
    case 6:
        BWIcon = "B";
        bgcolor = "rgb(64,255,255)";
        fgcolor = "rgb(32,32,32)";
        break;
    case 7:
        BWIcon = "B";
        bgcolor = "rgb(255,255,255)";
        fgcolor = "rgb(32,32,32)";
        break;
    case 8:
        BWIcon = "B";
        bgcolor = "rgb(192,192,192)";
        fgcolor = "rgb(32,32,32)";
        break;
    case 9:
        BWIcon = "W";
        bgcolor = "rgb(32,32,32)";
        fgcolor = "rgb(255,0,0)";
        break;
    case 10:
        BWIcon = "W";
        bgcolor = "rgb(32,32,32)";
        fgcolor = "rgb(0,255,0)";
        break;
    case 11:
        BWIcon = "W";
        bgcolor = "rgb(32,32,32)";
        fgcolor = "rgb(255,255,0)";
        break;
    case 12:
        BWIcon = "W";
        bgcolor = "rgb(32,32,32)";
        fgcolor = "rgb(32,32,255)";
        break;
    case 13:
        BWIcon = "W";
        bgcolor = "rgb(32,32,32)";
        fgcolor = "rgb(255,0,255)";
        break;
    case 14:
        BWIcon = "W";
        bgcolor = "rgb(32,32,32)";
        fgcolor = "rgb(0,255,255)";
        break;
    case 15:
        BWIcon = "W";
        bgcolor = "rgb(32,32,32)";
        fgcolor = "rgb(255,255,255)";
        break;
    default:
        BWIcon = "W";
        fgcolor = "rgb(255,255,255)";
        bgcolor = "rgb(64,64,64)";
        break;
    }

    //set color by number
    iconButton->setStyleSheet("QPushButton {"
                           "border: 2px solid " + fgcolor + ";"
                           "background-color: " + bgcolor + ";"
                           "}");

    iconButton->setIcon(QIcon(":/fader/icons/" + QString::number(iconNumber) + BWIcon));
}
