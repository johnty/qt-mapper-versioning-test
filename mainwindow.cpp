 #include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() <<"mapperDB thread started";


    //NOTE: timer removed after
    //dbRefreshTimer = new QTimer();
    //connect(dbRefreshTimer, SIGNAL(timeout()), this, SLOT(refreshDB()));
    //dbRefreshTimer->start(1000); //update once per second

    //db update trigger
    //connect(myDB, SIGNAL(devUpdatedSig()), this, SLOT(refreshDB()));

    if (LIVE_MAPPER_DB)
    {
        myDB = new mapperdbthread();
        myDB->start();
        connect(myDB, SIGNAL(sigUpdatedSig()), this, SLOT(refreshDB()));
        connect(myDB, SIGNAL(mapUpdatedSig()), this, SLOT(refreshDB()));
    }
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

    popUpDevicesWind = new QWidget();
    popUpContent = new QTreeView();
    popUpContent->setModel(listDevsTree);
    popUpContent->expandAll();
    //popUpContent->setParent(popUpWind);

    //init the list tab

    mySigListViewTab = new ListTab(ui->tabMapperView);
    ui->tabMapperView->setTabText(0, "List View");

    //QObject::connect(ui->tabMapperView, SIGNAL(on) )

    myOtherViewtab = new QTabWidget();
    ui->tabMapperView->addTab(myOtherViewtab, "Signals View");
    ui->tabMapperView->setCurrentIndex(1);

    QGridLayout *otherLayout = new QGridLayout(myOtherViewtab);

    graphicsView = new QGraphicsView();
    otherLayout->addWidget(graphicsView);
    mapperScene = new QMapperDbScene(this);
    graphicsView->setScene(mapperScene);
    mapperScene->setSceneRect(graphicsView->rect());

    //ui->tabMapperView->setTabText(1, "Other View");

    mapperSceneDbModel = new QMapperDbModel();
    //mapperSceneDbModel->LoadFromTest();

    mapperSceneDbModelActive = new QMapperDbModel();
    //mapperSceneDbModelActive->LoadFromTest();

    mapperScene->setMapperDbModel(mapperSceneDbModel);
    mapperScene->setMapperDbModelActive(mapperSceneDbModelActive);

    //TODO: use this in the future... for now, refreshDB from MainWindow...
    //QObject::connect(mapperSceneDbModel, SIGNAL(dBUpdateSig()), mapperScene, SLOT(dpUpdated()));
    QObject::connect(mapperSceneDbModel, SIGNAL(dBUpdateSig()), mapperScene, SLOT(dpUpdated()));

    QObject::connect(mapperScene, SIGNAL(sceneMapSig(int, int)), this, SLOT(sceneMapSigReceived(int,int)));
    QObject::connect(mapperScene, SIGNAL(sceneUnMapSig(int, int)), this, SLOT(sceneUnMapSigReceived(int,int)));




    QGridLayout* layout = new QGridLayout();
    layout->addWidget(popUpContent);
    //layout->addWidget(ui->tabDevs);
    popUpDevicesWind->setLayout(layout);
    //popUpWind->show();
    QObject::connect(ui->treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(selectedItem(QModelIndex)));

    resize(QDesktopWidget().availableGeometry(this).size()*0.7);

    //pop up the versioning dialog
    popupVersionsDlg = new VersionsDialog(this);
    popupVersionsDlg->show();
    popupVersionsDlg->move(this->window()->x()+this->window()->width(), this->window()->y());


    //note: depending on project.user settings,
    //this folder may have to be corrected!
    QDir dir(QDir::current());
    dir.cdUp(); dir.cdUp(); dir.cdUp();; dir.cdUp();
    QString app_root = dir.absolutePath();
    //popupVersionsDlg->testLoadSaveJSON(file);

    //load all the previous versions in this folder:
    QString version_dir = app_root+"/versiondata";
    popupVersionsDlg->loadHistory(version_dir);
    QObject::connect(popupVersionsDlg, SIGNAL(versionPressedSig(int)), this, SLOT(versionPressed(int)));

    const QMapperDbModel* versionModel = popupVersionsDlg->getMostRecent();
    if (versionModel)
    {
        mapperSceneDbModel->syncWith(*versionModel);
        mapperSceneDbModelActive->syncWith(*versionModel);
    }
    mapperScene->updateScene();
}

MainWindow::~MainWindow()
{
    delete ui;

    if (LIVE_MAPPER_DB)
    {
        myDB->stopThread();
        myDB->wait();
        qDebug()<<"closing mapper db...";
        delete myDB;
        qDebug()<<"   ...done.";
    }

    delete dbRefreshTimer;

    stopAndDeleteTestDevs();
}

void MainWindow::resizeEvent(QResizeEvent* e)
{
    //qDebug() <<"main resize";
    if ( (popupVersionsDlg != NULL) && (!popupVersionsDlg->isHidden()) )
    {
        popupVersionsDlg->move(this->window()->x()+this->window()->width()
                               , this->window()->y());
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

void MainWindow::sceneMapSigReceived(int src, int dst)
{
    qDebug() << "UI mapping received: " <<src<<" to " <<dst;
    if (LIVE_MAPPER_DB)
        myDB->tryMap(src, dst);
    else
    {
        mapperSceneDbModel->updateMap(src, dst, true);
    }
}

void MainWindow::sceneUnMapSigReceived(int src, int dst)
{
    qDebug() << "UI unmap received: " <<src<<" to " <<dst;
    if (LIVE_MAPPER_DB)
        myDB->tryMap(src, dst, false);
    else
    {
        mapperSceneDbModel->updateMap(src, dst, false);
    }
}

void MainWindow::versionPressed(int idx)
{
    qDebug()<< "MainWindow: version sig " <<idx;
    if (popupVersionsDlg->getVersionModel(idx) != nullptr) {
        qDebug()<< " old size = "<< mapperSceneDbModelActive->getMapSrcs().size();
        mapperScene->clearActiveLayer();//NOTE: we have to clear the layer
        // before adding new stuff, otherwise the update doesn't remove stuff proerply.
        // TODO: this is an undesired behaviour of how the scene and scene layer update!!
        mapperSceneDbModelActive->syncWith(*popupVersionsDlg->getVersionModel(idx));
        qDebug()<< " new size = "<< mapperSceneDbModelActive->getMapSrcs().size();
        //mapperScene->update();
        mapperScene->setActiveLayerVisible(true);
        mapperScene->updateScene();
    }
    else {
        qDebug() <<"index > stored versions; clicked on working version";
        //set top layer to invisible
        mapperScene->setActiveLayerVisible(false);
    }
}

void MainWindow::on_tabMain_tabBarDoubleClicked(int index)
{
    if (index == ui->tabMain->indexOf(ui->tabDevs))
    {
        popUpDevicesWind->show();
    }
    else if (index == ui->tabMain->indexOf(ui->tabMapperMain))
    {

    }
    else if (index == ui->tabMain->indexOf(ui->tabVersions))
    {
        if (popupVersionsDlg != NULL)
        {
            popupVersionsDlg->show();
        }
    }
}

void MainWindow::on_pushButtonLaunchTestDevs_clicked()
{
    createAndStartTestDevs();
}

void MainWindow::createAndStartTestDevs()
{
    for (int i=0; i<2; i++)
    {
        QString dev_name = "Device_"+QString::number(i+1);
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


    //if (devList.size() != listDevsTree->rowCount())
    if (1) //TODO: compare/filter out changes so we don't necessarily refresh entire list
    {       //right now the update when initializing devices take a while since
            //with each action we get an entire refresh/query...
        //qDebug() << "numDevs changed; refreshing tree";


        //TODO: the this structure is probably better than
        // the one in the current QMapperDbModel..
        const std::vector<QString>& devList = myDB->getDeviceList();
        listDevsTree->clear();
        listDevsTree->setHorizontalHeaderItem(0, new QStandardItem("Devices"));
        for (auto devname : devList)
        {
            //for each device
            QStandardItem* dev = new QStandardItem(devname);
            //listDevsTree->setHorizontalHeaderItem(0, new QStandardItem("Devices"));

            //add signals to dev
            const std::vector<QString> sigList = myDB->getSigList(devname);
            for (int i=0; i< sigList.size(); i++)
            {
                //qDebug() <<"Test Tree Model: dev = "<<devname<<"  sig = " << sigList.at(i);
                dev->appendRow(new QStandardItem(sigList.at(i)));
            }

            //add device to list (at root level)
            QStandardItem* rootItem = listDevsTree->invisibleRootItem();
            rootItem->appendRow(dev);

            //qDebug() << "dev added to tree test model: " << devname;
        }

        //note: there isn't a super elegant way to keep tree
        //open exactly the way we had between updates, unless we
        // a.) "double buffer" the model and only refresh when changed
        // b.) keep track of treeview status, and reset it every time
        // c.) something else?
        // for now, just leave it in expanded mode...
        //ui->treeView->expandAll();

        //update the list tab here:
        //mySigListViewTab->deviceEvent(myDB->getDB());

//REMINDER: this only happens when number of devices change, which is not ideal...
// we should do more in depth comparisons, or better yet hook up signal handlers to the instance of the class
        // another alternative hacky but better way may be tor

        syncAndRefreshScene();

    }
}

void MainWindow::on_pushButtonStopTestDevs_clicked()
{
    stopAndDeleteTestDevs();
}

void MainWindow::stopAndDeleteTestDevs()
{
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

void MainWindow::syncAndRefreshScene()
{   //TODO: consider making the DbModels emit update commands
    // to their attached scenes...
    if (LIVE_MAPPER_DB)
        myDB->syncRenderModel(mapperSceneDbModel); //update the data
    //Q_EMIT dBUpdateSig();    //trigger render
}
