#ifndef MIXFACEFADERWIDGET_H
#define MIXFACEFADERWIDGET_H

#include <QtWidgets>
#include "MixFaceFonts.h"
#include "MixFaceEnumTypes.h"
#include "DebugLibrary.hpp"

#define MAX_AUDIO_CHANNELS 2
#define M_INFINITE 3.4e38f
#define CLIP_FLASH_DURATION_MS 1000
#define CLAMP(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

class volMeter;

class dynMeter;

class MetersTimer;

class TicksPaint;

class FaderWidget : public QWidget {
    Q_OBJECT

public:
    explicit FaderWidget(float dpiRatio, DebugLibrary *debug = nullptr, MixFaceFonts *m_fonts = nullptr, QMainWindow *mainWindow = nullptr);
    ~FaderWidget();

    void setFaderType(FaderType ftype);
    void setChannelNativeName(QString value);

    void setFaderValue(float value);
    void setPanValue(float value);
    void setMute(bool value);
    void setSolo(bool value);

    void setLogo(int value);
    void setColor(int value);
    void setName(QString value);

    void setMeter(float preL_, float preR_, float gate, float comp);

signals:
    void faderChanged(float value);
    void panChanged(float value);
    void muteChanged(int value);
    void soloChanged(int value);
    void iconButtonClicked();

    void srcClicked();
    void eqClicked();
    void dynClicked();

private:
    float dpiRatio = 1;
    int iconNumber = 1;

    QSlider *volSlider;
    TicksPaint *ticks;
    QWidget *volArea;
    MixFaceFonts *m_fonts;

    QMainWindow *mainWindow;
    QSlider *panSlider;
    QFrame *iconPopup;
    QPushButton *channelName;
    QPushButton *mute;
    QPushButton *solo;
    QPushButton *src;
    QPushButton *eq;
    QPushButton *dyn;
    QPushButton *icon;
    QLabel *nativeName;
    QLineEdit *db;

    void initWidget();
    void connectWidgets();
    void emitMuteChanged();
    void emitSoloChanged();

    void emitFaderChanged();
    void dbEditingFinished();

    volMeter *m_vmeter;
    dynMeter *m_dmeter;
    MetersTimer *metersRenew;
    DebugLibrary *debug;

protected:
    void resizeEvent( QResizeEvent *e ) override;
};

class volMeter : public QWidget {
    Q_OBJECT

public:
    explicit volMeter();
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

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:

    void ClipEnding();

};

class TicksPaint : public QWidget {
    Q_OBJECT

public:
    explicit TicksPaint(float dpiRatio_);

private:
    QPainter *painter;
    QPixmap *tickPaintCache;
    QFont tickFont;
    QColor majorTickColor;
    QColor minorTickColor;
    qreal minimumLevel;
    qreal maximumLevel;
    float dpiRatio = 1;

    void paintVTicks(QPainter &painter, int x, int y, int height);

protected:
    void paintEvent(QPaintEvent *event) override;

};

class dynMeter : public QWidget {
    Q_OBJECT

public:
    explicit dynMeter(float dpiRatio_,QWidget *parent_);
    void setlevel(float comp_, float gate_);

private:
    QPainter *painter;
    QPixmap *dynMeterCache;
    QColor backColor;
    QColor treshColor;
    float dpiRatio = 1;
    QSize dynMeterSize;
    float currentGate;
    float currentComp;

    void paintCache(QPainter &painter, QSize dynMeterSize);
    void paintComp(QPainter &painter, QSize dynMeterSize);
    void paintGate(QPainter &painter, QSize dynMeterSize);

protected:
    void paintEvent(QPaintEvent *event) override;

};

class MetersTimer : public QTimer {
    Q_OBJECT

public:
    volMeter *m_vmeter;
    dynMeter *m_dmeter;
    inline MetersTimer(volMeter *m_vmeter_,dynMeter *m_dmeter_) : QTimer() {
        m_vmeter = m_vmeter_;
        m_dmeter = m_dmeter_;
    }

protected:
    void timerEvent(QTimerEvent *event) override {
        Q_UNUSED(event)
        m_vmeter->update();
        m_dmeter->update();
    }
};
#endif
