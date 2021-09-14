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

    void compOnOffRecieved(int value);
    void compTresholdRecieved(float value);
    void compRatioRecieved(int value);

    void compMixRecieved(float value);
    void compGainRecieved(float value);
    void compKneeRecieved(float value);

    void compModeCompExpRecieved(int value);
    void compEnvLinLogRecieved(int value);
    void compDetPeakRmsRecieved(int value);

    void compAttackRecieved(float value);
    void compHoldRecieved(float value);
    void compReleaseRecieved(float value);
    void compAutoTimeRecieved(int value);

    void compKeySourceRecieved(int value);
    void compFilterRecieved(int value);
    void compFilterSoloRecieved(int value);
    void compFilterTypeRecieved(int value);
    void compFilterFrequencyRecieved(float value);

signals:
    void compOnOffChanged(int value);
    void compTresholdChanged(float value);
    void compRatioChanged(int value);

    void compMixChanged(float value);
    void compGainChanged(float value);
    void compKneeChanged(float value);

    void compModeCompExpChanged(int value);
    void compEnvLinLogChanged(int value);
    void compDetPeakRmsChanged(int value);

    void compAttackChanged(float value);
    void compHoldChanged(float value);
    void compReleaseChanged(float value);
    void compAutoTimeChanged(int value);

    void compKeySourceChanged(int value);
    void compFilterChanged(int value);
    void compFilterSoloChanged(int value);
    void compFilterTypeChanged(int value);
    void compFilterFrequencyChanged(float value);

private:
    QFrame *compWidget();
    QWidget *compRadioWidget();
    QWidget *compGainWidget();
    QWidget *compEnvWidget();
    QWidget *compFltrWidget();
    QFrame *gateWidget();
    void connectSignals();

    void emitCompOnOffChanged();
    void emitCompTresholdChanged();
    void emitCompRatioChanged();

    void emitCompMixChanged();
    void emitCompGainChanged();
    void emitCompKneeChanged();

    void emitCompModeCompExpChanged();
    void emitCompEnvLinLogChanged();
    void emitCompDetPeakRmsChanged();

    void emitCompAttackChanged();
    void emitCompHoldChanged();
    void emitCompReleaseChanged();
    void emitCompAutoTimeChanged();

    void emitCompKeySourceChanged();
    void emitCompFilterChanged();
    void emitCompFilterSoloChanged();
    void emitCompFilterTypeChanged();
    void emitCompFilterFrequencyChanged();

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
