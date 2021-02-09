#include "DebugLibrary.h"

DebugLibrary::DebugLibrary(QApplication *qapp)
{
    if (qapp != nullptr) {
        if (qapp->arguments().contains("-debug",Qt::CaseInsensitive)) {
            QRegExp rxdebug("-debug");
            rxdebug.setCaseSensitivity(Qt::CaseInsensitive);
            int idxdebug = qapp->arguments().indexOf(rxdebug)+1;
            int debugLevelArg = QString(qapp->arguments().value(idxdebug)).toInt();
            if (debugLevelArg > -1 && debugLevelArg < 6)
                setDebugLevel(debugLevelArg);
        }
    }
    sendMessage("DebugLibrary::DebugLibrary Debuger loaded...",1);
}

void DebugLibrary::sendMessage(QString message_, int debugLevel_){
    QString message = getCurrentTime() + " " + message_;
    if (debugLevel_<=debugLevel) {
        qDebug()<<message;
    }
}

void DebugLibrary::setDebugLevel(int debugLevel_){
    debugLevel = debugLevel_;
    sendMessage("DebugLibrary::setDebugLevel Debuger level set to " + QString::number(debugLevel),0);
}

QString DebugLibrary::getCurrentTime(){
    QString currentTime;

    int hour = time.currentTime().hour();
    if (hour < 10) currentTime = "0" + QString::number(hour) + ":";
    else currentTime = QString::number(hour) + ":";

    int minute = time.currentTime().minute();
    if (minute < 10) currentTime = currentTime + "0" + QString::number(minute) + ":";
    else currentTime = currentTime + QString::number(minute) + ":";

    int sec = time.currentTime().second();
    if (sec < 10) currentTime = currentTime + "0" + QString::number(sec) + ".";
    else currentTime = currentTime + QString::number(sec) + ".";

    int msec = time.currentTime().msec();
    if (msec < 10) currentTime = currentTime + "00" + QString::number(msec);
    else if (msec < 100) currentTime = currentTime + "0" + QString::number(msec);
    else currentTime = currentTime + QString::number(msec);

    return currentTime;
}
