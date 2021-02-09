#include "MixFaceKeeper.h"
MixFaceKeeper::MixFaceKeeper(QObject *parent) : QObject(parent){}

void MixFaceKeeper::open(){
    QFile file("data000.xml");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QXmlStreamReader xmlReader;
    xmlReader.setDevice(&file);
    xmlReader.readNext();
    while(!xmlReader.atEnd())
        {
            if (xmlReader.name() == "serialname")
                foreach(const QXmlStreamAttribute &attr, xmlReader.attributes()) {
                    if (attr.name().toString() == "value")
                    {
                        attr.value().toString().toInt();
                    }
                }
            xmlReader.readNext();
        }
    emitopened();
}

void MixFaceKeeper::save(){
    QFile config("data000.xml");
    config.open(QIODevice::WriteOnly);
    QXmlStreamWriter writer(&config);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
        writer.writeStartElement("settings");
            writer.writeStartElement("serialname");
                writer.writeAttribute("value", QString::number(123));
            writer.writeEndElement();
        writer.writeEndElement();
    writer.writeEndDocument();
    config.close();
}

void MixFaceKeeper::emitopened(){
    emit opened();
}

void MixFaceKeeper::emitsaved(){
    emit saved();
}
