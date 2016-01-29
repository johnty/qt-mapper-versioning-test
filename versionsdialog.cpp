#include "versionsdialog.h"
#include "ui_versionsdialog.h"

VersionsDialog::VersionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VersionsDialog)
{

    ui->setupUi(this);
    versionList = new QStandardItemModel(this);
    ui->listView->setModel(versionList);
}

VersionsDialog::~VersionsDialog()
{
    while (myVersions.size())
    {
        delete myVersions.at(0);
        myVersions.pop_front();
    }
    delete ui;
}

void VersionsDialog::testLoadSaveJSON(QString filename)
{
    QDir dir(QDir::current());
    loadMappingFromFile(filename);
    dir.cdUp(); dir.cdUp(); dir.cdUp(); dir.cdUp();
    QString app_root = dir.absolutePath();
    QString file = app_root+"/test_out.json";
    if (myVersions.size())
    {
        qDebug() <<"saving to file " <<file;

        myVersions.back()->FromUIDbToJson(); //sync ui with
        if (myVersions.back()->SaveConfigToJSONFile(file))
            qDebug() << "save success!";
    }
}

void VersionsDialog::fromMapperDBView(const QMapperDbModel *dbviewmodell)
{

}

void VersionsDialog::loadHistory(QString folder_path)
{
    QStringList nameFilter("*.json");
    QDir directory(folder_path);
    QStringList jsonFileList = directory.entryList(nameFilter);
    for (auto &file : jsonFileList)
    {
        qDebug() <<"found json file: " << file;
        QString full_path = folder_path+"/"+file;
        loadMappingFromFile(full_path);
        QStandardItem* item = new QStandardItem(file);
        versionList->appendRow(item);
    }
    nameFilter = QStringList("*.txt");
    QStringList annoFileList = directory.entryList(nameFilter);
    for (auto &file : annoFileList)
    {
        qDebug()<<"found txt file: " << file;
        QString full_path = folder_path+"/"+file;
        QFile txtFile(full_path);
        if (!txtFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug()<<"error reading txt annotation!";
        }
        else
        {
            //expect a single line only...
            QTextStream in(&txtFile);
            QString annoText = in.readLine();
            qDebug() <<"txt contents: " <<annoText;
            annotationList.append(annoText);
        }
    }
    versionList->appendRow(new QStandardItem("working version"));
}

const QMapperDbModel *VersionsDialog::getMostRecent()
{
    if (myVersions.size())
    {  //could also be at position 0, depending on file organization...
        return myVersions.at(myVersions.size()-1)->getMapperUIDbModel();
    }
    else
        return nullptr;
}

const QMapperDbModel *VersionsDialog::getVersionModel(int idx)
{
    for (int i=0; i<myVersions.size(); i++)
    {
        const QMapperDbModel* model = myVersions.at(i)->getMapperUIDbModel();
        const QVector<int> srcs = model->getMapSrcs();
        qDebug() << " version # "<<i<< "size"<< srcs.size();
    }

    if (idx < myVersions.size())
    {
        return myVersions.at(idx)->getMapperUIDbModel();
    }
    else
        return nullptr;
}

void VersionsDialog::loadMappingFromFile(QString filepath)
{
    qDebug()<<"opening json file " <<filepath;
    QFile myfile(filepath);
    if (myfile.open(QFile::ReadOnly))
    {
        QString data;
        QTextStream s1(&myfile);
        data.append(s1.readAll());

        //put into text view
        //mapperTextViewTab->setText(data);

        MapperJsonConfig* mapperJSON = new MapperJsonConfig(filepath, QIODevice::ReadOnly);

        myVersions.push_back(mapperJSON);

    }
    myfile.close();
}

void VersionsDialog::on_listView_pressed(const QModelIndex &index)
{
    qDebug()<<"version pressed" << index.row();
    if (index.row() < annotationList.size())
        ui->annotationEdit->setText(annotationList.at(index.row()));

    Q_EMIT versionPressedSig(index.row());

}
