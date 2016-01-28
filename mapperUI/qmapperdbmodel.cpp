#include "qmapperdbmodel.h"

QMapperDbModel::QMapperDbModel()
{

}

void QMapperDbModel::addDevice(QString devName)
{
    mapperDevNames.append(devName);
}

void QMapperDbModel::addSignal(QString devName, QString sigName, bool isInput)
{
    qDebug()<<"DbModel add sig" <<devName<<":"<<sigName<<":"<<isInput;
    QStandardItem* newSig = new QStandardItem(sigName);

    newSig->insertRow(0, new QStandardItem(devName));
    if (isInput)
        newSig->insertRow(1, new QStandardItem("input"));
    else
        newSig->insertRow(1, new QStandardItem("output"));

    mapperSignals.append(newSig);

}

void QMapperDbModel::removeDevice(QString devName)
{
    for (int i=0; i<mapperDevNames.size(); ++i)
    {
        if (mapperDevNames.at(i) == devName)
        {
            mapperDevNames.removeAt(i);
            break;
        }
    }

}

void QMapperDbModel::removeSignal(QString devName, QString sigName)
{
    //TODO: fill this; we probably don't use it except when clearing whole list,
    // but in future when
    for (int i=0; i<mapperSignals.size(); ++i)
    {
        if (sigName == mapperSignals.at(i)->text())
        {
            if (mapperSignals.at(i)->child(0)->text() == devName)
            {
                qDebug() <<"DbModel remove sig " <<sigName<<":" <<devName;
                mapperSignals.removeAt(i);
                break;
            }
        }
    }
}

void QMapperDbModel::updateMap(QString srcDev, QString srcSig, QString dstDev, QString dstSig, bool isAdd)
{
    int src = -1;
    int dst = -1;
    for (int i=0; i<mapperSignals.size(); ++i)
    {
        QStandardItem* signal = mapperSignals.at(i);
        if ( (srcSig == signal->text()) && (srcDev == signal->child(0)->text()) )
        {
            qDebug() << "!!! Found " <<srcDev<<":"<<srcSig<<" at idx " <<i;
            src = i;

        }
        if ( (dstSig == signal->text()) && (dstDev == signal->child(0)->text()) )
        {
            qDebug() << "!!! FOUND " <<dstDev<<":"<<dstSig<<" at idx " <<i;
            dst = i;
        }
    }
    if ((src != -1) && (dst != -1))
        updateMap(src, dst, isAdd);
}

void QMapperDbModel::updateMap(int srcIdx, int dstIdx, bool isAdd)
{
    //cases:
    //  1.)doesn't exist, want to add -> add
    //  2.)doesn't exist, want to remove-> nothing (invalid)
    //  3.)exists, want to add -> nothing (redundant add)
    //  4.)exists, want to remove -> remove
    int mapIndex = mapExists(srcIdx, dstIdx);
    if ((mapIndex == -1) && isAdd)
    {   //if its not there, and we want to add...
        qDebug()<<"Adding Map to model...";
        mapperMapsSrc.push_back(srcIdx);
        mapperMapsDst.push_back(dstIdx);
    }
    else if ( (mapIndex != -1) && !isAdd )
    {
        removeMap(mapIndex);
    }
    Q_EMIT dBUpdateSig();
}

void QMapperDbModel::removeMap(int idx)
{
    mapperMapsSrc.removeAt(idx);
    mapperMapsDst.removeAt(idx);
}

int QMapperDbModel::mapExists(int srcIdx, int dstIdx)
{
    //TODO: should we be checking UI code, or simply apply the map to model and then
    // let model deal with it?
    int found_src = -1;
    for (int i=0; i<mapperMapsSrc.size(); ++i)
    {
        if (mapperMapsSrc.at(i) == srcIdx)
        {
            found_src = i;
            if (mapperMapsDst.at(i) == dstIdx)
                return i;
        }
    }
    //not found
    return -1;
}

int QMapperDbModel::mapExists(QString srcDev, QString srcSig, QString dstDev, QString dstSig)
{

}

const std::vector<QString> QMapperDbModel::getDevs()
{
    return mapperDevNames.toStdVector();
}

const std::vector<QString> QMapperDbModel::getDevSigs(QString devname)
{
    std::vector<QString> list;
    for (int i=0; i<mapperSignals.size(); ++i)
    {
        // this shows why tree structure makes more sense
        // (and elsewhere it might show otherwise...)
        if (devname == mapperSignals.at(i)->child(0)->text())
        {
            QString signame = mapperSignals.at(i)->text();
            list.push_back(signame);
        }
    }
    return list;
}

void QMapperDbModel::LoadFromTest(int testDB)
{
    //make some devices with sigs
    //some hard coded test situations

    //we use same devices and signals for each test sample:

    QString dev1name = "TestDev01";
    mapperDevNames.append(dev1name);

    for (int i=0; i<3; ++i)
    {
        QString signame = "signame" + QString::number(i+1);
        QStandardItem* newSig = new QStandardItem(signame);

        newSig->insertRow(0, new QStandardItem(dev1name));
        newSig->insertRow(1, new QStandardItem("output"));

        mapperSignals.append(newSig);
        qDebug() << "signal " <<mapperSignals.size()-1<< " is output";
    }

    dev1name = "Synth01";
    mapperDevNames.append(dev1name);
    for (int i=0; i<5; ++i)
    {
        QString signame = "signame" + QString::number(i+1);
        QStandardItem* newSig = new QStandardItem(signame);

        newSig->insertRow(0, new QStandardItem(dev1name));
        newSig->insertRow(1, new QStandardItem("input"));

        mapperSignals.append(newSig);
        qDebug() << "signal " <<mapperSignals.size()-1<< " is input";
    }

    /*
signal  0  is output
signal  1  is output
signal  2  is output
signal  3  is input
signal  4  is input
signal  5  is input
signal  6  is input
signal  7  is input
     */

    //make some maps
    if (testDB == 0)
    {
        mapperMapsSrc.append(0);
        mapperMapsDst.append(4);

        mapperMapsSrc.append(0);
        mapperMapsDst.append(5);

        mapperMapsSrc.append(2);
        mapperMapsDst.append(7);
    }
    else if (testDB == 1)
    {
        mapperMapsSrc.append(1);
        mapperMapsDst.append(4);

        mapperMapsSrc.append(1);
        mapperMapsDst.append(5);

        mapperMapsSrc.append(2);
        mapperMapsDst.append(7);
    }

}

const QString QMapperDbModel::getSigName(int idx)
{
    return mapperSignals.at(idx)->text();
}

const QString QMapperDbModel::getSigDevName(int idx)
{
    return mapperSignals.at(idx)->child(0)->text();
}

const bool QMapperDbModel::isOutputSig(int idx)
{
    return (mapperSignals.at(idx)->child(1)->text() == "output");
}

void QMapperDbModel::clearAll()
{
    mapperDevNames.clear();
    mapperSignals.clear();
    mapperMapsDst.clear();
    mapperMapsSrc.clear();

}

void QMapperDbModel::syncWith(const QMapperDbModel &model)
{
    clearAll();
    mapperDevNames = model.mapperDevNames;
    mapperSignals = model.mapperSignals;
    mapperMapsDst = model.mapperMapsDst;
    mapperMapsSrc = model.mapperMapsSrc;

    Q_EMIT dBUpdateSig();

    //TODO: we should probably emit update from here...
}
