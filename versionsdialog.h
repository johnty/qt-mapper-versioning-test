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

    void setVersionDataDir(QDir dir) {versionDataDir = dir;}
    void loadVersionHistory();

    const QMapperDbModel *getMostRecent();
    const QMapperDbModel *getVersionModel(int idx);

    void saveModelToJSON(QMapperDbModel* modelToSave, int idx);



Q_SIGNALS:
    void versionPressedSig(int idx);
    void versionLoadSig(int idx);
    void loadPressed(); //emit load pressed signal, will apply "active"layer's model onto the working model.
    void versionSaveSig(int); // emit save signal and move working copy into json file and save it

private Q_SLOTS:


    void on_listView_pressed(const QModelIndex &index);

    void on_pushButtonLoad_clicked();


    void on_pushButtonSave_clicked();

private:

    void loadMappingFromFile(QString filepath);
    void loadVersionHistory(QString folder_path);

    Ui::VersionsDialog *ui;

    QVector<MapperJsonConfig*> myVersions;

    QStandardItemModel* versionList;
    QStringList annotationList;

    QDir versionDataDir;
    QString annoStrTmp;
    QString annoStrDef;
};

#endif // VERSIONSDIALOG_H
