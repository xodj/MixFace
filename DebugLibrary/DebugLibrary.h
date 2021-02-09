#ifndef DEBUGLIBRARY_H
#define DEBUGLIBRARY_H

#include <QApplication>
#include <QString>
#include <QDebug>
#include <QFile>
#include <QTime>

class DebugLibrary : public QObject {
    Q_OBJECT
public:
    DebugLibrary(QApplication *qapp = nullptr);

    void sendMessage(QString message_, int debugLevel_);
    void setDebugLevel(int debugLevel_);

private:
    int debugLevel = 0;
    QString getCurrentTime();
    QTime time;
};

#endif // DEBUGLIBRARY_H
