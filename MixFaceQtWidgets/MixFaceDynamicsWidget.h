#ifndef MIXFACEDYNAMICSWIDGET_H
#define MIXFACEDYNAMICSWIDGET_H

#include <QWidget>
#include "MixFaceFonts.h"
#include "MixFaceEnumTypes.h"
#include "MixFaceFaderWidget.h"

class compGainPaint;

class compEnvPaint;

class tablesTimer;

class DynamicsWidget : public QWidget {
    Q_OBJECT
public:
    explicit DynamicsWidget(int idx, float dpiRatio, MixFaceFonts *m_fonts_ = nullptr);
    ~DynamicsWidget();

signals:
    void setOn(int value);
    void tresholdChanged(float value);
    void ratioChanged(int value);

    void mixChanged(float value);
    void gainChanged(float value);
    void kneeChanged(float value);

    void typeChanged(int value);
    void linLogChanged(int value);
    void peakRmsChanged(int value);

    void attackChanged(float value);
    void holdChanged(float value);
    void releaseChanged(float value);
    void autoTimeChanged(int value);

    void keySourceChanged(int value);
    void filterChanged(int value);
    void filterTypeChanged(int value);
    void filterFrequencyChanged(float value);

private:
    QFrame *compWidget(float dpiRatio);
    QFrame *gateWidget(int idx);
    void connectSignals();

    void emitSetOnChanged();
    void emitTresholdChanged();
    void emitRatioChanged();

    void emitMixChanged();
    void emitGainChanged();
    void emitKneeChanged();

    void emitTypeChanged();
    void emitLinLogChanged();
    void emitPeakRmsChanged();

    void emitAttackChanged();
    void emitHoldChanged();
    void emitReleaseChanged();
    void emitAutoTimeChanged();

    void emitKeySourceChanged();
    void emitFilterChanged();
    void emitFilterTypeChanged();
    void emitFilterFrequencyChanged();

    MixFaceFonts *m_fonts;
    compGainPaint *compGain;
    compEnvPaint *compEnv;

    QSlider *tresholdSlider;
    QLineEdit *tresholdLine;

    QSlider *ratioSlider;
    QLineEdit *ratioLine;

    QSlider *mixSlider;
    QLineEdit *mixLine;

    QSlider *gainSlider;
    QLineEdit *gainLine;

    QSlider *kneeSlider;
    QLineEdit *kneeLine;

    QButtonGroup *compType;
    QRadioButton *compTypeRadio;
    QRadioButton *expTypeRadio;

    QButtonGroup *compLinLogType;
    QRadioButton *compLinRadio;
    QRadioButton *compLogRadio;

    QButtonGroup *compPeakRmsType;
    QRadioButton *compPeakRadio;
    QRadioButton *compRmsRadio;

    QSlider *compAttackSlider;
    QLineEdit *compAttackLine;

    QSlider *compHoldSlider;
    QLineEdit *compHoldLine;

    QSlider *compReleaseSlider;
    QLineEdit *compReleaseLine;

    tablesTimer *tablesRenew;
};

class compGainPaint : public QWidget {
    Q_OBJECT

public:
    explicit compGainPaint(float dpiRatio_);
    void setTreshold(float treshold_){
        treshold = treshold_;
        repaint = true;
    }
    void setRatio(float ratio_){
        ratio = ratio_;
        repaint = true;
    }
    void setKnee(int knee_){
        knee = knee_;
        repaint = true;
    }
    void setType(int type_){
        type = type_;
        repaint = true;
    }
    void setOn(bool on_){
        on = on_;
        repaint = true;
    }

private:
    QPainter *painter;
    QPixmap *squaresCache;
    QPixmap *lineCache;

    QColor backColor;
    QColor squaresColor;
    QColor lineColor;
    QPen linePen;

    float dpiRatio = 1;
    float treshold = -16.5;
    float ratio = 2.5;
    int knee = 0;
    int type = 0;
    bool on = true;
    bool repaint = true;

    void paintSquares(QPainter &painter, int height);
    void paintLine(QPainter &painter, int height);

protected:
    void paintEvent(QPaintEvent *event) override;

};

class compEnvPaint : public QWidget {
    Q_OBJECT

public:
    explicit compEnvPaint(float dpiRatio_);
    void setAttack(float attack_){
        attack = attack_;
        repaint = true;
    }
    void setHold(float hold_){
        hold = hold_;
        repaint = true;
    }
    void setRelease(float release_){
        release = release_;
        repaint = true;
    }
    void setAuto(int autoTime_){
        autoTime = autoTime_;
        repaint = true;
    }
    void setOn(bool on_){
        on = on_;
        repaint = true;
    }

private:
    QPainter *painter;
    QPixmap *squaresCache;
    QPixmap *lineCache;

    QColor backColor;
    QColor squaresColor;
    QColor lineColor;
    QPen linePen;

    float dpiRatio = 1;

    float attack = 1;
    float hold = 1;
    float release = 1;
    int autoTime = 0;

    bool on = true;
    bool repaint = true;

    void paintSquares(QPainter &painter, int height);
    void paintLine(QPainter &painter, int height);

protected:
    void paintEvent(QPaintEvent *event) override;

};

class tablesTimer : public QTimer {
    Q_OBJECT

public:
    inline tablesTimer() : QTimer() {}

    void addGainPaint(compGainPaint *compGain_) {
        compGain = compGain_;
    }

    void addEnvPaint(compEnvPaint *compEnv_) {
        compEnv = compEnv_;
    }

private:
    compGainPaint *compGain;
    compEnvPaint *compEnv;

protected:
    void timerEvent(QTimerEvent *event) override {
        Q_UNUSED(event)
        compGain->update();
        compEnv->update();
    }
};
#endif
