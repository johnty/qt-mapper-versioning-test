#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QTreeView>
#include <QDebug>


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

private:
    Ui::MainWindow *ui;
    QStandardItemModel *listDevsTree;
    QStandardItemModel *listPropsTable;
    void syncTreeToTable(const QStandardItemModel* tree, QStandardItemModel* table);

    QWidget* popUpWind;
    QTreeView* popUpContent;
};

#endif // MAINWINDOW_H
