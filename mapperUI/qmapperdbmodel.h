#ifndef QMAPPERDBMODEL_H
#define QMAPPERDBMODEL_H

#include <QStandardItemModel>
#include <QObject>
#include <QDebug>

// represents the entire state of the mapper network's devices, signals, and maps for drawing
// with the QT framework GUI objects
// Interfaces with:
//    git loader
//    QSql (is this still necessary?)
//    Mapper DB
//    dummy test


class QMapperDbModel
{
public:
    QMapperDbModel();

    //create a bunch of test devices/signals/maps
    void LoadFromTest(int testDB = 0);

    int getNumSigs() {
        return mapperSignals.size();
    }

    void clearAll();

    void syncWith(const QMapperDbModel& model);

    const QStandardItem* getSigItem(int index) {return mapperSignals.at(index);}
    const QString getSigName(int idx);
    const QString getSigDevName(int idx);
    const bool isOutputSig(int idx);

    //TODO: decide on QVector or std::vector and then make it consistent throughout!!
    const QVector<int>& getMapSrcs() { return mapperMapsSrc;}
    const QVector<int>& getMapDsts() { return mapperMapsDst;}

    void addDevice(QString devName);
    void addSignal(QString devName, QString sigName, bool isInput);
    void removeDevice(QString devName);
    void removeSignal(QString devName, QString sigName);

    const std::vector<QString> getDevs();
    const std::vector<QString> getDevSigs(QString devname);
private:

    //TODO: more organized data structures
    QVector<QString> mapperDevNames;
    QVector<QStandardItem*> mapperSignals;
    //QVector<QStandardItem*> mapperMaps;

    //probably a pair structure would suffice?
    QVector<int> mapperMapsSrc;
    QVector<int> mapperMapsDst;
};

#endif // QMAPPERDBMODEL_H