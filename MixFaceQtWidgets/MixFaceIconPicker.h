#ifndef MIXFACEICONPICKER_H
#define MIXFACEICONPICKER_H

#include <QObject>
#include <QWidget>
#include <QMainWindow>
#include <QFrame>
#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QScroller>
#include <QListWidget>
#include "MixFaceFonts.h"
#include "DebugLibrary.hpp"

class MixFaceIconPicker : public QFrame
{
    Q_OBJECT
public:
    explicit MixFaceIconPicker(float dpiRatio_, DebugLibrary *debug_, MixFaceFonts *m_fonts_, QMainWindow *mainWindow_);
    void showIconPopup(int idx_, int logoId, int colorId, QString nativeName_, QString name);

signals:
    void logoChanged(int idx, int value);
    void colorChanged(int idx, int value);
    void nameChanged(int idx, QString value);

private:
    void initIconPopup();

    void emitLogoChanged(int idl);
    void emitColorChanged();
    void emitNameChanged();
    void setColorPreview(int value);

    float dpiRatio = 1;
    int idx = 0;
    int colorNumber = 0;
    int iconNumber = 0;
    QString BWIcon = "W";

    QMainWindow *mainWindow;
    DebugLibrary *debug;
    MixFaceFonts *m_fonts;

    QLabel *nativeName;
    QLineEdit *channelName;

    QPushButton *iconButton;

    QListWidget *iconsList;
    QList<QPushButton*> *colorsList;

};
#endif // MIXFACEICONPICKER_H
