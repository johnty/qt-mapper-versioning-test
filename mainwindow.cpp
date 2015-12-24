 #include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

    popUpWind->show();


    QObject::connect(ui->treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(selectedItem(QModelIndex)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::syncTreeToTable()
{
    if ((listDevsTree == nullptr) || (listPropsTable == nullptr) )
    {
        return;
    }
    listPropsTable->clear();


    for (int i=0; i<listDevsTree->rowCount(); ++i)
    {
        QStandardItem* dev = listDevsTree->item(i);
        qDebug() << "dev name = " << dev->text();
        for (int j=0; j<dev->rowCount(); j++)
        { //for each sig, we want a row with first column == dev name, second column == sig name

            //probably a terrible way of doing this. probably.
            listPropsTable->setRowCount(listPropsTable->rowCount()+1);
            listPropsTable->setItem(listPropsTable->rowCount()-1, 0, new QStandardItem(dev->text()));
            listPropsTable->setItem(listPropsTable->rowCount()-1, 1, new QStandardItem(dev->child(j)->text()));
        }
    }
}

void MainWindow::selectedItem(QModelIndex model_idx)
{
    qDebug() << "selected item from treeview: " << model_idx;
    listPropsTable->setHorizontalHeaderItem(0, new QStandardItem("Name"));
    listPropsTable->setHorizontalHeaderItem(1, new QStandardItem("Value"));
}
