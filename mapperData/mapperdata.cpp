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
    //****UI DB
    myUIDbModel.clearAll();
    for (int i=0; i<mySignals.size(); i++)
    {
        MAPPER_SIGNAL sig = mySignals.at(i);
        bool isInput = true;
        if (sig.direction == "output")
            isInput = false;
        myUIDbModel.addSignal(sig.dev, sig.name, isInput);
    }
    for (int i=0; i<myUIDbModel.getNumSigs(); i++)
        qDebug() << "sig "<<i<<":"<< myUIDbModel.getSigName(i);

    //associate each source-destination pair in originall format as a map!
    // one assumption is the two lists are same in length, which based on my
    // understanding of the format, it should be...
    if (mySourceList.size() == myDestinationList.size())
    {
        for (int i=0; i<mySourceList.size(); i++)
        {
            QString src_dev = mySourceList.at(i).dev;
            QString src_sig = mySourceList.at(i).sig;
            QString dst_dev = myDestinationList.at(i).dev;
            QString dst_sig = myDestinationList.at(i).sig;
            myUIDbModel.updateMap(src_dev, src_sig, dst_dev, dst_sig);
        }
    }

    return;
    //****SQL DB
    //drop existing tables, and make new ones
    dropAllTables();
    createTables();
    if (myDB.isOpen())
    {
        QSqlQuery query(myDB);
        //sources:
        query.prepare("INSERT INTO sources (id, dev, sig)"
                      "VALUES (:id, :dev, :sig)");
        for (int i=0; i<mySourceList.size()-1; i++)
        {
            query.bindValue(":id", mySourceList.at(i).id);
            query.bindValue(":dev", mySourceList.at(i).dev);
            query.bindValue(":sig", mySourceList.at(i).sig);
            if (query.exec())
                qDebug() << "added values into db";
        }
        // similarly for other objects, but wait until we know what exactlly
        // we want to store in this central db before actually doing it,
        // since we're not directly accessing it and calling the model objects anyway at this point.


        query.clear();
    }
    else
    {
        qWarning("unable to create in-memory db for storing mapping data!");
    }

    //UI DB

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

void MapperData::FromUIDbModel(const QMapperDbModel *model)
{
    myUIDbModel.syncWith(*model);
}

void MapperData::ToUIDbModel(QMapperDbModel *modelToSync)
{
    modelToSync->syncWith(myUIDbModel);
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

