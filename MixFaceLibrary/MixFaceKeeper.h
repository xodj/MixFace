#ifndef MIXFACEKEEPER_H
#define MIXFACEKEEPER_H

#include <QObject>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QFile>
#include "MixFaceLibrary.h"

class MixFaceKeeper : public QObject
{
    Q_OBJECT
public:
    explicit MixFaceKeeper(QObject *parent = nullptr);

public slots:
    void open();
    void save();

signals:
    void opened();
    void saved();

private:
    void emitopened();
    void emitsaved();
};

#endif // MIXFACEKEEPER_H
