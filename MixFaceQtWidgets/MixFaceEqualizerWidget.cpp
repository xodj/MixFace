#include "MixFaceEqualizerWidget.h"

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

QWidget *EqualizerWidget::bandWidget(int idx){
    QWidget *widget = new QWidget;
    widget->setSizePolicy(QSizePolicy::Maximum,
                            QSizePolicy::Minimum);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(4, 4, 4, 4);
    vlayout->setSpacing(4);

    //label
    QLabel *label = new QLabel("Band " + QString::number(idx + 1));
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont8);
    label->setAlignment(Qt::AlignCenter);
    vlayout->addWidget(label);

    //mode+reset
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setContentsMargins(4, 4, 4, 4);
    hlayout->setSpacing(4);

    eqBandTypeComboBox[idx] = new QComboBox;
    hlayout->addWidget(eqBandTypeComboBox[idx]);

    eqBandResetPushButton[idx] = new QPushButton("Reset");
    eqBandResetPushButton[idx]->setMinimumSize(64*dpiRatio, 24*dpiRatio);
    eqBandResetPushButton[idx]->setStyleSheet(buttonStyle);
    eqBandResetPushButton[idx]->setFont(m_fonts->boldFont8);
    hlayout->addWidget(eqBandResetPushButton[idx]);

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

    eqBandGainLine[idx] = new QLineEdit("-15.0 dB");
    eqBandGainLine[idx]->setStyleSheet(lineStyle);
    eqBandGainLine[idx]->setAlignment(Qt::AlignCenter);
    eqBandGainLine[idx]->setMinimumSize(48*dpiRatio, 24*dpiRatio);
    eqBandGainLine[idx]->setMaximumWidth(48*dpiRatio);
    eqBandGainLine[idx]->setFont(m_fonts->boldFont8);
    hlayout->addWidget(eqBandGainLine[idx]);

    QSpacerItem *horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);
    hlayout->addItem(horizontalSpacer);

    vlayout->addLayout(hlayout);

    eqBandGainSlider[idx] = new QSlider;
    eqBandGainSlider[idx]->setOrientation(Qt::Orientation::Horizontal);
    eqBandGainSlider[idx]->setMinimumSize(32*dpiRatio, 32*dpiRatio);
    eqBandGainSlider[idx]->setStyleSheet(sliderStyle);
    eqBandGainSlider[idx]->setMaximum(5);
    eqBandGainSlider[idx]->setValue(0);
    eqBandGainSlider[idx]->setPageStep(0);
    vlayout->addWidget(eqBandGainSlider[idx]);

    //freq
    hlayout = new QHBoxLayout;
    hlayout->setContentsMargins(4, 4, 4, 4);
    hlayout->setSpacing(4);

    label = new QLabel("Freq");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont12);
    label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    hlayout->addWidget(label);

    eqBandFreqLine[idx] = new QLineEdit("863.0 Hz");
    eqBandFreqLine[idx]->setStyleSheet(lineStyle);
    eqBandFreqLine[idx]->setAlignment(Qt::AlignCenter);
    eqBandFreqLine[idx]->setMinimumSize(48*dpiRatio, 24*dpiRatio);
    eqBandFreqLine[idx]->setMaximumWidth(48*dpiRatio);
    eqBandFreqLine[idx]->setFont(m_fonts->boldFont8);
    hlayout->addWidget(eqBandFreqLine[idx]);

    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);
    hlayout->addItem(horizontalSpacer);

    vlayout->addLayout(hlayout);

    eqBandFreqSlider[idx] = new QSlider;
    eqBandFreqSlider[idx]->setOrientation(Qt::Orientation::Horizontal);
    eqBandFreqSlider[idx]->setMinimumSize(32*dpiRatio, 32*dpiRatio);
    eqBandFreqSlider[idx]->setStyleSheet(sliderStyle);
    eqBandFreqSlider[idx]->setMaximum(5);
    eqBandFreqSlider[idx]->setValue(0);
    eqBandFreqSlider[idx]->setPageStep(0);
    vlayout->addWidget(eqBandFreqSlider[idx]);

    //qual
    hlayout = new QHBoxLayout;
    hlayout->setContentsMargins(4, 4, 4, 4);
    hlayout->setSpacing(4);

    label = new QLabel("Qual");
    label->setStyleSheet("QLabel {color: rgb(255,255,255);}");
    label->setFont(m_fonts->boldFont12);
    label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    hlayout->addWidget(label);

    eqBandQualLine[idx] = new QLineEdit("0.3");
    eqBandQualLine[idx]->setStyleSheet(lineStyle);
    eqBandQualLine[idx]->setAlignment(Qt::AlignCenter);
    eqBandQualLine[idx]->setMinimumSize(48*dpiRatio, 24*dpiRatio);
    eqBandQualLine[idx]->setMaximumWidth(48*dpiRatio);
    eqBandQualLine[idx]->setFont(m_fonts->boldFont8);
    hlayout->addWidget(eqBandQualLine[idx]);

    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);
    hlayout->addItem(horizontalSpacer);

    vlayout->addLayout(hlayout);

    eqBandQualSlider[idx] = new QSlider;
    eqBandQualSlider[idx]->setOrientation(Qt::Orientation::Horizontal);
    eqBandQualSlider[idx]->setMinimumSize(32*dpiRatio, 32*dpiRatio);
    eqBandQualSlider[idx]->setStyleSheet(sliderStyle);
    eqBandQualSlider[idx]->setMaximum(5);
    eqBandQualSlider[idx]->setValue(0);
    eqBandQualSlider[idx]->setPageStep(0);
    vlayout->addWidget(eqBandQualSlider[idx]);

    //end

    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    vlayout->addItem(verticalSpacer);

    widget->setLayout(vlayout);

    return widget;
}

void EqualizerWidget::connectSignals(){
}

void EqualizerWidget::disconnectSignals(){
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
}

void eqTablePaint::paintLine(QPainter &painter, QSize paintCacheSize){
    qreal freqScale = paintCacheSize.width() / qreal(201);
    qreal gainScale = paintCacheSize.height() / qreal(60);

    QPointF zero = QPointF(0, paintCacheSize.height() / 2);

    QPointF end = QPointF(paintCacheSize.width(), paintCacheSize.height() / 2);

    painter.setPen(linePen);
    QPainterPath painterpath(zero);
    painterpath.lineTo(end);

    painter.drawPath(painterpath);

    QTransform transform = painter.transform();
    transform.scale(2.5, 2.);
    painter.setTransform(transform);
}
