#ifndef MIXFACEEQUALIZERWIDGET_H
#define MIXFACEEQUALIZERWIDGET_H

#include <QtWidgets>
#include "MixFaceFonts.h"
#include "MixFaceEnumTypes.h"

class eqTablePaint;
class eqTableTimer;

class EqualizerWidget : public QWidget {
    Q_OBJECT
public:
    explicit EqualizerWidget(float dpiRatio, MixFaceFonts *m_fonts_ = nullptr);
    ~EqualizerWidget();

    void setIdx(int idx_);
    int getIdx(){ return idx; };

    void eqOnOffRecieved(int state);
    void eqBandTypeRecieved(int bandId, int type);
    void eqBandGainRecieved(int bandId, float gain);
    void eqBandFreqRecieved(int bandId, float freq);
    void eqBandQualRecieved(int bandId, float qual);

signals:
    void eqOnOffChanged(int state);
    void eqBandTypeChanged(int bandId, int type);
    void eqBandGainChanged(int bandId, float gain);
    void eqBandFreqChanged(int bandId, float freq);
    void eqBandQualChanged(int bandId, float qual);

private:
    QFrame *topFrame();
    QFrame *bottomFrame();
    QWidget *bandWidget(int idx);

    void connectSignals();
    void disconnectSignals();

    void emitEqOnOffClicked();
    void emitEqResetClicked();
    void emitEqRtaClicked();

    void emitEqBandTypeChanged();
    void emitEqBandResetClicked();
    void emitEqBandGainChanged();
    void emitEqBandFreqChanged();
    void emitEqBandQualChanged();

    QPushButton *eqActivePushButton;
    QPushButton *eqResetPushButton;
    QPushButton *eqRtaPushButton;

    QWidget *bandWidgetList[6];
    QPushButton *eqBandResetPushButton[6];
    QComboBox *eqBandTypeComboBox[6];
    QSlider *eqBandGainSlider[6];
    QLineEdit *eqBandGainLine[6];
    QSlider *eqBandFreqSlider[6];
    QLineEdit *eqBandFreqLine[6];
    QSlider *eqBandQualSlider[6];
    QLineEdit *eqBandQualLine[6];

    MixFaceFonts *m_fonts;
    eqTablePaint *eqTable;
    eqTableTimer *eqTimer;
    float dpiRatio = 1.f;
    int idx = 0;

    //QSS
    QString lineStyle;
    QString sliderStyle;
    QString buttonStyle;
};

class eqTablePaint : public QWidget {
    Q_OBJECT
#define fftsize 201

public:
    explicit eqTablePaint(float dpiRatio_);
    void setOn(bool on_){
        on = on_;
        repaint = true;
    }
    void setBandType(int bandid, int type){
        eqtype[bandid] = type;
        repaint = true;
    }
    void setBandGain(int bandid, float gain){
        eqgain[bandid] = gain - 0.5f;
        repaint = true;
    }
    void setBandFreq(int bandid, float freq){
        eqfreq[bandid] = freq * fftsize;
        repaint = true;
    }
    void setBandQual(int bandid, float qual){
        eqqual[bandid] = 30 / qual;
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

    int eqtype[6] = {2,2,2,2,2,2};
    float eqgain[6] = {0.f,0.f,0.f,0.f,0.f,0.f};
    int eqfreq[6] = {0,0,0,0,0,0};
    float eqqual[6] = {0.4648f,0.4648f,0.4648f,0.4648f,0.4648f,0.4648f};

    float eqspline[fftsize];

    float dpiRatio = 1;
    bool on = true;
    bool repaint = true;

    void paintSquares(QPainter &painter, QSize paintCacheSize);
    void paintLine(QPainter &painter, QSize paintCacheSize);

protected:
    void paintEvent(QPaintEvent *event) override;

};

class eqTableTimer : public QTimer {
    Q_OBJECT

public:
    inline eqTableTimer() : QTimer() {}

    void addEqTablePaint(eqTablePaint *eqTable_) {
        eqTable = eqTable_;
    }

private:
    eqTablePaint *eqTable;

protected:
    void timerEvent(QTimerEvent *event) override {
        Q_UNUSED(event)
        eqTable->update();
    }
};

#endif // MIXFACEEQUALIZERWIDGET_H
