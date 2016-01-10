#ifndef SIGNALLIST_H
#define SIGNALLIST_H

#include "mapper/mapper_cpp.h"
#include <QWidget>
#include <QObject>

namespace Ui {
class SignalList;
}

class SignalList : public QWidget
{
    Q_OBJECT

Q_SIGNALS:
    void signalSelected(QString dev, QString sig);


public:
    explicit SignalList(QWidget *parent, const char *_label, int _is_src);
    ~SignalList();

    void setRowHeight(int rowH) {row_height = rowH;}
    void clear();
    //TODO: decouple UI stuff with mapper stuff, and
    // instead use a custom data container to hold dev/sig info
    // (since we will want to render off-line stuff from versioning system)
    // QStandardItemModel should be sufficient for this purpose.
    int addDevice(int index, const mapper::Device* dev);
    void addSignal(int parentindex, const mapper::Signal* sig, QString devname);
    void expand();

    QString getSelectedDev() {return selected_dev;}
    QString getSelectedSig() {return selected_sig;}

    int getSelectedRow();


private Q_SLOTS:
    void on_tree_clicked(const QModelIndex &index);

private:
    Ui::SignalList *ui;
    int is_src;
    QString selected_dev;
    QString selected_sig;
    int row_height;
    int selected_row;
};

#endif // SignalList_H
