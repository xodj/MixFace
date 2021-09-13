#ifndef MIXFACEEQUALIZERWIDGET_H
#define MIXFACEEQUALIZERWIDGET_H

#include <QtWidgets>
#include "MixFaceFonts.h"
#include "MixFaceEnumTypes.h"

class EqualizerWidget : public QWidget {
    Q_OBJECT
public:
    explicit EqualizerWidget(float dpiRatio, MixFaceFonts *m_fonts_ = nullptr);

    void setIdx(int idx_){ idx = idx_; };
    int getIdx(){ return idx; };

signals:

private:
    MixFaceFonts *m_fonts;
    float dpiRatio = 1.f;
    int idx = 0;
};

#endif // MIXFACEEQUALIZERWIDGET_H
