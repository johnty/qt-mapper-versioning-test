#include "versionsdialog.h"
#include "ui_versionsdialog.h"

VersionsDialog::VersionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VersionsDialog)
{

    ui->setupUi(this);
    versionList = new QStandardItemModel(this);
    ui->listView->setModel(versionList);

    ui->pushButtonSave->setEnabled(false);
    ui->pushButtonLoad->setEnabled(false);

    versionDataDir = QDir("");
    annoStrDef = "[insert annotation here]";
    annoStrTmp = "";
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

void VersionsDialog::loadVersionHistory(QString folder_path)
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

void VersionsDialog::loadVersionHistory()
{
    QString folder_path = versionDataDir.absolutePath();
    loadVersionHistory(folder_path);
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

void VersionsDialog::saveModelToJSON(QMapperDbModel *modelToSave, int idx)
{

    //as we trigger a save from main window, do following:
    //  1 write json file of current mapperScene
    //  2 write txt file of current annotation
    //  3 load newly stored json and text files
    //   - and add to annotation and versions list
    //   - update versionList model

    MapperJsonConfig* newVersionJSON = new MapperJsonConfig();
    newVersionJSON->FromUIDbModel(modelToSave);
    newVersionJSON->FromUIDbToJson();
    qDebug() <<"versionsDialog saving model to json version " <<idx;
    QString jsonfilename = versionDataDir.absolutePath()+"/version"+QString::number(idx)+".json";
    newVersionJSON->SaveConfigToJSONFile(jsonfilename);

    //save annotation:
    QString annonfilename = versionDataDir.absolutePath()+"/version"+QString::number(idx)+".txt";
    QFile file(annonfilename);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        stream << ui->annotationEdit->document()->toPlainText();
    }


    //and now do a load!
    loadMappingFromFile(jsonfilename);
    //TODO: modularize this!
    QFile txtFile(annonfilename);
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
    //store the annotation of working model
    if (index.row() == annotationList.size())
    {
        ui->annotationEdit->setReadOnly(false);
//        if (annoStrTmp != "")
//            ui->annotationEdit->setText(annoStrTmp);
//        else
        ui->annotationEdit->setText("[insert annotation here]");
    }
    else //store the working copy annotation
    {
//        if (annoStrTmp == "") //note: the trick is to only do this once
//            annoStrTmp = ui->annotationEdit->document()->toPlainText();
        ui->annotationEdit->setReadOnly(true);
    }

    //load existing annotation
    if (index.row() < myVersions.size())
    {
        ui->annotationEdit->setText(annotationList.at(index.row()));
    }
    Q_EMIT versionPressedSig(index.row());


    if (index.row() < myVersions.size())
    {
        ui->pushButtonSave->setEnabled(false);
        ui->pushButtonLoad->setEnabled(true);
    }
    else if (index.row() == myVersions.size()) //we've selected "working version"
    {
        ui->pushButtonLoad->setEnabled(false);
        ui->pushButtonSave->setEnabled(true);
    }

}

void VersionsDialog::on_pushButtonLoad_clicked()
{
    int idx = ui->listView->currentIndex().row();
    qDebug() <<"loading row" << idx;
    if (idx < myVersions.size())
    {
        Q_EMIT versionLoadSig(idx);
    }
}

void VersionsDialog::on_pushButtonSave_clicked()
{

    //emit message so main window can trigger db save from current working version

    //add it beneath the "working version", which is at count()-1
    // the new version index should be count().

    int listSize = versionList->rowCount();

    //test
    QString name = "version" + QString::number(listSize-1) + ".json";
    versionList->insertRow(listSize-1, new QStandardItem(name));
    //test
    qDebug() <<"added new version: " <<name;

    Q_EMIT versionSaveSig(myVersions.size());
}
