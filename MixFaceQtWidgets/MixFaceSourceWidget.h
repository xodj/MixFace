#ifndef MIXFACESOURCEWIDGET_H
#define MIXFACESOURCEWIDGET_H

#include <QtWidgets>
#include "MixFaceFonts.h"
#include "MixFaceEnumTypes.h"

class SourceWidget : public QWidget {
    Q_OBJECT
public:
    explicit SourceWidget(int idx, float dpiRatio, MixFaceFonts *m_fonts_ = nullptr);

signals:

private:
    MixFaceFonts *m_fonts;
};

#endif
