#ifndef CONFIG_H
#define CONFIG_H

#include <QString>


class Config
{
public:
    Config(QString filename);

    void updateConfig(QString savesLocation,
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
                      );

    QString getSavesLocation() { return savesLocation; }
    QString getRunsLocation() { return runsLocation; }

    // Potions
    bool getPotionWrite() { return potionWrite; }
    QString getPotionLocation() { return potionLocation; }
    QString getPotionFormat() { return potionFormat; }

    bool getSozuOverride() { return sozuOverride; }
    bool getWbsOverride() { return wbsOverride; }
    QString getSozuFormat() { return sozuFormat; }
    QString getWbsFormat() { return wbsFormat; }

    // Card Chances
    bool getUncWrite() { return uncWrite; }
    bool getRareWrite() { return rareWrite; }

    QString getUncommonLocation() { return uncLocation; }
    QString getUncommonFormat() { return uncFormat; }

    QString getRareLocation() { return rareLocation; }
    QString getRareFormat() { return rareFormat; }

private:
    QString configLocation;
    QString savesLocation;
    QString runsLocation;

    QString uncLocation;
    QString rareLocation;
    QString potionLocation;

    bool potionWrite;
    bool sozuOverride;
    bool wbsOverride;
    bool uncWrite;
    bool rareWrite;

    QString potionFormat;
    QString sozuFormat;
    QString wbsFormat;

    QString uncFormat;
    QString rareFormat;



    void loadFromFile();
    void writeToFile();

};

#endif // CONFIG_H
