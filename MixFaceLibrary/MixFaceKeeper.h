#ifndef MIXFACEKEEPER_H
#define MIXFACEKEEPER_H

#include <QObject>
#include "MixFaceLibrary.h"

class MixFaceKeeper : public QObject
{
    Q_OBJECT
public:
    explicit MixFaceKeeper();

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
