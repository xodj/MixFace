#ifndef MIXFACEEQUALIZERWIDGET_H
#define MIXFACEEQUALIZERWIDGET_H

#include <QtWidgets>
#include "MixFaceFonts.h"
#include "MixFaceEnumTypes.h"

class EqualizerWidget : public QWidget {
    Q_OBJECT
public:
    explicit EqualizerWidget(int idx, float dpiRatio, MixFaceFonts *m_fonts_ = nullptr);

signals:

private:
    MixFaceFonts *m_fonts;
};

#endif // MIXFACEEQUALIZERWIDGET_H
