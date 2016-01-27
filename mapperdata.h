#ifndef MAPPERDATA_H
#define MAPPERDATA_H

#include <QtSql>
#include <QSqlDatabase>
#include <QDebug>

struct MAPPER_SRC_DST
{
    QString id;
    QString dev;
    QString sig;
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

protected:

    QSqlDatabase myDB;

    QList<MAPPER_SRC_DST> mySources;
    QList<MAPPER_SRC_DST> myDestinations;
    QList<QString> myConnectionExprs;


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
