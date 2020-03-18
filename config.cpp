#include "config.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>

Config::Config(QString filename) :
    configLocation(filename),
    potionWrite(true),
    potionOut("potions.txt"),
    potionFormat("$% !Potions")
{
    this->loadFromFile();

}

void Config::updateConfig(QString savesLocation, bool potionWrite, QString potionOut, QString potionFormat) {
    this->savesLocation = savesLocation;
    this->potionWrite = potionWrite;
    this->potionOut = potionOut;
    this->potionFormat = potionFormat;

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

            if (obj.contains("saves_folder")) {
                this->savesLocation = obj["saves_folder"].toString();
            }
            if (obj.contains("potions_write_to_file")) {
                this->potionWrite = obj["potions_write_to_file"].toBool();
            }
            if (obj.contains("potions_file")) {
                this->potionOut = obj["potions_file"].toString();
            }
            if (obj.contains("potions_format")) {
                this->potionFormat = obj["potions_format"].toString();
            }

            configFile.close();
        }
        else {
            qDebug() << "ERROR: couldn't read the config file";
        }
    }
    else {
        qDebug() << "error: no config found";
        //TODO: open tutorial?
    }

}

void Config::writeToFile() {
    // TODO

    QJsonObject obj;
    obj["saves_folder"] = this->savesLocation;
    obj["potions_write_to_file"] = this->potionWrite;
    obj["potions_file"] = this->potionOut;
    obj["potions_format"] = this->potionFormat;

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
