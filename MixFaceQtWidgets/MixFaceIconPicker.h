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
#include "DebugLibrary.h"

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
    void emitColorChanged(int idc);
    void emitNameChanged();

    float dpiRatio = 1;
    int idx = 0;

    QMainWindow *mainWindow;
    DebugLibrary *debug;
    MixFaceFonts *m_fonts;

    QLabel *nativeName;
    QLineEdit *channelName;

    QPushButton *iconButton;

    QListWidget *iconsList;
    QListWidget *colorsList;

};
#endif // MIXFACEICONPICKER_H
