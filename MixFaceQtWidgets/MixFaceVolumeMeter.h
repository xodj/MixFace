#ifndef MIXFACEVOLUMEMETER_H
#define MIXFACEVOLUMEMETER_H

#include <QWidget>
#include <QElapsedTimer>
#include <QMutex>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>

#include "DebugLibrary.hpp"
#include "MixFaceStaticMath.h"

#define MAX_AUDIO_CHANNELS 2
#define M_INFINITE 3.4e38f
#define CLIP_FLASH_DURATION_MS 1000
#define CLAMP(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

#define peakHoldMS 3000
#define tickHoldMS 1500

class MixFaceVolumeMeter : public QWidget
{
    Q_OBJECT
public:
    explicit MixFaceVolumeMeter(QWidget *parent = nullptr,
                                DebugLibrary *debug_ = new DebugLibrary,
                                float dpiRatio_ = 1.f);
    void setChannels(int channels_){
        channels = channels_;
        displayNrAudioChannels = channels_;
        setMinimumSize(displayNrAudioChannels * 20, 50);
        setMaximumWidth(displayNrAudioChannels * 20);
        resetLevels();
    };
    void setMeter(float preL_, float preR_);

private:
    QPainter *painter;
    QElapsedTimer *elapsedTimer;

    QFont tickFont;
    QColor backgroundNominalColor;
    QColor backgroundWarningColor;
    QColor backgroundErrorColor;
    QColor foregroundNominalColor;
    QColor foregroundWarningColor;
    QColor foregroundErrorColor;
    QColor clipColor;
    QColor magnitudeColor;
    QColor majorTickColor;
    QColor minorTickColor;
    qreal minimumLevel;
    qreal warningLevel;
    qreal errorLevel;
    qreal clipLevel;
    qreal minimumInputLevel;
    qreal peakDecayRate;
    qreal magnitudeIntegrationTime;
    qreal peakHoldDuration;
    qreal inputPeakHoldDuration;

    int displayNrAudioChannels = 0;

    uint64_t lastRedrawTime = 0;
    int channels = 0;
    bool clipping = false;
    bool vertical;

    void resetLevels();
    inline void handleChannelCofigurationChange();
    inline bool detectIdle(uint64_t ts);
    inline void calculateBallistics(uint64_t ts,
                    qreal timeSinceLastRedraw = 0.0);
    inline void calculateBallisticsForChannel(int channelNr, uint64_t ts,
                          qreal timeSinceLastRedraw);

    void paintInputMeter(QPainter &painter, int x, int y, int width,
                 int height, float peakHold);
    void paintVMeter(QPainter &painter, int x, int y, int width, int height,
             float magnitude, float peak, float peakHold);

    QMutex dataMutex;

    uint64_t currentLastUpdateTime = 0;
    float currentMagnitude[MAX_AUDIO_CHANNELS];
    float currentPeak[MAX_AUDIO_CHANNELS];
    float currentInputPeak[MAX_AUDIO_CHANNELS];

    float displayMagnitude[MAX_AUDIO_CHANNELS];
    float displayPeak[MAX_AUDIO_CHANNELS];
    float displayPeakHold[MAX_AUDIO_CHANNELS];
    uint64_t displayPeakHoldLastUpdateTime[MAX_AUDIO_CHANNELS];
    float displayInputPeakHold[MAX_AUDIO_CHANNELS];
    uint64_t displayInputPeakHoldLastUpdateTime[MAX_AUDIO_CHANNELS];
    float dpiRatio = 1.;
    DebugLibrary *debug;

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:

    void ClipEnding();

};

class SMixFaceVolumeMeter : public QWidget
{
    Q_OBJECT
public:
    explicit SMixFaceVolumeMeter(QWidget *parent = nullptr,
                                DebugLibrary *debug_ = new DebugLibrary,
                                float dpiRatio_ = 1.f);
    ~SMixFaceVolumeMeter(){}

    void setChannels(int channels_){
        channels = channels_;
        setMinimumSize(channels * meterWSize, 50);
        setMaximumWidth(channels * meterWSize);
    };

    void setMeter(float channel1, float channel2){
        if (channel1 > 1) channel1 = 1.f;
        if (channel1 < 0) channel1 = 0.f;
        if (channel2 > 1) channel2 = 1.f;
        if (channel2 < 0) channel2 = 0.f;
        currentPeak = channel1;
    }

    void setValue(float value) {
        if (value > 1) value = 1.f;
        if (value < 0) value = 0.f;
        currentPeak = value;
    }

private:
    void render(QPainter& drawControl, QPaintEvent *event);
    void renderBackground();
    QPixmap renderPeak(int redraw);
    QPixmap renderMeter(int redraw);
    void calculateDecay(int redraw);

    QPixmap *backgroundMeter;

    QColor bgColor = QColor(227, 227, 227);
    QColor blackColor = QColor(0, 0, 0);
    QColor greenColor = QColor(55, 239, 70);
    QColor yellowColor = QColor(245, 209, 70);
    QColor redColor = QColor(245, 70, 70);
    QColor lgreenColor = QColor(45, 137, 52);
    QColor lyellowColor = QColor(140, 122, 52);
    QColor lredColor = QColor(140, 52, 52);

    QColor majorTickColor = QColor(255, 255, 255);

    float redAspect = 0.1f;
    float yellowAspect = 0.15f;
    int redSize;
    int yellowSize;
    int greenSize;
    int meterHSize;
    int panelWSize;
    int panelHSize = -1;
    int borders = 0;
    int meterWSize = 16;
    int peakHSize = 8;
    float currentPeak = 0.f;
    float displayPeak = 0.f;
    int lastRedraw = 0;
    int lastTickRedraw = 0;
    int lastTickSize = 0;
    float lastTickPeak = 0.f;
    int lastPeakRedraw = 0;
    int frame = 0;
    bool drawFrame = true;

    float peakDecayRate = .000365f;
    int channels = 1;
    DebugLibrary *debug;

protected:
    void paintEvent(QPaintEvent *event) override;

};

#endif // MIXFACEVOLUMEMETER_H
