#include "MixFaceVolumeMeter.h"

MixFaceVolumeMeter::MixFaceVolumeMeter(QWidget *parent, DebugLibrary *debug_,
                                         float dpiRatio_)
    : QWidget(parent), debug(debug_)
{
    Q_UNUSED(dpiRatio_)
    setMinimumSize(channels * (meterWSize + (borders * 2)), 50);
    setMaximumWidth(channels * (meterWSize + (borders * 2)));
    panelWSize = (meterWSize + (borders * 2));
}

void MixFaceVolumeMeter::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    int redraw = std::chrono::duration_cast<std::chrono::milliseconds>
            (std::chrono::system_clock::now().time_since_epoch() %
             std::chrono::seconds{ 1000 }).count();
    calculateDecay(redraw, 0);
    if (channels > 1)
        calculateDecay(redraw, 1);
    const QRect rect = event->region().boundingRect();
    int hsize = rect.height();
    if (hsize != panelHSize) {
        panelHSize = hsize;
        meterHSize = int((panelHSize - (borders * 2) - peakHSize) * 0.70f);
        redSize = int(meterHSize * redAspect);
        yellowSize = int(meterHSize * yellowAspect);
        greenSize = int(meterHSize - redSize - yellowSize);
        renderBackground();
    }
    painter.drawPixmap(QRect(borders, borders + int(panelHSize * 0.3f), meterWSize, peakHSize), renderPeak(redraw, 0));
    painter.drawPixmap(QRect(borders, borders + peakHSize - 1 + int(panelHSize * 0.3f), meterWSize, meterHSize), renderMeter(redraw, 0));
    if (channels > 1){
        painter.drawPixmap(QRect(borders + meterWSize - 1, borders + int(panelHSize * 0.3f), meterWSize, peakHSize), renderPeak(redraw, 1));
        painter.drawPixmap(QRect(borders + meterWSize - 1, borders + peakHSize - 1 + int(panelHSize * 0.3f), meterWSize, meterHSize), renderMeter(redraw, 1));
    }
}

void MixFaceVolumeMeter::renderBackground(){
    backgroundMeter = new QPixmap(meterWSize, meterHSize);
    QPainter backgroundMeterDC(backgroundMeter);
    backgroundMeterDC.fillRect(QRect(0, redSize + yellowSize, meterWSize, greenSize), lowGreenColor);
    backgroundMeterDC.fillRect(QRect(0, redSize, meterWSize, yellowSize), lowYellowColor);
    backgroundMeterDC.fillRect(QRect(0, 0, meterWSize, redSize), lowRedColor);
}

QPixmap MixFaceVolumeMeter::renderPeak(int redraw, int channel){
    if (currentPeak[channel] == 1.f)
        lastPeakRedraw[channel] = redraw;
    QPixmap peak(meterWSize, peakHSize);
    QPainter drawControl(&peak);
    if (currentPeak[channel] == 1.f || (lastPeakRedraw[channel] + peakHoldMS) > redraw){
        drawControl.fillRect(QRect(0, 0, meterWSize, peakHSize), redColor);
        if (drawFrame) {
            drawControl.setPen(blackColor);
            drawControl.drawRect(0, 0, meterWSize - 1, peakHSize - 1);
        }
    } else {
        drawControl.fillRect(QRect(0, 0, meterWSize, peakHSize), lowRedColor);
        if (drawFrame) {
            drawControl.setPen(blackColor);
            drawControl.drawRect(0, 0, meterWSize - 1, peakHSize - 1);
        }
    }
    return peak;
}

QPixmap MixFaceVolumeMeter::renderMeter(int redraw, int channel){
    QPixmap meter(meterWSize, meterHSize);
    QPainter drawControl(&meter);
    drawControl.drawPixmap(QRect(0, 0, meterWSize, meterHSize), *backgroundMeter);
    int peakSize = meterHSize * displayPeak[channel];
    if (peakSize <= greenSize){
        drawControl.fillRect(QRect(0, meterHSize, meterWSize, - peakSize), lightGreenColor);
    } else if (peakSize <= (greenSize + yellowSize)) {
        drawControl.fillRect(QRect(0, meterHSize, meterWSize, - greenSize), lightGreenColor);
        drawControl.fillRect(QRect(0, meterHSize - greenSize, meterWSize,
                                   - (peakSize - greenSize)), lightYellowColor);
    } else {
        drawControl.fillRect(QRect(0, meterHSize, meterWSize, - greenSize), lightGreenColor);
        drawControl.fillRect(QRect(0, meterHSize - greenSize, meterWSize, - yellowSize),
                             lightYellowColor);
        drawControl.fillRect(QRect(0, meterHSize - greenSize - yellowSize,
                                   meterWSize, - (peakSize - greenSize - yellowSize)), lightRedColor);
    }
    //Draw peak tick
    if (peakSize > lastTickSize[channel] || (lastTickRedraw[channel] + tickHoldMS) < redraw) {
        lastTickRedraw[channel] = redraw;
        lastTickSize[channel] = peakSize;
    }
    drawControl.setPen(whiteColor);
    drawControl.drawLine(QLine(0, meterHSize - lastTickSize[channel], meterWSize,
                               meterHSize - lastTickSize[channel]));
    //Draw frame
    if (drawFrame) {
        drawControl.setPen(blackColor);
        drawControl.drawRect(0, 0, meterWSize - 1, meterHSize - 1);
    }
    return meter;
}

void MixFaceVolumeMeter::calculateDecay(int redraw, int channel) {
    if (currentPeak[channel] >= displayPeak[channel] || displayPeak[channel] == 0.f)
        displayPeak[channel] = currentPeak[channel];
    else {
        float decay = float(peakDecayRate * (redraw - lastRedraw[channel]));
        displayPeak[channel] = CLAMP(displayPeak[channel] - decay, currentPeak[channel], 1.);
        }
    lastRedraw[channel] = redraw;
}
