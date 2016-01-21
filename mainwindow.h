#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QTreeView>
#include <QTimer>
#include <QDebug>
#include "testmapperdevice.h"
#include "mapperdbthread.h"
#include "listtab.h"

#include "mapperUI/qmapperdbscene.h"
#include "mapperUI/qmapperdbmodel.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public Q_SLOTS:
    void selectedItem(QModelIndex model_idx);

private Q_SLOTS:
    void on_tabMain_tabBarDoubleClicked(int index);

    void on_pushButtonLaunchTestDevs_clicked();

    void refreshDB();

    void on_pushButtonStopTestDevs_clicked();

private:

    void createAndStartTestDevs();
    void stopAndDeleteTestDevs();

    Ui::MainWindow *ui;
    QStandardItemModel *listDevsTree;
    QStandardItemModel *listPropsTable;
    void syncTreeToTable(const QStandardItemModel* tree, QStandardItemModel* table);

    std::vector<testmapperdevice*> testDevices;
    mapperdbthread* myDB;
    QWidget* popUpWind;
    QTreeView* popUpContent;

    //signal list tab
    ListTab* mySigListViewTab;

    QTabWidget* myOtherViewtab; //placeholder for another mapping view
    QGraphicsView *graphicsView;
    QMapperDbScene *mapperScene;

    QMapperDbModel *mapperSceneDbModel;


    // for keeping track of last selected index in model
    QModelIndex lastIndex;

    QTimer* dbRefreshTimer; //TODO: ponder over whether the db refresh should be done by main UI (which then can update other models/views), or the mapperdbthread class...
};

#endif // MAINWINDOW_H
