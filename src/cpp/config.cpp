#include "src/h/config.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>

Config::Config(QString filename) :
    configLocation(filename),
    savesLocation("C:/Program Files (x86)/Steam/steamapps/common/SlayTheSpire/saves"),
    runsLocation("C:/Program Files (x86)/Steam/steamapps/common/SlayTheSpire/runs"),

    uncLocation("uncommonChance.txt"),
    rareLocation("rareChance.txt"),
    potionLocation("potions.txt"),

    potionWrite(true),
    sozuOverride(true),
    wbsOverride(true),

    uncWrite(false),
    rareWrite(true),

    potionFormat("$% !Potions"),
    sozuFormat("0% !Potions"),
    wbsFormat("100% !Potions"),

    uncFormat("Uncommon: $% (@%)"),
    rareFormat("Rare: $% (@%)")
{
    this->loadFromFile();

}

void Config::updateConfig(QString savesLocation,
                          QString runsLocation,
                          QString potionLocation,

                          QString uncLocation,
                          QString rareLocation,

                          bool potionWrite,
                          bool sozuOverride,
                          bool wbsOverride,
                          bool uncWrite,
                          bool rareWrite,

                          QString potionFormat,
                          QString sozuFormat,
                          QString wbsFormat,

                          QString uncFormat,
                          QString rareFormat
                          ) {
    this->savesLocation = savesLocation;
    this->runsLocation = runsLocation;
    this->potionLocation = potionLocation;

    this->uncLocation = uncLocation;
    this->rareLocation = rareLocation;

    this->potionWrite = potionWrite;
    this->sozuOverride = sozuOverride;
    this->wbsOverride = wbsOverride;

    this->uncWrite = uncWrite;
    this->rareWrite = rareWrite;

    this->potionFormat = potionFormat;
    this->sozuFormat = sozuFormat;
    this->wbsFormat = wbsFormat;

    this->uncFormat = uncFormat;
    this->rareFormat = rareFormat;

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

            // Main StS saves/runs location
            if (obj.contains("saves_folder"))
                this->savesLocation = obj["saves_folder"].toString();
            if (obj.contains("runs_folder"))
                this->runsLocation = obj["runs_folder"].toString();

            // Output file locations
            if (obj.contains("potions_file"))
                this->potionLocation = obj["potions_file"].toString();
            if (obj.contains("unc_file"))
                this->uncLocation = obj["unc_file"].toString();
            if (obj.contains("rare_file"))
                this->rareLocation = obj["rare_file"].toString();

            // Enabled booleans
            if (obj.contains("potions_write_to_file"))
                this->potionWrite = obj["potions_write_to_file"].toBool();
            if (obj.contains("sozu_override"))
                this->sozuOverride = obj["sozu_override"].toBool();
            if (obj.contains("wbs_override"))
                this->wbsOverride = obj["wbs_override"].toBool();

            if (obj.contains("unc_write_to_file"))
                this->uncWrite = obj["unc_write_to_file"].toBool();
            if (obj.contains("rare_write_to_file"))
                this->rareWrite = obj["rare_write_to_file"].toBool();

            // Format strings
            if (obj.contains("potions_format"))
                this->potionFormat = obj["potions_format"].toString();
            if (obj.contains("sozu_format"))
                this->sozuFormat = obj["sozu_format"].toString();
            if (obj.contains("wbs_format"))
                this->wbsFormat = obj["wbs_format"].toString();

            if (obj.contains("unc_format"))
                this->uncFormat = obj["unc_format"].toString();
            if (obj.contains("rare_format"))
                this->rareFormat = obj["rare_format"].toString();

            configFile.close();
        }
        else {
            qWarning() << "ERROR: couldn't read the config file";
        }
    }
    else {
        qWarning() << "error: no config found";
        //TODO: open tutorial for first boot?
    }

}

void Config::writeToFile() {
    // Create the JSON object and populate it
    QJsonObject obj;

    // Main StS saves/runs location
    obj["saves_folder"] = this->savesLocation;
    obj["runs_folder"] = this->runsLocation;

    // Output files
    obj["potions_file"] = this->potionLocation;
    obj["unc_file"] = this->uncLocation;
    obj["rare_file"] = this->rareLocation;

    // Enabled booleans
    obj["potions_write_to_file"] = this->potionWrite;
    obj["sozu_override"] = this->sozuOverride;
    obj["wbs_override"] = this->wbsOverride;

    obj["unc_write_to_file"] = this->uncWrite;
    obj["rare_write_to_file"] = this->rareWrite;

    // Format Strings
    obj["potions_format"] = this->potionFormat;
    obj["sozu_format"] = this->sozuFormat;
    obj["wbs_format"] = this->wbsFormat;
    obj["unc_format"] = this->uncFormat;
    obj["rare_format"] = this->rareFormat;

    // Write the JSON to the file specified by the config location
    QFile configFile(configLocation);
    if (configFile.open(QFile::Truncate | QFile::WriteOnly)) {

        QJsonDocument doc(obj);
        configFile.write(doc.toJson());

        configFile.close();
    }
    else {
        qWarning() << "ERROR: can't write config file";
    }
}
