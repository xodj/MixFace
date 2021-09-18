#ifndef MIXFACEVOLUMEMETER_H
#define MIXFACEVOLUMEMETER_H

#include <QWidget>
#include <QElapsedTimer>
#include <QMutex>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>

#include "DebugLibrary.hpp"
#include "MixFaceStatic.h"

#define CLIP_FLASH_DURATION_MS 1000

#define peakHoldMS 3000
#define tickHoldMS 1500
#define peakDecayRate .000365f

class MixFaceVolumeMeter : public QWidget
{
    Q_OBJECT
public:
    explicit MixFaceVolumeMeter(QWidget *parent = nullptr,
                                DebugLibrary *debug_ = new DebugLibrary,
                                float dpiRatio_ = 1.f);
    ~MixFaceVolumeMeter(){}

    void setChannels(int channels_){
        channels = channels_;
        setMinimumSize(channels * meterWSize, 50);
        setMaximumWidth(channels * meterWSize);
    };

    void setMeter(float channel0, float channel1 = 0.f){
        currentPeak[0] = meterLogToFloatRatio(channel0);
        currentPeak[1] = meterLogToFloatRatio(channel1);
    }

private:
    void render(QPainter& drawControl, QPaintEvent *event);
    void renderBackground();
    QPixmap renderPeak(int redraw, int channel);
    QPixmap renderMeter(int redraw, int channel);
    void calculateDecay(int redraw, int channel);
    //Cache
    QPixmap *backgroundMeter;
    //Colors
    QColor blackColor = QColor(32, 32, 32);
    QColor whiteColor = QColor(222, 222, 222);
    QColor redColor = QColor(255, 32, 32);
    QColor lightGreenColor = QColor(55, 239, 70);
    QColor lightYellowColor = QColor(245, 209, 70);
    QColor lightRedColor = QColor(245, 70, 70);
    QColor lowGreenColor = QColor(45, 137, 52);
    QColor lowYellowColor = QColor(140, 122, 52);
    QColor lowRedColor = QColor(140, 52, 52);
    //Sizing
    const float redAspect = 0.1f;
    const float yellowAspect = 0.15f;
    int redSize;
    int yellowSize;
    int greenSize;
    int meterHSize;
    int panelWSize;
    int panelHSize = -1;
    const int borders = 0;
    int meterWSize = 16;
    int peakHSize = 8;
    const int frame = 0;
    //Variables
    float currentPeak[2] = {0.f, 0.f};
    float displayPeak[2] = {0.f, 0.f};
    int lastRedraw[2] = {0, 0};
    int lastTickRedraw[2] = {0, 0};
    int lastTickSize[2] = {0, 0};
    float lastTickPeak[2] = {0.f, 0.f};
    int lastPeakRedraw[2] = {0, 0};
    bool drawFrame = true;
    int channels = 1;

    DebugLibrary *debug;

protected:
    void paintEvent(QPaintEvent *event) override;

};

#endif // MIXFACEVOLUMEMETER_H
