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
        myJsonMapperConfig = jsonDoc.object();
        ParseJsonObject(myJsonMapperConfig);
        toDB();
    }

}

bool MapperJsonConfig::SaveConfigToJSONFile(QString filePath)
{
    //TODO: remove test
    FromUIDbToJson();
    //END TODO
    QJsonDocument jsonDoc(myJsonMapperConfig);
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

void MapperJsonConfig::FromUIDbToJson()
{
    //clear it.
    myJsonMapperConfig = QJsonObject();

    //mapping:
    //  sources list s0-sN
    //  destinations list d0-dN
    QJsonArray srcArray;
    QJsonArray dstArray;
    //NOTE: this heavily relies on the mapSrcs and mapDsts
    // to be in sync, kind of like what was in the original json file actually...
    for (int i=0; i<myUIDbModel.getMapSrcs().size(); i++)
    {
        QJsonObject srcObj;
        QString id = "s"+QString::number(i);
        int sigIdx = myUIDbModel.getMapSrcs().at(i);

        //insert puts it at 0
        srcObj.insert("signal", myUIDbModel.getSigName(sigIdx));
        srcObj.insert("device", myUIDbModel.getSigDevName(sigIdx));
        srcObj.insert("id", id);

        QJsonObject dstObj;
        id = "d"+QString::number(i);
        sigIdx = myUIDbModel.getMapDsts().at(i);
        dstObj.insert("signal", myUIDbModel.getSigName(sigIdx));
        dstObj.insert("device", myUIDbModel.getSigDevName(sigIdx));
        dstObj.insert("id", id);

        srcArray.append(srcObj);
        dstArray.append(dstObj);


    }
    QJsonObject mappingsObj;
    mappingsObj.insert("sources", srcArray);
    mappingsObj.insert("destinations", dstArray);

    myJsonMapperConfig.insert("mapping", mappingsObj);

    //signal list:
    QJsonArray sigsArray;
    for (int i=0; i<myUIDbModel.getNumSigs(); i++)
    {
        QJsonObject sig;
        sig.insert("name", myUIDbModel.getSigName(i));
        sig.insert("device", myUIDbModel.getSigDevName(i));
        sig.insert("direction", myUIDbModel.getSigDir(i));
        sigsArray.append(sig);

    }
    myJsonMapperConfig.insert("signals", sigsArray);
}

bool MapperJsonConfig::ParseJsonObject(const QJsonObject& json_obj)
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
                    mySourceList.append(mapper_src);

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
                    myDestinationList.append(mapper_dst);
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
                qDebug() << "***SIGNALS (FLAT view)***";
                QJsonArray sig_arr = val.toObject()["signals"].toArray();
                for (int i=0; i<sig_arr.size(); i++)
                {
                    QJsonObject curr_sig = sig_arr.at(i).toObject();
                    QString sig_name = curr_sig["name"].toString();
                    QString sig_devname = curr_sig["device"].toString();
                    QString direction = curr_sig["direction"].toString();
                    qDebug() <<" Parsed signal: " << sig_devname<<"/"<<sig_name<<" dir= "<<direction;
                    MAPPER_SIGNAL signal;
                    signal.name = sig_name;
                    signal.dev = sig_devname;
                    signal.direction = direction;
                    mySignals.append(signal);
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
