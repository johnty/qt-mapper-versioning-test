#ifndef MAPPERJSONCONFIG_H
#define MAPPERJSONCONFIG_H

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QIODevice>
#include <QDebug>
#include "mapperdata.h"

class MapperJsonConfig : public MapperData
{
public:
    MapperJsonConfig();
    MapperJsonConfig(const QString filePath, QIODevice::OpenModeFlag mode = QIODevice::ReadWrite);

    const QList<MAPPER_SRC_DST>* getSrcs() { return &mySources; }
    const QList<MAPPER_SRC_DST>* getDests() { return &myDestinations; }
    const QList<QString>* getConns() { return &myConnectionExprs; }

private:
    bool ParseFile(const QJsonObject& json_doc);
    //TODO: data to hold other stuff...
};

#endif // MAPPERJSONCONFIG_H
