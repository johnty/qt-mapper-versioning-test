#ifndef MAPPERDBTHREAD_H
#define MAPPERDBTHREAD_H

#include <QThread>
#include <QMutex>
#include <QDebug>
#include <mapper/mapper_cpp.h>


class mapperdbthread : public QThread
{
public:
    mapperdbthread();
    ~mapperdbthread();

    void run();
    void stopThread();

    bool keepGoing;

    QMutex myLock;

    const std::vector<QString> getDeviceList();
    const std::vector<QString> getSigList(QString devname, mapper_direction DIR = MAPPER_DIR_ANY);

    void makeMap(QString sdev, QString ddev, QString ssig, QString dsig);

    //TODO: rethink this kind of access, as well as similarly poor life choices
    mapper::Db* getDB() { return &db;}

private:

    mapper::Db db;

    //TODO: should we use std containers or QT ones?
    std::vector<QString> devlist;
    std::vector<QString> outSigsList;

   std::vector<mapper::Map*> myMaps;

};

#endif // MAPPERDBTHREAD_H
