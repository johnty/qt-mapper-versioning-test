#ifndef VERSIONSDIALOG_H
#define VERSIONSDIALOG_H

#include <QDialog>
#include "mapperUI/qmapperdbmodel.h"
#include "mapperData/mapperjsonconfig.h"

namespace Ui {
class VersionsDialog;
}

class VersionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VersionsDialog(QWidget *parent = 0);
    ~VersionsDialog();

    void testLoadSaveJSON(QString filename);

    void fromMapperDBView(const QMapperDbModel* dbviewmodell);

    void loadHistory(QString folder_path);

    const QMapperDbModel *getMostRecent();
    const QMapperDbModel *getVersionModel(int idx);

Q_SIGNALS:
    void versionPressedSig(int idx);
    void versionLoadSig(int idx);
    void loadPressed(); //emit load pressed message, will apply "active"layer's model onto the working model.

private Q_SLOTS:


    void on_listView_pressed(const QModelIndex &index);

    void on_pushButtonLoad_clicked();


private:

    void loadMappingFromFile(QString filepath);

    Ui::VersionsDialog *ui;

    QVector<MapperJsonConfig*> myVersions;

    QStandardItemModel* versionList;
    QStringList annotationList;
};

#endif // VERSIONSDIALOG_H
