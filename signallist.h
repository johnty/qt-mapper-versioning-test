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
    int addDevice(int index, mapper::Device* dev);
    void addSignal(int parentindex, mapper::Signal* sig);

private:
    Ui::SignalList *ui;
    int is_src;
};

#endif // SignalList_H
