#include "mapperdbthread.h"

mapperdbthread::mapperdbthread() :db(MAPPER_SUBSCRIBE_ALL)
{
    db.add_device_callback(devActionHandler, nullptr);
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
    while (myMaps.size())
    {
        mapper::Map* map = myMaps.at(myMaps.size()-1);
        map->unmap();
        delete map;
        myMaps.pop_back();
    }
}


const std::vector<QString> mapperdbthread::getDeviceList()
{
    std::vector<QString> list;
    myLock.lock();
    for (auto const &device : db.devices())
    {
        QString devname((const char*)device.property("name"));
        list.push_back(devname);        
    }
    myLock.unlock();
    return list;
}

const std::vector<QString> mapperdbthread::getSigList(QString devname, mapper_direction dir)
{
    std::vector<QString> list;
    myLock.lock();

    mapper::Device dev = db.device_by_name(devname.toStdString());

    mapper::Signal::Query qry = dev.signals(dir);

    for (; qry != qry.end(); qry++)
    {
        mapper::Signal sig = *qry;
        QString sig_name = sig.name().c_str();
        list.push_back(sig_name);
    }
    myLock.unlock();
    return list;
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
    myMaps.push_back(map);

}


void mapperdbthread::devEvent(mapper_device dev, mapper_record_action action)
{

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

    qDebug() << "devAction from " << mapper_device_name(dev) << " Action = "
                 << actionStr;

}

void mapperdbthread::syncRenderModel(QMapperDbModel *modelToSync)
{
    modelToSync->clearAll();

    //For now, we're really just interested in the flat signals view, so thats what we do...
    // for whatever's sake, we do a complete query of the db so really this is not a sync as a complete
    // poll, (clean this up once we have callbacks, and world peace...)
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
