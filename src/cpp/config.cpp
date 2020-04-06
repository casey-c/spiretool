#include "src/h/config.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>

Config::Config(QString filename) :
    configLocation(filename),
    savesLocation("C:/Program Files (x86)/Steam/steamapps/common/SlayTheSpire/saves"),
    runsLocation("C:/Program Files (x86)/Steam/steamapps/common/SlayTheSpire/runs"),
    potionWrite(true),
    potionOut("potions.txt"),
    potionFormat("$% !Potions"),
    sozuOverride(true),
    wbsOverride(true),
    sozuFormat("0% !Potions"),
    wbsFormat("100% !Potions")
{
    this->loadFromFile();

}

void Config::updateConfig(QString savesLocation,
                          QString runsLocation,
                          bool potionWrite,
                          QString potionOut,
                          QString potionFormat,
                          bool sozuOverride,
                          bool wbsOverride,
                          QString sozuFormat,
                          QString wbsFormat
                          ) {
    this->savesLocation = savesLocation;
    this->runsLocation = runsLocation;

    this->potionWrite = potionWrite;
    this->potionOut = potionOut;
    this->potionFormat = potionFormat;

    this->sozuOverride = sozuOverride;
    this->wbsOverride = wbsOverride;

    this->sozuFormat = sozuFormat;
    this->wbsFormat = wbsFormat;

    writeToFile();
}

void Config::loadFromFile() {
    // TODO

    QFile configFile(configLocation);
    if (configFile.exists()) {
        if (configFile.open(QFile::ReadOnly)) {

            QByteArray data = configFile.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(data);
            QJsonObject obj = doc.object();

            if (obj.contains("saves_folder"))
                this->savesLocation = obj["saves_folder"].toString();
            if (obj.contains("runs_folder"))
                this->runsLocation = obj["runs_folder"].toString();
            if (obj.contains("potions_write_to_file"))
                this->potionWrite = obj["potions_write_to_file"].toBool();
            if (obj.contains("potions_file"))
                this->potionOut = obj["potions_file"].toString();
            if (obj.contains("potions_format"))
                this->potionFormat = obj["potions_format"].toString();

            if (obj.contains("sozu_override"))
                this->sozuOverride = obj["sozu_override"].toBool();
            if (obj.contains("wbs_override"))
                this->wbsOverride = obj["wbs_override"].toBool();

            if (obj.contains("sozu_format"))
                this->sozuFormat = obj["sozu_format"].toString();
            if (obj.contains("wbs_format"))
                this->wbsFormat = obj["wbs_format"].toString();

            configFile.close();
        }
        else {
            qDebug() << "ERROR: couldn't read the config file";
        }
    }
    else {
        qDebug() << "error: no config found";
        //TODO: open tutorial for first boot?
    }

}

void Config::writeToFile() {
    QJsonObject obj;
    obj["saves_folder"] = this->savesLocation;
    obj["runs_folder"] = this->runsLocation;
    obj["potions_write_to_file"] = this->potionWrite;
    obj["potions_file"] = this->potionOut;
    obj["potions_format"] = this->potionFormat;

    obj["sozu_override"] = this->sozuOverride;
    obj["wbs_override"] = this->wbsOverride;
    obj["sozu_format"] = this->sozuFormat;
    obj["wbs_format"] = this->wbsFormat;

    QFile configFile(configLocation);
    if (configFile.open(QFile::Truncate | QFile::WriteOnly)) {

        QJsonDocument doc(obj);
        configFile.write(doc.toJson());

        configFile.close();
    }
    else {
        qDebug() << "ERROR: can't write config file";
    }
}
