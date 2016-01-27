#include "mapperdbthread.h"

mapperdbthread* FIX_ME;

mapperdbthread::mapperdbthread() :db(MAPPER_SUBSCRIBE_ALL)
{
    db.add_device_callback(devActionHandler, nullptr);
    db.add_signal_callback(sigActionHandler, nullptr);
    db.add_map_callback(mapActionHandler, nullptr);

    //set up callback fn ptrs
    //note: so apparently its a bad idea
    // to try and bind a signal callback to
    // an instance of a class
    //  (https://isocpp.org/wiki/faq/pointers-to-members#memfnptr-vs-fnptr)
    // so once we have some time we should re-look how
    // the db model interacts with QT objects (main limitation being
    // we cannot emit QT signals directly from static functions).
    //ptrDevAction = &mapperdbthread::devActionFn;
    FIX_ME = this;
}

void mapperdbthread::run()
{
    keepGoing = true;
    while (keepGoing)
    {
        //res = "Db scan result:\n";
        myLock.lock();
        db.update(50);
        myLock.unlock();
    }
    qDebug()<<"db thread ending...";
}

void mapperdbthread::stopThread()
{
    myLock.lock();
    keepGoing = false;
    myLock.unlock();
}

mapperdbthread::~mapperdbthread()
{
    //we probably don't want to unmap stuff on quit...
    //just delete our local structures...
    while (myMaps.size())
    {
        mapper::Map* map = myMaps.at(myMaps.size()-1);
        //map->unmap();
        delete map;
        myMaps.pop_back();
    }
}


const std::vector<QString> mapperdbthread::getDeviceList()
{
    return myDbNetworkModel.getDevs();
    //return devList;
}

const std::vector<QString> mapperdbthread::getSigList(QString devname)
{
    return myDbNetworkModel.getDevSigs(devname);
}

//const std::vector<QString> mapperdbthread::getSigList(QString devname, mapper_direction dir)
//{
//    return sigList;
//}

//const std::vector<QString> mapperdbthread::getSigList()
//{

//}


void mapperdbthread::refreshDbNetworkModel()
{
    myDbNetworkModel.clearAll();
    for (auto const &device : db.devices())
    {   //NOTE2SELF: the way we're updating the Db**Model object
        // here probably motivates an inheritance of the db model
        // instead of composition: we make direct modifications
        // of the model in many places in this class, which suggests a tighter
        // coupling between that object and this class... think about it.
        QString devname((const char*)device.property("name"));
        myDbNetworkModel.addDevice(devname);
        addDevSigs(devname);
    }
}

void mapperdbthread::addDevSigs(QString devname)
{
    mapper::Device dev = db.device_by_name(devname.toStdString());

    mapper::Signal::Query qry = dev.signals(MAPPER_DIR_ANY);
    for (; qry != qry.end(); qry++)
    {
        mapper::Signal csig = *qry;
        signalToDB(devname, csig, true);
   }
}

void mapperdbthread::sigActionHandler(mapper_signal sig,
                                         mapper_record_action action,
                                         const void *user)
{
    QString actionStr;
    switch (action) {
    case MAPPER_ADDED:
        actionStr = "Added";
        break;
    case MAPPER_MODIFIED:
        actionStr = "modified";
        break;
    case MAPPER_REMOVED:
        actionStr = "removed";
        break;
    case MAPPER_EXPIRED:
        actionStr = "unresponsive";
        //mapper_db_flush(db, 10, 0);
        break;
    }
    FIX_ME->sigActionFn(sig, action);
}

void mapperdbthread::mapActionHandler(mapper_map map, mapper_record_action action, const void *user)
{

    QString actionStr;
    switch (action) {
    case MAPPER_ADDED:
        actionStr = "Added";
        break;
    case MAPPER_MODIFIED:
        actionStr = "modified";
        break;
    case MAPPER_REMOVED:
        actionStr = "removed";
        break;
    case MAPPER_EXPIRED:
        actionStr = "unresponsive";
        //mapper_db_flush(db, 10, 0);
        break;
    }
    qDebug() <<"mapperAction callled:" <<actionStr;
    FIX_ME->mapActionFn(map, action);
}

void mapperdbthread::sigActionFn(mapper_signal sig, mapper_record_action action)
{
    //this call might suggest an addition to the Signal wrapper class?
    QString devname(mapper_device_name( mapper_signal_device(sig)));
    //we assume just two cases, add or remove
    mapper::Signal csignal(sig);
    switch (action) {
    case MAPPER_ADDED:
        qDebug()<<"sigAction: adding sig";
        signalToDB(devname, csignal, true);
        Q_EMIT devUpdatedSig();
        break;
     case MAPPER_REMOVED:
        qDebug()<<"sigAction: removing sig";
        signalToDB(devname, csignal, false);
        Q_EMIT devUpdatedSig();
        break;
    }
}

void mapperdbthread::mapActionFn(mapper_map map, mapper_record_action action)
{

}

void mapperdbthread::tryMap(int src, int dst)
{
    QString src_sig = myDbNetworkModel.getSigItem(src)->text();
    QString src_dev = myDbNetworkModel.getSigItem(src)->child(0)->text();
    QString dst_sig = myDbNetworkModel.getSigItem(dst)->text();
    QString dst_dev = myDbNetworkModel.getSigItem(dst)->child(0)->text();
    qDebug()<<"mapperDB trying to map from" <<src_dev<<":"<<src_sig<<
              "to <<"<< dst_dev<<":"<<dst_sig;
    makeMap(src_dev, dst_dev, src_sig, dst_sig);
}


void mapperdbthread::devActionFn(mapper_device dev, mapper_record_action action)
{
    //qDebug() <<"instance devAction";
    QString devname(mapper_device_name(dev));
    switch (action) {
    case MAPPER_ADDED:
        qDebug()<<"devAction: adding dev";
        myDbNetworkModel.addDevice(devname);
        //signalToDB(devname, csignal, true);
        break;
     case MAPPER_REMOVED:
        qDebug()<<"devAction: removing dev";
        //signalToDB(devname, csignal, false);
        myDbNetworkModel.removeDevice(devname);
        break;
    }
        //we let the UI respond to sig changes only, and don't "care" about the dev
        // updates for now except for updating our internal db above...
    //Q_EMIT devUpdatedSig();
}

void mapperdbthread::devActionHandler(mapper_device dev,
                            mapper_record_action action,
                            const void *user)
{
    QString actionStr;
    switch (action) {
    case MAPPER_ADDED:
        actionStr = "Added";
        break;
    case MAPPER_MODIFIED:
        actionStr = "modified";
        break;
    case MAPPER_REMOVED:
        actionStr = "removed";
        break;
    case MAPPER_EXPIRED:
        actionStr = "unresponsive";
        //mapper_db_flush(db, 10, 0);
        break;
    }

    //(*ptrDevAction)(dev, action);
    FIX_ME->devActionFn(dev, action);

    //qDebug() << "devAction from " << mapper_device_name(dev) << " Action = "<< actionStr;

}

void mapperdbthread::syncRenderModel(QMapperDbModel *modelToSync)
{
    qDebug()<<"syncing QMapperDbModel";
    //For now, we're really just interested in the flat signals view, so thats what we do...
    // for whatever's sake, we do a complete query of the db so really this is not a sync as a complete
    // poll, (clean this up once we have callbacks, and world peace...)
    // ---- update: we have something like callbacks in behavior now, but are still far from world peace.

    //new way: just sync the model objects
    modelToSync->syncWith(myDbNetworkModel);


    return;

    //old method: complete query on sync
    myLock.lock();
    for (auto const &device : db.devices())
    {
        QString dev_name((const char*)device.property("name"));
        modelToSync->addDevice(dev_name);
        mapper::Signal::Query qry = device.signals(MAPPER_DIR_INCOMING);
        for (; qry != qry.end(); qry++)
        {
            mapper::Signal sig = *qry;
            QString sig_name = sig.name().c_str();
            modelToSync->addSignal(dev_name, sig_name, true );
        }

        mapper::Signal::Query qry2 = device.signals(MAPPER_DIR_OUTGOING);
        for (; qry2 != qry2.end(); qry2++)
        {
            mapper::Signal sig = *qry2;
            QString sig_name = sig.name().c_str();
            modelToSync->addSignal(dev_name, sig_name, false );
        }

    }
    myLock.unlock();
}

void mapperdbthread::makeMap(QString sdev, QString ddev, QString ssig, QString dsig)
{
    mapper::Device srcdev = db.device_by_name(sdev.toStdString());
    mapper::Signal *src_sig = NULL;

    mapper::Signal::Query qry1 = srcdev.signals(MAPPER_DIR_OUTGOING);
    for (; qry1 != qry1.end(); qry1++)
    {
        mapper::Signal sig = *qry1;
        QString sig_name = sig.name().c_str();
        if (sig_name == ssig)
        {
            src_sig = &sig;
            qDebug()<<"found src signal " <<sig_name<< "from device " << sdev;
        }
    }

    mapper::Device dstdev = db.device_by_name(ddev.toStdString());
    mapper::Signal *dst_sig = NULL;

    mapper::Signal::Query qry2 = dstdev.signals(MAPPER_DIR_INCOMING);
    for (; qry2 != qry2.end(); qry2++)
    {
        mapper::Signal sig = *qry2;
        QString sig_name = sig.name().c_str();
        if (sig_name == dsig)
        {
            dst_sig = &sig;
            qDebug()<<"found dst signal " <<sig_name<< "from device " << sdev;
        }
    }
    mapper::Map* map = new mapper::Map(srcdev.signal(ssig.toStdString()),
                                       dstdev.signal(dsig.toStdString()));

    map->push();

    myMaps.push_back(map);

}

void mapperdbthread::signalToDB(QString devname, const mapper::Signal signal, bool isAdd)
{
    QString sig_name(signal.name().c_str());
    mapper_direction dir = signal.property("direction");
    bool is_input = true;
    if (dir == MAPPER_DIR_OUTGOING)
        is_input = false;
    if (isAdd)
        myDbNetworkModel.addSignal(devname, sig_name, is_input);
    else
        myDbNetworkModel.removeSignal(devname, sig_name);
}
