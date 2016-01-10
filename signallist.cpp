#include "signallist.h"
#include "ui_signallist.h"
#include <QDebug>

SignalList::SignalList(QWidget *parent, const char *_label, int _is_src) :
    QWidget(parent),
    ui(new Ui::SignalList)
{
    ui->setupUi(this);
    ui->label->setText(_label);
    is_src = _is_src;
    row_height = 20;
    selected_row = -1;
    ui->tree->setColumnWidth(0, 65);
    ui->tree->setColumnWidth(1, 65);
    ui->tree->setColumnWidth(2, 35);
    ui->tree->setColumnWidth(3, 15);

}

SignalList::~SignalList()
{
    delete ui;
}

void SignalList::clear()
{
    ui->tree->clear();
}

int SignalList::addDevice(int index, const mapper::Device* dev)
{
    QTreeWidgetItem *qdev = new QTreeWidgetItem((QTreeWidget*)0,
                                                QStringList(QString(dev->name().c_str())));
    qdev->setSizeHint(0, QSize(30, 20));
    ui->tree->insertTopLevelItem(index, qdev);
    return ui->tree->indexOfTopLevelItem(qdev);
}

void SignalList::addSignal(int parentindex, const mapper::Signal* sig, QString devname)
{
    QTreeWidgetItem *qdev = ui->tree->topLevelItem(parentindex);

    QTreeWidgetItem *qsig = new QTreeWidgetItem();
    qsig->setSizeHint(0, QSize(30,20));
    qsig->setText(0, devname);
    qsig->setText(1, QString(sig->name().c_str()));
    qsig->setText(2, QString(QChar(sig->type())));
    qsig->setText(3, QString::number(sig->length()));
    qdev->addChild(qsig);
}


void SignalList::expand()
{
    ui->tree->expandAll();
}

void SignalList::on_tree_clicked(const QModelIndex &index)
{
    qDebug() <<"sig tree clicked row = " << index.row() << " col = " << index.column();
    qDebug() << "currItem = " << ui->tree->currentItem()->text(0);
    if (!ui->tree->currentItem()->childCount())
    {
        //we've clicked on a signal
        //better ways to determine this?
        //
        QString signame = ui->tree->currentItem()->text(0);
        QString devname = ui->tree->currentItem()->parent()->text(0);

        qDebug() << "dev = " <<devname <<" sig = " <<signame;
        selected_dev = devname;
        selected_sig = signame;
        Q_EMIT signalSelected(devname, signame); //if we keep track of these, we could simply call getters instead
    }
    else
    {
        qDebug() <<"is dev";
        selected_dev = "";
        selected_sig = "";
        Q_EMIT signalSelected("", "");
    }

}

int SignalList::getSelectedRow()
{
    return selected_row;
}
