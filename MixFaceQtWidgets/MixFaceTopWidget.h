#ifndef MIXFACETOPWIDGET_H
#define MIXFACETOPWIDGET_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QFrame>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QAbstractScrollArea>
#include <QMainWindow>
#include "MixFaceFonts.h"
#include "DebugLibrary.h"

class MixFaceTopWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MixFaceTopWidget(float dpiRatio,DebugLibrary *debug_, MixFaceFonts *m_fonts_, QMainWindow *mainWindow_);

    QLineEdit *lineIp;
    QLabel *console;
    QLabel *consoleName;
    QPushButton *syncAction;
    QPushButton *connectAction;

signals:
    void connect();
    void sync();

private:
    QMainWindow *mainWindow;
    MixFaceFonts *m_fonts;
    DebugLibrary *debug;
    float dpiRatio = 1;

    QPushButton *settings;
    QFrame *settingsPopup;

    void initTopAreaBar();
    void initSettingsPopup();
    void settingsButtonClicked();

};

#endif // MIXFACETOPWIDGET_H
