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

private Q_SLOTS:


    void on_listView_pressed(const QModelIndex &index);

private:

    void loadMappingFromFile(QString filepath);

    Ui::VersionsDialog *ui;

    QVector<MapperJsonConfig*> myVersions;

    QStandardItemModel* versionList;
    QStringList annotationList;
};

#endif // VERSIONSDIALOG_H
