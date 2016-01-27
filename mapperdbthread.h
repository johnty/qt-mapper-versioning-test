#ifndef MAPPERDBTHREAD_H
#define MAPPERDBTHREAD_H

#include <QThread>
#include <QMutex>
#include <QDebug>
#include <mapper/mapper_cpp.h>
#include "mapperUI/qmapperdbmodel.h"


class mapperdbthread : public QThread
{

    Q_OBJECT

public:
    mapperdbthread();
    ~mapperdbthread();

    // TODO: this mechanism needs some reworking since
    // we can't emit signals from static function...
    // function/bind might be a solution? [update: probably not] for now
    // we have a wonderful hack where
    // 1.) we assume there is only one instance of this class
    // 2.) we hold a static pointer of that instance
    // 3.) we call the instance's action handler
    //
    //
    // based on whats been tried so far, what makes the most sense
    // is to rework this class at some point, and have a single point
    // of entry to whatever object holding instance(s) that need to get
    // updates from here, and *don't mix QObject stuff* into this db interface class
    //

    //NOTE2: for now, we could also get away with just reacting to
    // signal add/removes, as we're using a flat view of sig->sig connections

    static void devActionHandler(mapper_device dev,
                                 mapper_record_action action,
                                 const void *user);

    void devActionFn(mapper_device dev,
                     mapper_record_action action);

    static void sigActionHandler(mapper_signal sig,
                                    mapper_record_action action,
                                    const void *user);

    void sigActionFn(mapper_signal sig,
                     mapper_record_action action);

    void run();
    void stopThread();
    void syncRenderModel(QMapperDbModel* modelToSync);

    bool keepGoing;

    QMutex myLock;

    //because of some bad decisions made earlier, we
    // pass copies of the data... should be fixed!
    const std::vector<QString> getDeviceList();
    //const std::vector<QString> getSigList(QString devname, mapper_direction DIR = MAPPER_DIR_ANY);
    //const std::vector<QString> getSigList();
    const std::vector<QString> getSigList(QString devname);

    void refreshDbNetworkModel();

    void makeMap(QString sdev, QString ddev, QString ssig, QString dsig);

    //TODO: rethink this kind of access, as well as similarly poor life choices
    mapper::Db* getDB() { return &db;}

Q_SIGNALS:
    void devUpdatedSig();
    void mapUpdatedSig();


public Q_SLOTS:


private:

    mapper::Db db;

    //TODO: should we use std containers or QT ones?
    // ALSO, by now we should realize we have such a list
    // in many many places - should probably create a container class
    // for these things...

    std::vector<QString> devList;
    std::vector<QString> sigList;

    std::vector<mapper::Map*> myMaps;
    QMapperDbModel myDbNetworkModel;
    QMapperDbModel * myDbRenderModel;

//    fn pointers to callback methods
    //note: in c++11 we can init to NULL here, pre-c++11 we can't!
    void (mapperdbthread::*ptrDevAction)(mapper_device dev, mapper_record_action action) = NULL;

    void addDevSigs(QString devname);
    void signalToDB(QString devname, const mapper::Signal sig, bool isAdd);

};

#endif // MAPPERDBTHREAD_H
