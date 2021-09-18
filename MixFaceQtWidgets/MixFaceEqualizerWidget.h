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

    void setIdx(int idx_){ idx = idx_; };
    int getIdx(){ return idx; };

signals:

private:
    QFrame *topFrame();
    QFrame *bottomFrame();
    QWidget *bandWidget(int idx);

    void connectSignals();
    void disconnectSignals();

    QPushButton *eqActivePushButton;
    QPushButton *eqResetPushButton;
    QPushButton *eqRtaPushButton;

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

public:
    explicit eqTablePaint(float dpiRatio_);
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

    int eq1type = 1;
    float eq1g = 0.5000f;
    float eq1f = 0.2000f;
    float eq1q = 0.4648f;
    int eq2type = 2;
    float eq2g = 0.5000f;
    float eq2f = 0.3000f;
    float eq2q = 0.4648f;
    int eq3type = 2;
    float eq3g = 0.4000f;
    float eq3f = 0.4650f;
    float eq3q = 0.4648f;
    int eq4type = 2;
    float eq4g = 0.8000f;
    float eq4f = 0.6650f;
    float eq4q = 0.4648f;
    int eq5type = 2;
    float eq5g = 0.3000f;
    float eq5f = 0.8000f;
    float eq5q = 0.4648f;
    int eq6type = 4;
    float eq6g = 0.5000f;
    float eq6f = 0.9000f;
    float eq6q = 0.4648f;

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
