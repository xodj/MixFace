#include "MixFaceVolumeMeter.h"
#include "MixFaceStaticMath.h"

MixFaceVolumeMeter::MixFaceVolumeMeter(QWidget *parent, DebugLibrary *debug_,
                                       float dpiRatio_)
    : QWidget(parent),
      dpiRatio(dpiRatio_),
      debug(debug_)
{
    backgroundNominalColor.setRgb(0x26, 0x7f, 0x26); // Dark green
    backgroundWarningColor.setRgb(0x7f, 0x7f, 0x26); // Dark yellow
    backgroundErrorColor.setRgb(0x7f, 0x26, 0x26);   // Dark red
    foregroundNominalColor.setRgb(0x4c, 0xff, 0x4c); // Bright green
    foregroundWarningColor.setRgb(0xff, 0xff, 0x4c); // Bright yellow
    foregroundErrorColor.setRgb(0xff, 0x4c, 0x4c);   // Bright red
    clipColor.setRgb(0xff, 0xff, 0xff);              // Bright white
    magnitudeColor.setRgb(0x00, 0x00, 0x00);         // Black
    majorTickColor.setRgb(0xff, 0xff, 0xff);         // Black
    minorTickColor.setRgb(0xcc, 0xcc, 0xcc);         // Black
    minimumLevel = -60.0;                            // -60 dB
    warningLevel = -10.0;                            // -20 dB
    errorLevel = -5.0;                               //  -9 dB
    clipLevel = -0.5;                                //  -0.5 dB
    minimumInputLevel = -90.0;                       // -50 dB
    peakDecayRate = 23.53;                           //  20 dB / 1.7 sec
    magnitudeIntegrationTime = 0.3;                  //  99% in 300 ms
    peakHoldDuration = 3.0;                         //  20 seconds
    inputPeakHoldDuration = 1.0;                     //  1 second

    channels = 1;
    displayNrAudioChannels = 1;
    currentPeak[0] = -M_INFINITE;
    currentInputPeak[0] = -M_INFINITE;
    currentInputPeak[0] = -M_INFINITE;
    currentPeak[1] = -M_INFINITE;
    currentInputPeak[1] = -M_INFINITE;
    currentInputPeak[1] = -M_INFINITE;

    elapsedTimer = new QElapsedTimer;
    elapsedTimer->start();
    setMinimumSize(displayNrAudioChannels * 20, 50);
    setMaximumWidth(displayNrAudioChannels * 20);
}

void MixFaceVolumeMeter::setMeter(float preL_, float preR_) {
    QMutexLocker locker(&dataMutex);
    uint64_t ts = elapsedTimer->nsecsElapsed();
    currentLastUpdateTime = ts;
    if (displayNrAudioChannels==1){
        currentPeak[0] = log10(preL_) * 20; //пик значение
        currentInputPeak[0] = currentPeak[0]; //индикатор сигнала
        currentMagnitude[0] = currentPeak[0]; //среднее значение
    } else {
        currentPeak[0] = log10(preL_) * 20;
        currentPeak[1] = log10(preR_) * 20;
        currentInputPeak[0] = currentPeak[0];
        currentInputPeak[1] = currentPeak[1];
        currentMagnitude[0] = currentPeak[0];
        currentMagnitude[1] = currentPeak[1];
    }
    locker.unlock();
    calculateBallistics(ts);
}

void MixFaceVolumeMeter::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    const QRect rect = event->region().boundingRect();
    int height = rect.height();

    uint64_t ts = elapsedTimer->nsecsElapsed();
    qreal timeSinceLastRedraw = (ts - lastRedrawTime) * 0.000000001;

    calculateBallistics(ts, timeSinceLastRedraw);
    bool idle = detectIdle(ts);

    QPainter painter(this);
    painter.translate(0, height);
    painter.scale(1, -1);

    for (int channelNr = 0; channelNr < displayNrAudioChannels;
         channelNr++) {
        paintVMeter(painter, channelNr * 20, 27, 18, height * 0.75 - 44,
                    displayMagnitude[channelNr],
                    displayPeak[channelNr],
                    displayPeakHold[channelNr]);
        if (idle)
            continue;
        paintInputMeter(painter, channelNr * 20, 3, 18, 18, displayInputPeakHold[channelNr]);
    }

    lastRedrawTime = ts;
}

void MixFaceVolumeMeter::paintVMeter(QPainter &painter, int x, int y, int width,
                           int height, float magnitude, float peak,
                           float peakHold)
{
    qreal scale = height / minimumLevel;

    QMutexLocker locker(&dataMutex);
    int minimumPosition = y;
    int maximumPosition = y + height;
    int magnitudePosition = int(y + height - (magnitude * scale));
    int peakPosition = int(y + height - (peak * scale));
    int peakHoldPosition = int(y + height - (peakHold * scale));
    int warningPosition = int(y + height - (warningLevel * scale));
    int errorPosition = int(y + height - (errorLevel * scale));

    int nominalLength = warningPosition - minimumPosition;
    int warningLength = errorPosition - warningPosition;
    int errorLength = maximumPosition - errorPosition;
    locker.unlock();

    if (clipping) {
        peakPosition = maximumPosition;
    }

    if (peakPosition < minimumPosition) {
        painter.fillRect(x, minimumPosition, width, nominalLength,
                         backgroundNominalColor);
        painter.fillRect(x, warningPosition, width, warningLength,
                         backgroundWarningColor);
        painter.fillRect(x, errorPosition, width, errorLength,
                         backgroundErrorColor);
    } else if (peakPosition < warningPosition) {
        painter.fillRect(x, minimumPosition, width,
                         peakPosition - minimumPosition,
                         foregroundNominalColor);
        painter.fillRect(x, peakPosition, width,
                         warningPosition - peakPosition,
                         backgroundNominalColor);
        painter.fillRect(x, warningPosition, width, warningLength,
                         backgroundWarningColor);
        painter.fillRect(x, errorPosition, width, errorLength,
                         backgroundErrorColor);
    } else if (peakPosition < errorPosition) {
        painter.fillRect(x, minimumPosition, width, nominalLength,
                         foregroundNominalColor);
        painter.fillRect(x, warningPosition, width,
                         peakPosition - warningPosition,
                         foregroundWarningColor);
        painter.fillRect(x, peakPosition, width,
                         errorPosition - peakPosition,
                         backgroundWarningColor);
        painter.fillRect(x, errorPosition, width, errorLength,
                         backgroundErrorColor);
    } else if (peakPosition < maximumPosition) {
        painter.fillRect(x, minimumPosition, width, nominalLength,
                         foregroundNominalColor);
        painter.fillRect(x, warningPosition, width, warningLength,
                         foregroundWarningColor);
        painter.fillRect(x, errorPosition, width,
                         peakPosition - errorPosition,
                         foregroundErrorColor);
        painter.fillRect(x, peakPosition, width,
                         maximumPosition - peakPosition,
                         backgroundErrorColor);
    } else {
        if (!clipping) {
            QTimer::singleShot(CLIP_FLASH_DURATION_MS, this,
                               SLOT(ClipEnding()));
            clipping = true;
        }

        int end = errorLength + warningLength + nominalLength;
        painter.fillRect(x, minimumPosition, width, end,
                         QBrush(foregroundErrorColor));
    }

    if (peakHoldPosition - 3 < minimumPosition)
        ; // Peak-hold below minimum, no drawing.
    else if (peakHoldPosition < warningPosition)
        painter.fillRect(x, peakHoldPosition - 3, width, 3,
                         foregroundNominalColor);
    else if (peakHoldPosition < errorPosition)
        painter.fillRect(x, peakHoldPosition - 3, width, 3,
                         foregroundWarningColor);
    else
        painter.fillRect(x, peakHoldPosition - 3, width, 3,
                         foregroundErrorColor);

    if (magnitudePosition - 3 >= minimumPosition)
        painter.fillRect(x, magnitudePosition - 3, width, 3,
                         magnitudeColor);
    locker.unlock();
}

void MixFaceVolumeMeter::paintInputMeter(QPainter &painter, int x, int y, int width,
                               int height, float peakHold)
{
    QMutexLocker locker(&dataMutex);
    QColor color;

    if (peakHold < minimumInputLevel)
        color = backgroundNominalColor;
    else if (peakHold < warningLevel)
        color = foregroundNominalColor;
    else if (peakHold < errorLevel)
        color = foregroundWarningColor;
    else if (peakHold <= clipLevel)
        color = foregroundErrorColor;
    else
        color = clipColor;

    painter.fillRect(x, y, width, height, color);
}

inline void MixFaceVolumeMeter::calculateBallistics(uint64_t ts,
                                          qreal timeSinceLastRedraw)
{
    QMutexLocker locker(&dataMutex);

    for (int channelNr = 0; channelNr < MAX_AUDIO_CHANNELS; channelNr++)
        calculateBallisticsForChannel(channelNr, ts,
                                      timeSinceLastRedraw);
    locker.unlock();
}

inline bool MixFaceVolumeMeter::detectIdle(uint64_t ts)
{
    double timeSinceLastUpdate = (ts - currentLastUpdateTime) * 0.000000001;
    if (timeSinceLastUpdate > 0.5) {
        resetLevels();
        return true;
    } else {
        return false;
    }
}

void MixFaceVolumeMeter::resetLevels()
{
    currentLastUpdateTime = 0;
    for (int channelNr = 0; channelNr < MAX_AUDIO_CHANNELS; channelNr++) {
        currentMagnitude[channelNr] = -M_INFINITE;
        currentPeak[channelNr] = -M_INFINITE;
        currentInputPeak[channelNr] = -M_INFINITE;

        displayMagnitude[channelNr] = -M_INFINITE;
        displayPeak[channelNr] = -M_INFINITE;
        displayPeakHold[channelNr] = -M_INFINITE;
        displayPeakHoldLastUpdateTime[channelNr] = 0;
        displayInputPeakHold[channelNr] = -M_INFINITE;
        displayInputPeakHoldLastUpdateTime[channelNr] = 0;
    }
}

inline void MixFaceVolumeMeter::calculateBallisticsForChannel(int channelNr, uint64_t ts,
                                                    qreal timeSinceLastRedraw)
{
    if (currentPeak[channelNr] >= displayPeak[channelNr] ||
            isnan(displayPeak[channelNr])) {
        // Attack of peak is immediate.
        displayPeak[channelNr] = currentPeak[channelNr];
    } else {
        // Decay of peak is 40 dB / 1.7 seconds for Fast Profile
        // 20 dB / 1.7 seconds for Medium Profile (Type I PPM)
        // 24 dB / 2.8 seconds for Slow Profile (Type II PPM)
        float decay = float(peakDecayRate * timeSinceLastRedraw);
        displayPeak[channelNr] = CLAMP(displayPeak[channelNr] - decay,
                                       currentPeak[channelNr], 0);
    }

    if (currentPeak[channelNr] >= displayPeakHold[channelNr] ||
            !isfinite(displayPeakHold[channelNr])) {
        // Attack of peak-hold is immediate, but keep track
        // when it was last updated.
        displayPeakHold[channelNr] = currentPeak[channelNr];
        displayPeakHoldLastUpdateTime[channelNr] = ts;
    } else {
        // The peak and hold falls back to peak
        // after 20 seconds.
        qreal timeSinceLastPeak =
                (uint64_t)(ts -
                           displayPeakHoldLastUpdateTime[channelNr]) *
                0.000000001;
        if (timeSinceLastPeak > peakHoldDuration) {
            displayPeakHold[channelNr] = currentPeak[channelNr];
            displayPeakHoldLastUpdateTime[channelNr] = ts;
        }
    }

    if (currentInputPeak[channelNr] >= displayInputPeakHold[channelNr] ||
            !isfinite(displayInputPeakHold[channelNr])) {
        // Attack of peak-hold is immediate, but keep track
        // when it was last updated.
        displayInputPeakHold[channelNr] = currentInputPeak[channelNr];
        displayInputPeakHoldLastUpdateTime[channelNr] = ts;
    } else {
        // The peak and hold falls back to peak after 1 second.
        qreal timeSinceLastPeak =
                (uint64_t)(
                    ts -
                    displayInputPeakHoldLastUpdateTime[channelNr]) *
                0.000000001;
        if (timeSinceLastPeak > inputPeakHoldDuration) {
            displayInputPeakHold[channelNr] = currentInputPeak[channelNr];
            displayInputPeakHoldLastUpdateTime[channelNr] = ts;
        }
    }

    if (!isfinite(displayMagnitude[channelNr])) {
        // The statements in the else-leg do not work with
        // NaN and infinite displayMagnitude.
        displayMagnitude[channelNr] = currentMagnitude[channelNr];
    } else {
        // A VU meter will integrate to the new value to 99% in 300 ms.
        // The calculation here is very simplified and is more accurate
        // with higher frame-rate.
        float attack =
                float((currentMagnitude[channelNr] -
                       displayMagnitude[channelNr]) *
                      (timeSinceLastRedraw / magnitudeIntegrationTime) *
                      0.99);
        displayMagnitude[channelNr] =
                CLAMP(displayMagnitude[channelNr] + attack,
                      (float)minimumLevel, 0);
    }
}

void MixFaceVolumeMeter::ClipEnding() { clipping = false; }
