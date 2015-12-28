#ifndef TESTMAPPERDEVICE_H
#define TESTMAPPERDEVICE_H

#include <QThread>
#include <QObject>
#include <QDebug>
#include <QString>
#include <QMutex>
#include <mapper/mapper_cpp.h>

using namespace mapper;
enum SignalType
{
    SIGNAL_INPUT,
    SIGNAL_OUTPUT
};

class testmapperdevice : public QThread, public QObject
{
public:
    testmapperdevice(int numinputs, int numoutputs, QString devname);
    ~testmapperdevice();
    void addSignal(SignalType type);

    void stopRunning();

private:

    void init();

    mapper_device dev;

    Device *myDevPtr;

    bool keepGoing;

    int numInputs;
    int numOutputs;
    int devIndex;

    void run();

    QMutex myLock;


};

#endif // TESTMAPPERDEVICE_H
