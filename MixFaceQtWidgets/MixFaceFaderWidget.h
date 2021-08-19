#ifndef MIXFACEFADERWIDGET_H
#define MIXFACEFADERWIDGET_H

#include <QtWidgets>
#include "MixFaceFonts.h"
#include "MixFaceEnumTypes.h"
#include "DebugLibrary.hpp"
#include "MixFaceVolumeMeter.h"

#define MAX_AUDIO_CHANNELS 2
#define M_INFINITE 3.4e38f
#define CLIP_FLASH_DURATION_MS 1000
#define CLAMP(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

class dynMeter;

class MixFaceMetersTimer;

class TicksPaint;

class FaderWidget : public QWidget {
    Q_OBJECT

public:
    explicit FaderWidget(float dpiRatio, MixFaceFonts *m_fonts,
                         MixFaceMetersTimer *mf_metersTimer, DebugLibrary *debug);
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

    void setMeter(float preL_, float preR_);
    void setDynamics(float comp);
    void setGate(float gate);

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
    int colorNumber = 0;
    QString BWIcon = "W";
    QWidget *botArea;

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
    void emitPanChanged();
    void sliderReleased();
    int lastSliderReleasedTime = 0;
    void dbEditingFinished();

    MixFaceVolumeMeter *m_vmeter;
    dynMeter *m_dmeter;
    MixFaceMetersTimer *mf_metersTimer;
    DebugLibrary *debug;

    bool fader_lock = false;
    bool pan_lock = false;
    bool mute_lock = false;
    bool solo_lock = false;

protected:
    void resizeEvent( QResizeEvent *e ) override;
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
    explicit dynMeter(float dpiRatio_, QWidget *parent_);
    void setDynamics(float comp_);
    void setGate(float gate_);
    void enableGate(bool gate_){
        gate = gate_;
        delete dynMeterCache;
        dynMeterCache = nullptr;
    }

private:
    QPainter *painter;
    QPixmap *dynMeterCache;
    QColor backColor;
    QColor treshColor;
    float dpiRatio = 1;
    QSize dynMeterSize;
    float currentGate = 0.f;
    float currentComp = 0.f;
    bool gate = false;

    void paintCache(QPainter &painter, QSize dynMeterSize);
    void paintComp(QPainter &painter, QSize dynMeterSize);
    void paintGate(QPainter &painter, QSize dynMeterSize);

protected:
    void paintEvent(QPaintEvent *event) override;

};

class MixFaceMetersTimer : public QTimer {
    Q_OBJECT

public:
    inline MixFaceMetersTimer() : QTimer() {}

    void AddVolumeMeter(MixFaceVolumeMeter *meter) {
        volumeMeters.push_back(meter);
    }

    void RemoveVolumeMeter(MixFaceVolumeMeter *meter) {
        volumeMeters.removeOne(meter);
    }

    void AddDynMeter(dynMeter *meter) {
        dynMeters.push_back(meter);
    }

    void RemoveDynMeter(dynMeter *meter) {
        dynMeters.removeOne(meter);
    }

protected:
    void timerEvent(QTimerEvent *event) override {
        Q_UNUSED(event)
        for(MixFaceVolumeMeter *v_meter:volumeMeters)
            v_meter->update();
        for(dynMeter *d_meter:dynMeters)
            d_meter->update();
    }

    QList<MixFaceVolumeMeter *> volumeMeters;
    QList<dynMeter *> dynMeters;
};
#endif
