#ifndef MIXFACEFONTS_H
#define MIXFACEFONTS_H
#include <QFont>

class MixFaceFonts : public QObject
{
public:
    MixFaceFonts(){
        nativeFont.setFamily(QString::fromUtf8("Arial"));
        nativeFont.setPointSize(14);
        nativeFont.setStyleStrategy(QFont::PreferAntialias);

        boldFont.setFamily(QString::fromUtf8("Arial"));
        boldFont.setPointSize(14);
        boldFont.setBold(true);
        boldFont.setStyleStrategy(QFont::PreferAntialias);

        italicFont.setFamily(QString::fromUtf8("Arial"));
        italicFont.setPointSize(14);
        italicFont.setItalic(true);
        italicFont.setStyleStrategy(QFont::PreferAntialias);

        nativeFont12.setFamily(QString::fromUtf8("Arial"));
        nativeFont12.setPointSize(12);
        nativeFont12.setStyleStrategy(QFont::PreferAntialias);

        boldFont12.setFamily(QString::fromUtf8("Arial"));
        boldFont12.setPointSize(12);
        boldFont12.setBold(true);
        boldFont12.setStyleStrategy(QFont::PreferAntialias);

        italicFont12.setFamily(QString::fromUtf8("Arial"));
        italicFont12.setPointSize(12);
        italicFont12.setItalic(true);
        italicFont12.setStyleStrategy(QFont::PreferAntialias);

        italicFont8.setFamily(QString::fromUtf8("Arial"));
        italicFont8.setPointSize(8);
        italicFont8.setItalic(true);
        italicFont8.setStyleStrategy(QFont::PreferAntialias);

        nativeFont8.setFamily(QString::fromUtf8("Arial"));
        nativeFont8.setPointSize(8);
        nativeFont8.setStyleStrategy(QFont::PreferAntialias);

        boldFont8.setFamily(QString::fromUtf8("Arial"));
        boldFont8.setPointSize(8);
        boldFont8.setBold(true);
        boldFont8.setStyleStrategy(QFont::PreferAntialias);
    }

    QFont nativeFont;
    QFont boldFont;
    QFont italicFont;
    QFont nativeFont12;
    QFont boldFont12;
    QFont italicFont12;
    QFont nativeFont8;
    QFont boldFont8;
    QFont italicFont8;
};

#endif // MIXFACEFONTS_H
