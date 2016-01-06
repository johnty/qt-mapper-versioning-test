#ifndef TAB_H
#define TAB_H

#include <QTabWidget>
//#include "logic.h"

enum tabType {
    GRAPH_TAB,
    LIST_TAB,
};

class Tab : public QTabWidget
{
public:
    //Tab(QTabWidget *_parent, mapperGUIData data);
    Tab(QTabWidget *_parent);
    virtual void update() = 0;
    virtual void deviceEvent() = 0;
    virtual void linkEvent() = 0;

private:
    //mapperGUIData data;
};

#endif // TAB_H
