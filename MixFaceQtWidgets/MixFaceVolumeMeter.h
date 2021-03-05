#ifndef MIXFACEVOLUMEMETER_H
#define MIXFACEVOLUMEMETER_H

#include <QWidget>
#include <QElapsedTimer>
#include <QMutex>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>

#include "DebugLibrary.hpp"

#define MAX_AUDIO_CHANNELS 2
#define M_INFINITE 3.4e38f
#define CLIP_FLASH_DURATION_MS 1000
#define CLAMP(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

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

#endif // MIXFACEVOLUMEMETER_H
