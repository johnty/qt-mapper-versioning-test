#include "testmapperdevice.h"

testmapperdevice::testmapperdevice(int numinputs, int numoutputs, QString devname)
{
    numInputs = numinputs;
    numOutputs = numoutputs;

    myDevPtr = new mapper::Device(devname.toStdString());
    for (int i=0; i<numinputs; i++)
    {
        QString signame = "in" + QString::number(i);
        Signal testsig = myDevPtr->add_input(signame.toStdString(),1,'i',0,0,0,0,0);
    }
    for (int i=0; i<numoutputs; i++)
    {
        QString signame = "out" + QString::number(i);
        Signal testsig = myDevPtr->add_output(signame.toStdString(),1,'i',0,0,0);
    }
    while (!myDevPtr->ready())
    {
        myDevPtr->poll(100);
        qDebug() <<"waiting for device " <<devname;
    }
    qDebug() <<"created device with name = " << myDevPtr->name().c_str();


}

testmapperdevice::~testmapperdevice()
{
    if (myDevPtr != NULL) {
        delete myDevPtr;
    }
}

void testmapperdevice::stopRunning()
{
    myLock.lock();
    keepGoing = false;
    myLock.unlock();
}


void testmapperdevice::run()
{
    keepGoing = true;
    qDebug() <<"testmapperdevice thread started";
    while (keepGoing)
    {
        for (int i=0; i<numOutputs; i++)
        {
            Signal::Query qry = myDevPtr->signals(MAPPER_DIR_OUTGOING);
            for (; qry != qry.end(); qry++)
            {
                Signal sig = *qry;
                sig.update(0);
                //qDebug() <<myDevPtr->name().c_str()<<": updating output signal"<< sig.name().c_str();
            }
        }
        //qDebug() << myDevPtr->name().c_str()<<": polling dev inputs";
        myDevPtr->poll(500);

    }
    qDebug() <<"testmapperdevice done. goodbye.";
}
