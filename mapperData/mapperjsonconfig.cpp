#include "mapperjsonconfig.h"

MapperJsonConfig::MapperJsonConfig()
{

}

MapperJsonConfig::MapperJsonConfig(const QString filePath, QIODevice::OpenModeFlag mode)
{
    QFile loadFile(filePath);

    if (!loadFile.open(mode))
    {
        qWarning("cannot open json file!");
    }
    else
    {
        qDebug() << "json file opened.";
    }

    QByteArray loadData = loadFile.readAll();

    QJsonDocument jsonDoc(QJsonDocument::fromJson(loadData));

    if (jsonDoc.isObject()) //the top level is a "mapping" object
    {
        mapperConfigObject = jsonDoc.object();
        ParseFile(mapperConfigObject);
    }

}

bool MapperJsonConfig::SaveConfigToJSONFile(QString filePath)
{
    QJsonDocument jsonDoc(mapperConfigObject);
    QFile saveFile(filePath);
    if (!saveFile.open((QIODevice::WriteOnly)))
    {
        qWarning() <<"cannot open "<<saveFile.fileName();
        return false;
    }
    saveFile.write(jsonDoc.toJson());
    saveFile.close();
    return true;
}

bool MapperJsonConfig::ParseFile(const QJsonObject& json_obj)
{
    for (QJsonObject::const_iterator it = json_obj.begin(); it != json_obj.end(); it++)
    {
        //iterator is not really necessary...
        QString name = it.key();

        if (QString::compare(name, "mapping")==0)
        {
            QJsonValue val = it.value();
            if (val.isObject())
            {
                qDebug() << "***SOURCES***";
                QJsonArray src_arr = val.toObject()["sources"].toArray();
                for (int i=0; i<src_arr.size(); i++)
                {
                    //todo: type checking, just in case?
                    QJsonObject curr_src = src_arr.at(i).toObject();
                    QString id = curr_src["id"].toString();
                    QString device = curr_src["device"].toString();
                    QString signal = curr_src["signal"].toString();
                    qDebug()<<"     id: "<<id<<" device: "<<device<<" sig: "<<signal;
                    MAPPER_SRC_DST mapper_src;
                    mapper_src.id = id;
                    mapper_src.dev = device;
                    mapper_src.sig = signal;
                    mySources.append(mapper_src);

                }
                qDebug() << "***DESTINATIONS***";
                QJsonArray dst_arr = val.toObject()["destinations"].toArray();
                for (int i=0; i<dst_arr.size(); i++)
                {
                    QJsonObject curr_dst = dst_arr.at(i).toObject();
                    QString id = curr_dst["id"].toString();
                    QString device = curr_dst["device"].toString();
                    QString signal = curr_dst["signal"].toString();
                    qDebug()<<"     id: "<<id<<" device: "<<device<<" sig: "<<signal;
                    MAPPER_SRC_DST mapper_dst;
                    mapper_dst.id = id;
                    mapper_dst.dev = device;
                    mapper_dst.sig = signal;
                    myDestinations.append(mapper_dst);
                }
                qDebug() << "***CONNECTIONS***";
                QJsonArray con_arr = val.toObject()["connections"].toArray();
                for (int i=0; i<con_arr.size(); i++)
                {
                    QJsonObject curr_con = con_arr.at(i).toObject();
                    int mute = curr_con["mute"].toInt();
                    QString mode = curr_con["mode"].toString();
                    QString expr = curr_con["expression"].toString();
                    QJsonArray range = curr_con["range"].toArray(); //NOTE: range will be changed to subarrays??

                    QString clipMin = curr_con["clipMin"].toString();
                    QString clipMax= curr_con["clipMax"].toString();
                    myConnectionExprs.append(expr);
                    qDebug()<<"     mute: "<<mute<<" mode: "<<mode<<" expr: "<<expr;
                    //TODO:
                }
            }
            toDB();
            fromDB();


        }
        else
        {
            qWarning("invalid mapping file!");
        }
    }

}
