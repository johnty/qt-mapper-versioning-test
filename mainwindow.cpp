 #include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    myDB = new mapperdbthread();
    myDB->start();

    qDebug() <<"mapperDB thread started";


    dbRefreshTimer = new QTimer();
    connect(dbRefreshTimer, SIGNAL(timeout()), this, SLOT(refreshDB()));

    dbRefreshTimer->start(1000); //update once per second

    //QStringList testList;
    //testList.append("device 1");
    //testList.append("device 2");

    listDevsTree = new QStandardItemModel();
    listPropsTable = new QStandardItemModel();


    listDevsTree->setHorizontalHeaderItem(0, new QStandardItem("Devices"));
    QStandardItem* rootItem = listDevsTree->invisibleRootItem();

    QStandardItem* dev1 = new QStandardItem("Device 1");
    dev1->appendRow(new QStandardItem("sensor 0"));
    dev1->appendRow(new QStandardItem("sensor 1"));
    QStandardItem* dev2 = new QStandardItem("Device 2");
    dev2->appendRow(new QStandardItem("sensor 0"));
    dev2->appendRow(new QStandardItem("sensor 1"));
    dev2->appendRow(new QStandardItem("sensor 2"));
    dev2->appendRow(new QStandardItem("sensor 3"));
    rootItem->appendRow(dev1);
    rootItem->appendRow(dev2);

    //syncTreeToTable();

    //listDevs.setStringList(testList);
    ui->tableViewDevsSigs->setModel(listPropsTable);
    ui->treeView->setModel(listDevsTree);
    ui->treeView->expandAll();

    popUpWind = new QWidget();
    popUpContent = new QTreeView();
    popUpContent->setModel(listDevsTree);
    popUpContent->expandAll();
    //popUpContent->setParent(popUpWind);

    QGridLayout* layout = new QGridLayout();
    layout->addWidget(popUpContent);
    popUpWind->setLayout(layout);

 //popUpWind->setLayout(new QGridLayout());

    //popUpWind->show();


    QObject::connect(ui->treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(selectedItem(QModelIndex)));
}

MainWindow::~MainWindow()
{
    delete ui;

    myDB->stopThread();
    myDB->wait();
    qDebug() <<"db thread stopped";
    delete myDB;

    delete dbRefreshTimer;

    while (testDevices.size())
    {
        testmapperdevice* dev = testDevices.back();
        qDebug() <<"stopping dev " <<dev->getDevName();
        dev->stopRunning();
        //wait until stopped
        dev->wait();
        delete dev;
        testDevices.pop_back();
    }
}

void MainWindow::syncTreeToTable(const QStandardItemModel* tree, QStandardItemModel* table)
{
    if ((tree == nullptr) || (table == nullptr) )
    {
        return;
    }
    table->clear();

    for (int i=0; i<tree->rowCount(); ++i)
    {
        QStandardItem* dev = tree->item(i);
        qDebug() << "dev name = " << dev->text();
        for (int j=0; j<dev->rowCount(); j++)
        { //for each sig, we want a row with first column == dev name, second column == sig name

            //probably a terrible way of doing this. probably.
            table->setRowCount(table->rowCount()+1);
            table->setItem(table->rowCount()-1, 0, new QStandardItem(dev->text()));
            table->setItem(table->rowCount()-1, 1, new QStandardItem(dev->child(j)->text()));
        }
    }
}

void MainWindow::selectedItem(QModelIndex model_idx)
{
    qDebug() << "selected item from treeview: " << model_idx;
    listPropsTable->setHorizontalHeaderItem(0, new QStandardItem("Name"));
    listPropsTable->setHorizontalHeaderItem(1, new QStandardItem("Value"));
}

void MainWindow::on_tabMain_tabBarDoubleClicked(int index)
{
    if (index == ui->tabMain->indexOf(ui->tabDevs))
    {
        popUpWind->show();
    }
    else if (index == ui->tabMain->indexOf(ui->tabDB))
    {

    }
}

void MainWindow::on_pushButtonLaunchTestDevs_clicked()
{
    createDevs();
}

void MainWindow::createDevs()
{
    for (int i=0; i<2; i++)
    {
        QString dev_name = "mydevice_"+QString::number(i);
        int numIns = 1+3*(float)qrand()/RAND_MAX;
        int numOuts = 1+3*(float)qrand()/RAND_MAX;
        qDebug() <<"creating " << dev_name <<" with" << numIns<<"inputs and"<< numOuts<< "outputs";
        testmapperdevice* newdev = new testmapperdevice(numIns,numOuts,dev_name);
        newdev->start(); //note: we have to start it so it will update (and hence showup in db, etc...)
        testDevices.push_back(newdev);
    }
}

void MainWindow::refreshDB()
{
    qDebug() <<"refreshing DB...";

    for (auto devname : myDB->getDeviceList())
    {
        qDebug() << "dev registered: " << devname;
    }
    return;

    std::vector<QString> listDevs = myDB->getDeviceList();
    for (int i=0; i<listDevs.size(); ++i)
    {
        qDebug() << listDevs.at(i);
    }
}
