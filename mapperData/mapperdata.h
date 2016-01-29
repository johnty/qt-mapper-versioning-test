#ifndef MAPPERDATA_H
#define MAPPERDATA_H

#include <QtSql>
#include <QSqlDatabase>
#include <QDebug>
#include "mapperUI/qmapperdbmodel.h"

//the central class that holds different data models that are
// used by the mapper UI. Much consolidation is needed as there's a lot of redundant
// information from different systems (e.g. previous JSON system only stores map info, while
// the new UI with versioning stores signal information as well (including ones not mapped),
// and different systems support different levels of detail. currently, we're just
// syncing the relevant bare minimum data from each model so that we can get the first
// demonstration of the system up and running. not handled (as of this comment) is the live network db
// model.

struct MAPPER_SRC_DST
{
    QString id;
    QString dev;
    QString sig;
};

struct MAPPER_SIGNAL
{
    QString name;
    QString dev;
    QString direction;
};

struct MAPPER_CONNECTION
{
    //Fill with more stuff?
};


class MapperData
{
public:
    MapperData();
    ~MapperData();

    void toDB();
    void fromDB();

    void FromUIDbModel(const QMapperDbModel* model); //sync external model
    void ToUIDbModel(QMapperDbModel* modelToSync); //update internal model

    const QMapperDbModel* getMapperUIDbModel() {return &myUIDbModel;}

protected:

    QSqlDatabase myDB;
    QMapperDbModel myUIDbModel;

    QList<MAPPER_SRC_DST> mySources; //map sources
    QList<MAPPER_SRC_DST> myDestinations; //map destinations
    QList<QString> myConnectionExprs; //map expression strings
    QList<MAPPER_SIGNAL> mySignals; //flat view of all signals


    //TODO: integrate with libmapper API headers? that seems to make most sense...
    // current examples:
    //   SOURCE:
    //      id (s0, s1...)
    //      device
    //      signal (something/something/x - addressed based namespace)
    //
    //   DESTINATIONS:
    // id, dev, sig (same as above)
    //
    //   MAPPINGS: ("connections", "links", or... check new API)
    //   ..etc

private:
    void createTables();
    void dropAllTables();

};

#endif // MAPPERDATA_H
