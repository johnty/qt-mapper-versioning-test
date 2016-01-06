#ifndef SIGNALLIST_H
#define SIGNALLIST_H

#include "mapper/mapper_cpp.h"
#include <QWidget>

namespace Ui {
class SignalList;
}

class SignalList : public QWidget
{
    Q_OBJECT

public:
    explicit SignalList(QWidget *parent, const char *_label, int _is_src);
    ~SignalList();

    void clear();
    int addDevice(int index, const mapper::Device* dev);
    void addSignal(int parentindex, const mapper::Signal* sig);
    void expand();

private:
    Ui::SignalList *ui;
    int is_src;
};

#endif // SignalList_H
