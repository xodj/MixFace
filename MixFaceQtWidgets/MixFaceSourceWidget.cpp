#include "MixFaceSourceWidget.h"

SourceWidget::SourceWidget(int idx, float dpiRatio, MixFaceFonts *m_fonts_)
    :m_fonts(m_fonts_)
{
    Q_UNUSED(dpiRatio)

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->setSpacing(0);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(0, 0, 0, 0);
    vlayout->setSpacing(0);
    hlayout->addLayout(vlayout);

    QVBoxLayout *vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(0, 0, 0, 0);
    vlayout_->setSpacing(0);

    QLabel *label = new QLabel("SRC #" + QString::number(idx));
    vlayout_->addWidget(label);

    QFrame *frame = new QFrame;
    frame->setLayout(vlayout_);
    frame->setFrameShape(QFrame::Box);
    frame->setFrameShadow(QFrame::Plain);
    vlayout->addWidget(frame);

    vlayout_ = new QVBoxLayout;
    vlayout_->setContentsMargins(0, 0, 0, 0);
    vlayout_->setSpacing(0);

    label = new QLabel("SRC #" + QString::number(idx));
    vlayout_->addWidget(label);

    frame = new QFrame;
    frame->setLayout(vlayout_);
    frame->setFrameShape(QFrame::Box);
    frame->setFrameShadow(QFrame::Plain);
    vlayout->addWidget(frame);

    this->setLayout(hlayout);
}
