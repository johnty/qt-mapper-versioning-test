#include "versionsdialog.h"
#include "ui_versionsdialog.h"

VersionsDialog::VersionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VersionsDialog)
{
    ui->setupUi(this);

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
    dir.cdUp(); dir.cdUp(); dir.cdUp();
    QString app_root = dir.absolutePath();
    QString file = app_root+"/test_out.json";
    if (myVersions.size())
    {
        qDebug() <<"saving to file " <<file;
        if (myVersions.at(0)->SaveConfigToJSONFile(file))
            qDebug() << "save success!";
    }
}

void VersionsDialog::fromMapperDBView(const QMapperDbModel *dbviewmodell)
{

}

void VersionsDialog::loadMappingFromFile(QString filepath)
{
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
