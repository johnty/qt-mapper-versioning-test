#ifndef MAPPERJSONCONFIG_H
#define MAPPERJSONCONFIG_H

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QIODevice>
#include <QDebug>
#include "mapperdata.h"
#include "mapperUI/qmapperdbmodel.h"

class MapperJsonConfig : public MapperData
{
public:
    MapperJsonConfig();
    MapperJsonConfig(const QString filePath, QIODevice::OpenModeFlag mode = QIODevice::ReadWrite);
    bool SaveConfigToJSONFile(QString filePath);

    //NOTE: existing format contains just list of "maps"
    const QList<MAPPER_SRC_DST>* getSrcs() { return &mySources; }
    const QList<MAPPER_SRC_DST>* getDests() { return &myDestinations; }
    const QList<QString>* getConns() { return &myConnectionExprs; }

private:
    QJsonObject mapperConfigObject;
    bool ParseJsonObject(const QJsonObject& json_doc);
    //TODO: data to hold other stuff...
};

#endif // MAPPERJSONCONFIG_H
