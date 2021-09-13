#ifndef MIXFACEDYNAMICSWIDGET_H
#define MIXFACEDYNAMICSWIDGET_H

#include <QWidget>
#include "MixFaceFonts.h"
#include "MixFaceEnumTypes.h"
#include "MixFaceFaderWidget.h"

class compGainPaint;
class tablesTimer;

class DynamicsWidget : public QWidget {
    Q_OBJECT
public:
    explicit DynamicsWidget(float dpiRatio, MixFaceFonts *m_fonts_ = nullptr);
    ~DynamicsWidget();

    void setIdx(int idx_){ idx = idx_; };
    int getIdx(){ return idx; };

    void onOffRecieved(int value);
    void tresholdRecieved(float value);
    void ratioRecieved(int value);

    void mixRecieved(float value);
    void gainRecieved(float value);
    void kneeRecieved(float value);

    void modeCompExpRecieved(int value);
    void envLinLogRecieved(int value);
    void detPeakRmsRecieved(int value);

    void attackRecieved(float value);
    void holdRecieved(float value);
    void releaseRecieved(float value);
    void autoTimeRecieved(int value);

    void keySourceRecieved(int value);
    void filterRecieved(int value);
    void filterTypeRecieved(int value);
    void filterFrequencyRecieved(float value);

signals:
    void onOffChanged(int value);
    void tresholdChanged(float value);
    void ratioChanged(int value);

    void mixChanged(float value);
    void gainChanged(float value);
    void kneeChanged(float value);

    void modeCompExpChanged(int value);
    void envLinLogChanged(int value);
    void detPeakRmsChanged(int value);

    void attackChanged(float value);
    void holdChanged(float value);
    void releaseChanged(float value);
    void autoTimeChanged(int value);

    void keySourceChanged(int value);
    void filterChanged(int value);
    void filterTypeChanged(int value);
    void filterFrequencyChanged(float value);

private:
    QFrame *compWidget();
    QWidget *compRadioWidget();
    QWidget *compGainWidget();
    QWidget *compEnvWidget();
    QWidget *compFltrWidget();
    QFrame *gateWidget();
    void connectSignals();

    void emitOnOffChanged();
    void emitTresholdChanged();
    void emitRatioChanged();

    void emitMixChanged();
    void emitGainChanged();
    void emitKneeChanged();

    void emitModeCompExpChanged();
    void emitEnvLinLogChanged();
    void emitDetPeakRmsChanged();

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

    QPushButton *compActivePushButton;
    QPushButton *compAutoPushButton;
    QPushButton *compFilterPushButton;
    QPushButton *compFilterSoloPushButton;

    QComboBox *compFilterSourceComboBox;

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

    QSlider *compFilterTypeSlider;
    QLineEdit *compFilterTypeLine;

    QSlider *compFilterFreqSlider;
    QLineEdit *compFilterFreqLine;

    tablesTimer *tablesRenew;

    float dpiRatio = 1.f;
    int idx = 0;

    //QSS
    QString lineStyle;
    QString sliderStyle;
    QString buttonStyle;
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

class tablesTimer : public QTimer {
    Q_OBJECT

public:
    inline tablesTimer() : QTimer() {}

    void addGainPaint(compGainPaint *compGain_) {
        compGain = compGain_;
    }

private:
    compGainPaint *compGain;

protected:
    void timerEvent(QTimerEvent *event) override {
        Q_UNUSED(event)
        compGain->update();
    }
};
#endif
