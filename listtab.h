#ifndef LISTTAB_H
#define LISTTAB_H

#include "tab.h"
#include "signallist.h"
#include "linkview.h"
#include "connectionprops.h"

#include <QTabWidget>
#include <QSplitter>
#include <QGroupBox>
#include <QVBoxLayout>

class ListTab : public Tab
{
//    Q_OBJECT

public:
    //ListTab(QTabWidget *parent, mapperGUIData data);
    ListTab(QTabWidget *parent);
    ~ListTab();

    void update();
    void deviceEvent();
    void linkEvent();

private:
    ConnectionProps *props;
    SignalList *sources;
    SignalList *destinations;
    LinkView *links;
    //mapperGUIData data;
};

#endif // LISTTAB_H
