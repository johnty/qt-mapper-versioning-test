#include "mapperdata.h"

MapperData::MapperData()
{
    myDB = QSqlDatabase::addDatabase("QSQLITE");
    myDB.setDatabaseName(":memory:");
    if (myDB.open())
    {
        qDebug() << "in memory db object successfully opened!";
    }
}

MapperData::~MapperData()
{
    if (myDB.isOpen())
    {
        myDB.close();
        qDebug() <<" in memory db closed";
    }
}

void MapperData::toDB()
{
    //drop existing tables, and make new ones
    dropAllTables();
    createTables();
    if (myDB.isOpen())
    {
        QSqlQuery query(myDB);
        //sources:
        query.prepare("INSERT INTO sources (id, dev, sig)"
                      "VALUES (:id, :dev, :sig)");
        for (int i=0; i<mySources.size()-1; i++)
        {
            query.bindValue(":id", mySources.at(i).id);
            query.bindValue(":dev", mySources.at(i).dev);
            query.bindValue(":sig", mySources.at(i).sig);
            if (query.exec())
                qDebug() << "added values into db";
        }


        query.clear();
    }
    else
    {
        qWarning("unable to create in-memory db for storing mapping data!");
    }
}

void MapperData::fromDB()
{
    if (myDB.isOpen())
    {
        QSqlQuery query(myDB);
        query.prepare("SELECT id, dev, sig FROM sources");
        query.exec();
        while (query.next())
        {
            QString id = query.value(0).toString();
            QString dev = query.value(1).toString();
            QString sig = query.value(2).toString();
            qDebug() <<"loaded from DB test: " <<id<<" "<<dev<<" " <<sig;
        }

    }
    else
    {
        qWarning("unable to access in-memory db for storing mapping data!");
    }
}

void MapperData::createTables()
{
    QSqlQuery query(myDB);
    query.prepare("CREATE TABLE sources(id, dev, sig)");
    if (query.exec())
        qDebug() << "   srcs table created";
    query.prepare("CREATE TABLE destinations(id, dev, sig)");
    if (query.exec())
        qDebug() << "   dest table created";
    query.prepare("CREATE TABLE connections(mute, mode, range, expr, clipmin, clipmax)");
    if (query.exec())
        qDebug() << "   conns table created";
}

void MapperData::dropAllTables()
{
        QSqlQuery query(myDB);
        query.exec("DROP TABLE IF EXISTS sources");
        query.exec("DROP TABLE IF EXISTS destinations");
        query.exec("DROP TABLE IF EXISTS connections");
        query.clear();
}

