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
    listDevsTable = new QStandardItemModel();


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

    syncTreeToTable();

    //listDevs.setStringList(testList);


    ui->tableViewDevsSigs->setModel(listDevsTable);
    ui->treeView->setModel(listDevsTree);
    ui->treeView->expandAll();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::syncTreeToTable()
{
    if ((listDevsTree == nullptr) || (listDevsTable == nullptr) )
    {
        return;
    }
    listDevsTable->clear();


    for (int i=0; i<listDevsTree->rowCount(); ++i)
    {
        QStandardItem* dev = listDevsTree->item(i);
        qDebug() << "dev name = " << dev->text();
        for (int j=0; j<dev->rowCount(); j++)
        { //for each sig, we want a row with first column == dev name, second column == sig name

            //probably a terrible way of doing this. probably.
            listDevsTable->setRowCount(listDevsTable->rowCount()+1);
            listDevsTable->setItem(listDevsTable->rowCount()-1, 0, new QStandardItem(dev->text()));
            listDevsTable->setItem(listDevsTable->rowCount()-1, 1, new QStandardItem(dev->child(j)->text()));
        }
    }
}
